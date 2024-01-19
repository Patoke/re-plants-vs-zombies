#!/usr/bin/env bash

rm -rf build
cmake -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake -G Ninja -B build
