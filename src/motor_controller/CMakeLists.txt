cmake_minimum_required(VERSION 3.5)
project(motor_controller)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

# Find packages in ROS 2
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(motor_feedback_msgs REQUIRED)

# Create ament index resource which references the libraries in the binary dir
ament_export_dependencies(ament_cmake)
ament_export_dependencies(rclcpp)
ament_export_dependencies(std_msgs)
ament_export_dependencies(geometry_msgs)
ament_export_dependencies(tf2)
ament_export_dependencies(motor_feedback_msgs)

include_directories(
  include
  ${rclcpp_INCLUDE_DIRS}
  ${motor_feedback_msgs_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}
  /usr/local/include/modbus
)

# Add executables
add_executable(main_motorController
  src/main_motorController.cpp
  include/${PROJECT_NAME}/Oriental_BLVD_KRD.cpp
)
ament_target_dependencies(main_motorController
  rclcpp
  std_msgs
  geometry_msgs
  tf2
  motor_feedback_msgs
)
target_link_libraries(main_motorController modbus)

install(TARGETS
  main_motorController
  DESTINATION lib/${PROJECT_NAME}
)

# Install launch files
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

# Install directories
install(DIRECTORY
  include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}
)

ament_package()