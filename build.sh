#!/bin/sh

cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=$(pwd)/yaml/build/release

cmake --build build/debug

