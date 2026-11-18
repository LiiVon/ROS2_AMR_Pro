cmake_minimum_required(VERSION 3.8)
project(robot_hardware)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(robot_interfaces_core REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)

add_library(robot_hardware_backends SHARED
  src/protocol/packet.cpp
  src/kinematics/kinematcis.cpp
  src/transport/backend_factory.cpp
  src/transport/mock_backend.cpp
  src/transport/serial_backend.cpp
  src/transport/udp_backend.cpp
  src/system/adapter.cpp
)

target_include_directories(robot_hardware_backends PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
set_target_properties(robot_hardware_backends PROPERTIES POSITION_INDEPENDENT_CODE ON)


if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(chassis_packet_test test/chassis_packet_test.cpp)
  target_link_libraries(chassis_packet_test robot_hardware_backends)

  ament_add_gtest(chassis_system_adapter_test test/chassis_system_adapter_test.cpp)
  target_link_libraries(chassis_system_adapter_test robot_hardware_backends)
endif()

ament_package()
