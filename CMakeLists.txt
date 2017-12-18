project(iot_chess C CXX)
cmake_minimum_required(VERSION 2.8)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x -o uart-send -lwiringPi")
endif()

add_subdirectory(Stockfish)
#[[add_subdirectory(Network)]]
add_subdirectory(Chess)

