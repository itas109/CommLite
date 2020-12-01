mkdir bin

cd bin

set path=D:\Qt\Qt5.12.9\Tools\mingw730_64\bin;%path%

cmake -G "MinGW Makefiles" -DCOMMLITE_EXE_STATIC=ON ..

cmake --build .

pause