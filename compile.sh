#! /bin/bash

git submodule init
git submodule update

rm -rf CommLite

g++ -fpermissive -o CommLite src/CommLite.cpp src/tui.cpp \
	thirdLib/CSerialPort/src/SerialPortInfo.cpp thirdLib/CSerialPort/src/SerialPortInfoBase.cpp thirdLib/CSerialPort/src/SerialPortInfoUnixBase.cpp \
	thirdLib/CSerialPort/src/SerialPort.cpp thirdLib/CSerialPort/src/SerialPortBase.cpp thirdLib/CSerialPort/src/SerialPortUnixBase.cpp \
	-I. -IthirdLib/CSerialPort/include \
	-L. -lpthread -lncurses
