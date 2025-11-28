cmake_minimum_required(VERSION 3.10)
project(HttpEngine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/inc)
include_directories(${INCLUDE_DIR})

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

set(SOURCE_CORE 
    ${SOURCE_DIR}/Core/IOManager.cpp
    ${SOURCE_DIR}/Core/WorkerManager.cpp
)

set(SOURCE_FILES
    ${SOURCE_CORE}
)

add_executable(HttpEngine main.cpp ${SOURCE_FILES})