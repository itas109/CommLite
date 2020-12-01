#! /bin/bash

mkdir bin_aarch64
cd bin_aarch64

# ubuntu 20.04
# sudo apt-get install g++-aarch64-linux-gn

cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain_aarch64.cmake -G "Unix Makefiles" -DCOMMLITE_EXE_STATIC=ON ..

cmake --build .