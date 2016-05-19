cmake_minimum_required(VERSION 3.5)
project(RocketBunny)

set(Boost_USE_STATIC_LIBS TRUE)
set(Boost_USE_MULTITHREADED TRUE)
set(Boost_USE_STATIC_RUNTIME FALSE)

#boost flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(Boost COMPONENTS system filesystem REQUIRED)

set(SOURCE_FILES
        main.cpp rocket.cpp rocket.h)

include_directories(
        ${Boost_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
)

add_executable(RocketBunny ${SOURCE_FILES})
TARGET_LINK_LIBRARIES( RocketBunny ${OpenCV_LIBS} ${Boost_LIBRARIES})