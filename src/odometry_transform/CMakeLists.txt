cmake_minimum_required(VERSION 3.5)
project(odometry_transform)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

## Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(motor_feedback_msgs REQUIRED)
find_package(nav_msgs REQUIRED)

# Create ament index resource which references the libraries in the binary dir
ament_export_dependencies(ament_cmake)
ament_export_dependencies(rclcpp)
ament_export_dependencies(std_msgs)
ament_export_dependencies(tf2)
ament_export_dependencies(tf2_ros)
ament_export_dependencies(motor_feedback_msgs)
ament_export_dependencies(nav_msgs)

## Add executables
add_executable(main_tf_odometry src/main_tf_odometry.cpp)
ament_target_dependencies(main_tf_odometry
  rclcpp
  std_msgs
  tf2
  tf2_ros
  motor_feedback_msgs
  nav_msgs
)

target_link_libraries(main_tf_odometry ${main_tf_odometry_LIBRARIES})

## Install
install(TARGETS
  main_tf_odometry
  DESTINATION lib/${PROJECT_NAME}
)

# Install launch files
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

ament_package()
