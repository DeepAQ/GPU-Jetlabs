cmake_minimum_required(VERSION 2.8.3)
project(http_remote)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  cv_bridge
  image_transport
)

catkin_package(
  CATKIN_DEPENDS roscpp rospy std_msgs cv_bridge image_transport
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_definitions(-std=c++11)
add_executable(http_remote src/http_remote.cpp)
target_link_libraries(http_remote microhttpd ${catkin_LIBRARIES})
