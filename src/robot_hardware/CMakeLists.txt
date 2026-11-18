cmake_minimum_required(VERSION 3.8)
project(robot_hardware)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)


add_library(robot_hardware_backends SHARED
  src/protocol/packet.cpp
  src/kinematics/kinematcis.cpp)

target_include_directories(robot_hardware_backends PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
set_target_properties(robot_hardware_backends PROPERTIES POSITION_INDEPENDENT_CODE ON)


if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(chassis_packet_test test/chassis_packet_test.cpp)
  target_link_libraries(chassis_packet_test robot_hardware_backends)
endif()

ament_package()
