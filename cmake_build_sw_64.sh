#! /bin/bash

mkdir bin_sw_64
cd bin_sw_64

# ubuntu 20.04 or other
# this configure only for uos 20/deepin 20 ( swgcc710-nomieee-9916-cross.tar.gz )

export PATH=/usr/sw/swgcc710-nomieee-9916-cross/usr/bin:$PATH
#export LD_LIBRARY_PATH=/usr/sw/swgcc710-nomieee-9916-cross/usr/lib:$LD_LIBRARY_PATH
export CROSS_COMPILE=sw_64sw6a-sunway-linux-gnu-
export CC=${CROSS_COMPILE}gcc
export CXX=${CROSS_COMPILE}g++

cmake -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain_sw_64.cmake -G "Unix Makefiles" -DCOMMLITE_EXE_STATIC=ON ..

cmake --build .