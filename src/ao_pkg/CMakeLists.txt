cmake_minimum_required(VERSION 2.8.3)
project(ao_pkg)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
        cv_bridge
        sensor_msgs
        geometry_msgs
        image_transport
        roscpp
        rosbag
)
find_package(Eigen3 REQUIRED)
find_package(X11 REQUIRED)
find_package(OpenCV 4 REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)


## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   sensor_msgs#   std_msgs
# )

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

catkin_package(
#        LIBRARIES node_lib
#        DEPENDS Eigen
#        CATKIN_DEPENDS roscpp
)


add_definitions("-DENABLE_SSE")

# Set source files
set(lsd_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/util/settings.cpp
        ${PROJECT_SOURCE_DIR}/src/util/Undistorter.cpp
        ${PROJECT_SOURCE_DIR}/src/IOWrapper/Timestamp.cpp
        )
set(SOURCE_FILES
        ${lsd_SOURCE_FILES}
        ${PROJECT_SOURCE_DIR}/src/IOWrapper/ROS/ROSImageStreamThread.cpp
        )

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${catkin_INCLUDE_DIRS}
        ${EIGEN3_INCLUDE_DIR}

)
message("-- catkin_LIBRARIES : " ${catkin_LIBRARIES})
message("-- PROJECT_SOURCE_DIR : " ${PROJECT_SOURCE_DIR})

add_library(node_lib SHARED ${SOURCE_FILES})
target_link_libraries(node_lib ${OpenCV_LIBS} ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_executable(ao_node src/node.cpp src/util/ImageUtility.cpp)
target_link_libraries(ao_node node_lib ${catkin_LIBRARIES} ${X11_LIBRARIES} ${Boost_LIBRARIES})
add_executable(playground src/playground.cpp src/util/ImageUtility.cpp)
target_link_libraries(playground ${OpenCV_LIBS} ${Boost_LIBRARIES})
