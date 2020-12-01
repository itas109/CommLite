mkdir bin_x86

cd bin_x86

set path=D:\Qt\Qt5.6.3\Tools\mingw492_32\bin;%path%

cmake -G "MinGW Makefiles" -DCOMMLITE_EXE_STATIC=ON ..

cmake --build .

pause