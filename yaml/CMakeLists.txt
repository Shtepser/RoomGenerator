cmake_minimum_required(VERSION 3.18)

project(libyaml)

include(ExternalProject)

ExternalProject_Add(
    yaml
    GIT_REPOSITORY https://github.com/yaml/libyaml.git
    GIT_TAG 0.2.5
    INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>)

