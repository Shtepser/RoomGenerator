cmake_minimum_required(VERSION 3.18.4)
project(RoomGenerator)

find_package(yaml REQUIRED)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES generator-cli.cpp random.cpp random.hpp RoomContent.cpp RoomContent.hpp)
add_executable(room-generator-cli generator-cli.cpp random.cpp random.hpp RoomContent.cpp RoomContent.hpp)

add_executable(monsters-parser monsters-parser.cpp Monster.cpp Monster.hpp)
target_link_libraries(monsters-parser yaml)

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

