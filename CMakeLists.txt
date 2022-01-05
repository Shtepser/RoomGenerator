cmake_minimum_required(VERSION 3.18.4)
project(RoomGenerator)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES generator-cli.cpp random.cpp random.hpp RoomContent.cpp RoomContent.hpp)
add_executable(room-generator-cli ${SOURCES})

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

