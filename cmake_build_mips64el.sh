#! /bin/bash

mkdir bin_mips64el
cd bin_mips64el

# ubuntu 20.04
# sudo apt-get install g++-mips64el-linux-gnuabi64

cmake -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain_mips64el.cmake -G "Unix Makefiles" -DCOMMLITE_EXE_STATIC=ON ..

cmake --build .