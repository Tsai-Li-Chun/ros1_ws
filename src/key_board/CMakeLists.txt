cmake_minimum_required(VERSION 3.5)
project(keyboard)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

## Find dependencies
find_package(ament_cmake REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(Boost REQUIRED COMPONENTS 
  system 
  thread 
  program_options
)

set(THIS_PACKAGE_EXPORT_DEPENDS
  Boost
  geometry_msgs
  rclcpp
)

## Add executables
add_executable(keyboard_node src/keyboard.cpp)
ament_target_dependencies(keyboard_node
  sensor_msgs
  rclcpp
  Boost
  geometry_msgs
)

add_executable(joy_control_node src/joy_control.cpp)
ament_target_dependencies(joy_control_node
  sensor_msgs
  rclcpp
  geometry_msgs
)

add_executable(fake_cmdvel src/fake_cmdvel.cpp)
ament_target_dependencies(fake_cmdvel
  sensor_msgs
  rclcpp
  geometry_msgs
)

## Install
install(TARGETS
  keyboard_node
  joy_control_node
  fake_cmdvel
  DESTINATION lib/${PROJECT_NAME}
)

ament_package(CONFIG_EXTRAS "cmake/ConfigExtras.cmake")
