# ROS2 to Kafka bridge package
The package in this repository bridge specified ROS2 topics to Kafka topics

## Package installation

To install the packages from inside your workspace:
```console
cd src
git clone https://github.com/IdPDE/ros2kafka
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
Alternatively, these values can be hard-coded in the script.
In that case, the bridge can be run as follows:
```console
ros2 run ros2kafka joint_state_kafka_bridge
```