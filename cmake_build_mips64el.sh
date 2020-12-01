#! /bin/bash

mkdir bin_mips64el
cd bin_mips64el

# ubuntu 20.04
# sudo apt-get install g++-aarch64-linux-gnu

cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain_mips64el.cmake -G "Unix Makefiles" -DCOMMLITE_EXE_STATIC=ON ..

cmake --build .