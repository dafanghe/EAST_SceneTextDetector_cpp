cmake_minimum_required(VERSION 2.8)
project(SceneTextDetection)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(SOURCE_FILES main.cpp scene_text_detector.cpp utils.cpp text_box.cpp polys.cpp)
set(EXECUTABLE DetectText)

# Add modules
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

find_package(TensorFlow REQUIRED)
find_package(Protobuf REQUIRED)
find_package(OpenCV REQUIRED highgui imgproc)
find_package(Boost COMPONENTS system filesystem REQUIRED) 

#set(TensorFlow_LIBRARIES "/usr/local/lib/libtensorflow_cc.so" "/usr/local/lib/libtensorflow_framework.so") 
#set(TensorFlow_LIBRARIES "/usr/local/lib/libtensorflow_cc.so" "/usr/local/lib/libtensorflow.so") 
set(TensorFlow_LIBRARIES "/usr/local/lib/libtensorflow.so" "/usr/local/lib/libtensorflow_cc.so" "/usr/local/lib/libtensorflow_framework.so")

message(STATUS "protobuf include dirs:" ${PROTOBUF_INCLUDE_DIRS})
message(STATUS "protobuf libraries:" ${PROTOBUF_LIBRARIES})
message(STATUS "tensorflow include dirs:" ${TensorFlow_INCLUDE_DIRS})
message(STATUS "tensorflow libraries:" ${TensorFlow_LIBRARIES})
message(STATUS "boost libraries:" ${Boost_LIBRARIES})
message(STATUS "boost include dir:" ${Boost_INCLUDE_DIRS})

# set variables for external dependencies
set(EXTERNAL_DIR "${PROJECT_SOURCE_DIR}/external" CACHE PATH "Location where external dependencies will installed")
set(DOWNLOAD_LOCATION "${EXTERNAL_DIR}/src" CACHE PATH "Location where external projects will be downloaded")
set(NSYNC_DIR "/usr/local/lib/python2.7/dist-packages/tensorflow/include/external/nsync/public")

mark_as_advanced(EXTERNAL_DIR DOWNLOAD_LOCATION)

include(Eigen)

set(PROJECT_INCLUDE_DIRS  ${TensorFlow_INCLUDE_DIRS} ${EXTERNAL_DIR}/include ${PROTOBUF_INCLUDE_DIRS} ${NSYNC_DIR} ${OpenCV_INCLUDE_DIRS} ${Boost})
set(PROJECT_LIBRARIES ${TensorFlow_LIBRARIES} ${PROTOBUF_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})
set(PROJECT_DEPENDENCIES Eigen)

#for test opencv
add_executable(test_opencv test.cpp)
target_link_libraries(test_opencv ${PROJECT_LIBRARIES})

include_directories(${PROJECT_INCLUDE_DIRS})
add_library(SceneTextDetector scene_text_detector.cpp utils.cpp polys.cpp)
add_executable(${EXECUTABLE} ${SOURCE_FILES})
target_link_libraries(${EXECUTABLE} ${PROJECT_LIBRARIES})
add_dependencies(${EXECUTABLE} ${PROJECT_DEPENDENCIES})
