# ROS2 to Kafka bridge package
The package in this repository bridge specified ROS2 topics to Kafka topics

## Package installation

To install the packages from inside your workspace:
```console
cd src
git clone https://github.com/PronkLJ/ros2kafka
```

## Required installs
```console
sudo apt install librdkafka-dev
```


## Running the ROS2Kafka bridge
```console
ros2 run ros2kafka joint_state_kafka_bridge --ros-args -p \
ip:=[IP:Port] -p \
kafka_topic:=[topic_name] -p \
ros2_topic:=[/ros2topic_name]
```