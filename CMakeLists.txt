cmake_minimum_required(VERSION 3.8)
project(ros2kafka)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(RDKAFKA REQUIRED rdkafka)

add_executable(joint_state_kafka_bridge
  src/joint_state_kafka_bridge.cpp)

ament_target_dependencies(
  joint_state_kafka_bridge
  rclcpp
  sensor_msgs)
  
target_include_directories(joint_state_kafka_bridge PRIVATE
  ${RDKAFKA_INCLUDE_DIRS})

target_link_libraries(joint_state_kafka_bridge
  ${RDKAFKA_LIBRARIES})


install(TARGETS
  joint_state_kafka_bridge
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
