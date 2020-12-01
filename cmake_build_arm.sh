#! /bin/bash

mkdir bin_arm32
cd bin_arm32

# ubuntu 20.04
# sudo apt-get install g++-arm-linux-gnueabi

cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain_arm.cmake -G "Unix Makefiles" -DCOMMLITE_EXE_STATIC=ON ..

cmake --build .