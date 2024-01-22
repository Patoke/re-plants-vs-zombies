#!/usr/bin/env bash

rm -rf build
rm -f compile_commands.json
cmake -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake -G Ninja -B build
