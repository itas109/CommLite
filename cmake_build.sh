#! /bin/bash

mkdir bin
cd bin

cmake -G "Unix Makefiles" -DCOMMLITE_EXE_STATIC=ON ..

cmake --build .
