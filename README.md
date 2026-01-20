# ROS2 to Kafka bridge package
The package in this repository bridge specified ROS2 topics to Kafka topics

## Package installation

To install the packages from inside your workspace:
```console
cd src
git clone https://github.com/PronkLJ/ros2kafka
```

## Running the ROS2Kafka bridge
```console
ros2 run ros2kafka joint_state_kafka_bridge   --ros-args   -p broker:=[IP:Port]   -p kafka_topic:=[test-topic]   -p ros_topic:=[/ros2topic]
```