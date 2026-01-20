#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>

#include <librdkafka/rdkafka.h>
#include <sstream>

class JointStateKafkaBridge : public rclcpp::Node
{
public:
  JointStateKafkaBridge()
  : Node("joint_state_kafka_bridge")
  {
    // ---- Parameters ----
    broker_ = this->declare_parameter<std::string>(
        "broker", "192.168.1.1:9092");

    kafka_topic_ = this->declare_parameter<std::string>(
        "kafka_topic", "test-topic");

    ros_topic_ = this->declare_parameter<std::string>(
        "ros_topic", "/joint_states");

    init_kafka();

    sub_ = this->create_subscription<sensor_msgs::msg::JointState>(
        ros_topic_, 10,
        std::bind(&JointStateKafkaBridge::callback, this, std::placeholders::_1));

    RCLCPP_INFO(get_logger(), "ROS → Kafka bridge started");
    RCLCPP_INFO(get_logger(), "Broker: %s", broker_.c_str());
    RCLCPP_INFO(get_logger(), "Kafka topic: %s", kafka_topic_.c_str());
    RCLCPP_INFO(get_logger(), "ROS topic: %s", ros_topic_.c_str());
  }

  ~JointStateKafkaBridge()
  {
    if (producer_) {
      rd_kafka_flush(producer_, 3000);
      rd_kafka_destroy(producer_);
    }
  }

private:
  void init_kafka()
  {
    char errstr[512];
    rd_kafka_conf_t *conf = rd_kafka_conf_new();

    if (rd_kafka_conf_set(conf, "bootstrap.servers",
                          broker_.c_str(),
                          errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK)
    {
      throw std::runtime_error(errstr);
    }

    producer_ = rd_kafka_new(
        RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));

    if (!producer_) {
      throw std::runtime_error(errstr);
    }
  }

  void callback(const sensor_msgs::msg::JointState::SharedPtr msg)
  {
    std::ostringstream ss;
    ss << "{ \"name\": [";
    for (size_t i = 0; i < msg->name.size(); ++i) {
      ss << "\"" << msg->name[i] << "\"";
      if (i + 1 < msg->name.size()) ss << ",";
    }
    ss << "], \"position\": [";
    for (size_t i = 0; i < msg->position.size(); ++i) {
      ss << msg->position[i];
      if (i + 1 < msg->position.size()) ss << ",";
    }
    ss << "] }";

    std::string payload = ss.str();

    rd_kafka_resp_err_t err = rd_kafka_producev(
        producer_,
        RD_KAFKA_V_TOPIC(kafka_topic_.c_str()),
        RD_KAFKA_V_VALUE(
            const_cast<char*>(payload.data()),
            payload.size()),        
        RD_KAFKA_V_END);

    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
      RCLCPP_ERROR(get_logger(),
          "Kafka produce failed: %s", rd_kafka_err2str(err));
    }

    rd_kafka_poll(producer_, 0);
  }

  std::string broker_;
  std::string kafka_topic_;
  std::string ros_topic_;

  rd_kafka_t *producer_{nullptr};
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr sub_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<JointStateKafkaBridge>());
  rclcpp::shutdown();
  return 0;
}
