#!/bin/sh

cmake -S . -B build/release -DCMAKE_BUILD_TYPE=RelWithDebInfo

cmake --build build/release

