cmake_minimum_required(VERSION 2.8.3)
project(elikos_arena_detection)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  geometry_msgs
  nav_msgs
  image_transport
  roscpp
  std_msgs
  tf2
  eigen_conversions
  pcl_ros
)

find_package(OpenCV REQUIRED)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES DBSCAN
  CATKIN_DEPENDS cv_bridge geometry_msgs nav_msgs message_runtime roscpp std_msgs tf2 pcl_ros
)

include_directories(include ${catkin_INCLUDE_DIRS} ${OpenCV_LIBS})

add_library(DBSCAN src/DBSCAN.cpp)

add_executable( elikos_arena_detection
    src/iarc7_localization_node.cpp
    src/ArenaTracking.cpp
    src/PosePublisher.cpp
    src/LineIntersections.cpp
    src/GridFitting.cpp
    src/PositionEstimator.cpp
    include
)

add_dependencies(elikos_arena_detection ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(elikos_arena_detection DBSCAN ${catkin_LIBRARIES} ${OpenCV_LIBS})
