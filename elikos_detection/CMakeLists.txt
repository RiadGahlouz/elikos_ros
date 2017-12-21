cmake_minimum_required(VERSION 2.8.3)
project(elikos_detection)
set(CMAKE_CXX_FLAGS "-std=c++11 -I/usr/include/eigen3 ${CMAKE_CXX_FLAGS}")

find_package(catkin REQUIRED COMPONENTS
  roscpp
  cv_bridge
  elikos_ros
  image_transport
  geometry_msgs
  sensor_msgs
  std_msgs
  tf
  elikos_remote_calib_client
  nodelet
)
find_package(Qt5Gui)
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Test)
find_package(Qt5Concurrent)
find_package(Qt5OpenGL)
find_package(OpenCV 3 REQUIRED)


catkin_package(
    INCLUDE_DIRS src/elikos_transformation_utils src/elikos_context_utils src/blobdetection/include
    LIBRARIES elikos_transformation_utils elikos_context_utils
	CATKIN_DEPENDS roscpp cv_bridge elikos_ros image_transport sensor_msgs geometry_msgs std_msgs tf
    DEPENDS nodelet
)

include_directories(
	${catkin_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    src/elikos_context_utils
    src/elikos_camera_info
    src/elikos_transformation_utils
    src/blobdetection/include
)

add_library(elikos_blobdetection
    src/blobdetection/include/elikos_blobdetection_nodelet.hpp
    src/blobdetection/elikos_blobdetection_nodelet.cpp)

target_link_libraries(elikos_blobdetection
    ${catkin_LIBRARIES}
    ${OpenCV_LIBRARIES}
    yaml-cpp
)

add_executable(elikos_transformation
   src/elikos_transformation/elikos_transformation.cpp
   src/elikos_transformation/MessageHandler.h
   src/elikos_transformation/MessageHandler.cpp
   src/elikos_transformation/TransformationUnit.h
   src/elikos_transformation/TransformationUnit.cpp)

add_library(elikos_transformation_utils
    STATIC
    src/elikos_transformation_utils/TransformationUtils.cpp
    src/elikos_transformation_utils/TransformationUtils.h)
    
target_link_libraries(elikos_transformation_utils
    ${catkin_LIBRARIES}
    ${OpenCV_LIBRARIES}
  )

add_library(elikos_context_utils
    SHARED
    src/elikos_context_utils/CameraInfo.h
    src/elikos_context_utils/CameraInfo.cpp
    src/elikos_context_utils/QuadState.h
    src/elikos_context_utils/QuadState.cpp)

target_link_libraries(elikos_transformation
    ${catkin_LIBRARIES}
    ${OpenCV_LIBRARIES}
    elikos_transformation_utils
)

add_executable(elikos_remotecalib
    src/elikos_remotecalib/elikos_remotecalib.cpp
    src/elikos_remotecalib/MessageHandler.h
    src/elikos_remotecalib/MessageHandler.cpp
    src/elikos_remotecalib/Colors.hpp
    src/elikos_remotecalib/gui/headers/WindowCV.hpp
    src/elikos_remotecalib/gui/headers/CalibrationWindow.hpp
    src/elikos_remotecalib/gui/headers/ControlWindow.hpp
    src/elikos_remotecalib/gui/CalibrationWindow.cpp
    src/elikos_remotecalib/gui/ControlWindow.cpp
    src/elikos_remotecalib/gui/WindowCV.cpp)

target_link_libraries(elikos_remotecalib
    ${catkin_LIBRARIES}
    ${OpenCV_LIBRARIES}
)
