
set path=D:\Qt\Qt5.12.9\Tools\mingw730_64\bin;%path%

git submodule init
git submodule update

del /f /s /q CommLite.exe

g++ -o CommLite src/CommLite.cpp src/tui.cpp ^
	thirdLib/CSerialPort/src/SerialPortInfo.cpp thirdLib/CSerialPort/src/SerialPortInfoBase.cpp thirdLib/CSerialPort/src/SerialPortInfoWinBase.cpp ^
	thirdLib/CSerialPort/src/SerialPort.cpp thirdLib/CSerialPort/src/SerialPortBase.cpp thirdLib/CSerialPort/src/SerialPortWinBase.cpp ^
	-I. -IthirdLib/CSerialPort/include ^
	-L. -lpthread -lsetupapi -lpdcurses

@echo ping 127.0.0.1 -n 3 >nul

@echo exit

pause