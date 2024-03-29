cmake_minimum_required(VERSION 2.8.3)
project(elikos_localization)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(catkin REQUIRED COMPONENTS
    cmake_modules
    cv_bridge
    geometry_msgs
    elikos_ros
    elikos_detection
    nav_msgs
    image_transport
    roscpp
    std_msgs
    sensor_msgs
    tf2
    eigen_conversions
    pcl_ros
)
find_package(Qt5Gui)
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Test)
find_package(Qt5Concurrent)
find_package(Qt5OpenGL)

find_package(Eigen3 REQUIRED)
find_package(OpenCV 3 REQUIRED)

catkin_package(
    CATKIN_DEPENDS
    image_transport
    cv_bridge
    elikos_ros
    elikos_detection
    geometry_msgs
    nav_msgs
    message_runtime
    roscpp
    rospy
    sensor_msgs
    std_msgs
    tf2
    pcl_ros
)

catkin_python_setup()
catkin_add_nosetests(
  test/feature_tracking/unittest_unscented_kalman_filter.py
)

add_subdirectory(src/localization)


