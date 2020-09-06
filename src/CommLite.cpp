/**
 * @file CommLite.cpp
 * @author itas109 (itas109@qq.com) \n\n
 * Blog : https://blog.csdn.net/itas109 \n
 * Github : https://github.com/itas109 \n
 * QQ Group : 12951803
 * @brief  CommLite, a lightweight serial port application based CSerialPort 基于CSerialPort的轻量级串口助手CommLite
 * @version 1.0.0
 * @date 2020-09-06
 * @copyright The CommLite is Copyright (C) 2020 itas109. \n
 * Contact: itas109@qq.com \n\n
 *  You may use, distribute and copy the CommLite under the terms of \n
 *  GNU Lesser General Public License version 3, which is displayed below.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "tui.h"

#include <iostream>

#define COMMLITE_VERSION "1.0.0"

// CSerialPort
#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"
#include <vector>
using namespace itas109;
using namespace std;

class mySlot : public has_slots<>
{
public:
	mySlot(CSerialPort & sp)
    {
	    recLen = -1;
        m_sp = sp; 
    };

	void OnSendMessage()
	{
		//read
		recLen = m_sp.readAllData(str);

		if(recLen > 0)
		{
			str[recLen] = '\0';

            bodymsg(str);
		}
	};

private:
	mySlot(){};

private:
	CSerialPort m_sp;

	char str[1024];
	int recLen;
};

std::string ParityArray[] = {"None", "Odd", "Even", "Mark", "Space" };
std::string StopArray[] = {"1", "1.5", "2"};

std::string m_portName = "";
std::string m_BaudRate = "";
std::string m_Parity = "";
std::string m_DataBit = "";
std::string m_StopBit = "";

std::string m_portSetting = "";
std::string m_sendData = "https://github.com/itas109/CSerialPort";

vector<SerialPortInfo> m_availablePortsList;
CSerialPort m_serialPort;
mySlot receive(m_serialPort);

std::string getPortSetting(void)
{
    std::string result = "";

    char *fieldname[6] =
    {
        "PortName", "BaudRate", "Parity", "DataBit", "StopBit", (char *)0
    };

    char *fieldbuf[5];
    WINDOW *wbody = bodywin();
    int i, field = 50;

    for (i = 0; i < 5; i++)
    {
    	fieldbuf[i] = new char[field + 1];
    	fieldbuf[i] = "";
        //fieldbuf[i] = calloc(1, field + 1);
        //fieldbuf[i] = "";
    }
    
    m_availablePortsList = CSerialPortInfo::availablePortInfos();
    if(m_availablePortsList.size() > 0)
    {
    	fieldbuf[0] = const_cast<char*>(m_availablePortsList[0].portName.c_str());
    }
    else
    {
    	fieldbuf[0] = "/dev/S0";
    }
    
    //fieldbuf[0] = "/dev/S0";
    fieldbuf[1] = "9600";
    fieldbuf[2] = "None";
    fieldbuf[3] = "8";
    fieldbuf[4] = "1";
    

    if (getstrings(fieldname, fieldbuf, field) != KEY_ESC)
    {
	m_portName = fieldbuf[0];
	m_BaudRate = fieldbuf[1];
	m_Parity = fieldbuf[2];
	m_DataBit = fieldbuf[3];
	m_StopBit = fieldbuf[4];
	
	result = result.append(m_portName)
                .append(",").append(m_BaudRate)
				.append(",").append(m_Parity)
				.append(",").append(m_DataBit)
				.append(",").append(m_StopBit);
    }
/*
    for (i = 0; i < 5; i++)
    {
        //free(fieldbuf[i]);
        delete fieldbuf[i];
    }
    */
    return result;
}

std::string getSendData(void)
{
    std::string result;

    char *fieldname[2] =
    {
        "sendData", (char *)0
    };

    char *fieldbuf[1];
    WINDOW *wbody = bodywin();
    int i, field = 50;

    for (i = 0; i < 1; i++)
    {
    	fieldbuf[i] = new char[field + 1];
    }
    
    fieldbuf[0] = const_cast<char*>(m_sendData.c_str());

    if (getstrings(fieldname, fieldbuf, field) != KEY_ESC)
    {
	m_sendData = fieldbuf[0];
	
	result = m_sendData;
    }
/*
    for (i = 0; i < 1; i++)
    {
        //free(fieldbuf[i]);
        delete fieldbuf[i];
    }
    */
    return result;
}

/***************************** forward declarations ***********************/
// menu level 1
void subSetting(void), subSend(void), subReceive(void),subHelp(void);
// menu level 2
void portSetting(void), open(void), close(void),getCommStatus(void);
void send(void);
void clearReceive(void);
void author(void), CSerialPortAbout(void), commliteAbout(void);

/***************************** menus initialization ***********************/

menu MainMenu[] =
{
    { "Open", subSetting, "Serial Port Setting, Open and Close" },
    { "Send", subSend, "Serial Port Send" },
    { "Receive", subReceive, "Serial Port Receive" },
    { "Help", subHelp, "CommLite Help" },
    { "", (FUNC)0, "" }   // always add this as the last item!
};

menu SubMenuSetting[] =
{
    { "PortSetting", portSetting, "set port name(COM1 or /dev/ttyS0), baudrate(9600), parity(None), databit(8), stopbit(1)" },
    { "Open", open, "open serial port" },
    { "Close", close, "close serial port" },
    { "GetCommStatus", getCommStatus, "get Comm Status,such as [ opened ] - /dev/ttyS0,9600,None,8,1" },
    { "Exit", DoExit, "Exit Appliction" },
    { "", (FUNC)0, "" }
};

menu SubMenuSend[] =
{
    { "Send", send, "send some data to serial port" },
    { "", (FUNC)0, "" }
};

menu SubMenuReceive[] =
{
    { "Clear Receive", clearReceive, "clear receive" },
    { "", (FUNC)0, "" }
};

menu SubMenuHelp[] =
{
    { "Author", author, "author about" },
    { "CSerialPortAbout", CSerialPortAbout, "CSerialPort about" },
    { "CommLiteAbout", commliteAbout, "commlite about" },
    { "", (FUNC)0, "" }
};

/***************************** main menu functions ************************/

void subSetting(void)
{
    domenu(SubMenuSetting);
}

void subSend(void)
{
    domenu(SubMenuSend);
}

void subReceive(void)
{
    domenu(SubMenuReceive);
}

void subHelp(void)
{
    domenu(SubMenuHelp);
}

/***************************** SubMenuSetting functions *************************/

void portSetting(void)
{
	m_portSetting = getPortSetting();
	infomsg(const_cast<char*>(m_portSetting.c_str()));
}

void open(void)
{
    if(m_portSetting.empty())
    {
        errormsg("Please set and open serial port first");
        return;
    }

    m_serialPort.init(m_portName);//windows:COM1 Linux:/dev/ttyS0

	m_serialPort.open();
	
	if(m_serialPort.isOpened())
	{
        char str[256];
        snprintf(str,sizeof(str),"open success. %s,%d,%s,%d,%s\n",
                const_cast<char*>(m_portName.c_str()),
                m_serialPort.getBaudRate(),
                const_cast<char*>(ParityArray[(int)(m_serialPort.getParity())].c_str()),
                (int)(m_serialPort.getDataBits()),
                const_cast<char*>(StopArray[(int)(m_serialPort.getStopBits())].c_str())
            );
        infomsg(str);

        //connect for read
        m_serialPort.readReady.connect(&receive, &mySlot::OnSendMessage);
	}
	else
	{
        char str[256];
        snprintf(str,sizeof(str),"open %s failed, error code : %d\n", const_cast<char*>(m_portName.c_str()), m_serialPort.getLastError()); 
        errormsg(str);
	}
}

void close(void)
{
    infomsg("close \n");
    m_serialPort.readReady.disconnect_all();
    m_serialPort.close();
}

void getCommStatus(void)
{
    char str[256];
    snprintf(str,sizeof(str),"[ %s ] - %s", m_serialPort.isOpened()?"opened":"closed", const_cast<char*>(m_portSetting.c_str())); 
    infomsg(str);
}
/***************************** SubMenuSend functions *************************/

void send(void)
{
    if(m_serialPort.isOpened())
    {
        std::string str = getSendData();
        m_serialPort.writeData(const_cast<char*>(str.c_str()), str.length());
    }
    else
    {
        errormsg("Please set and open serial port first");
    }
}

/***************************** SubMenuReceive functions *************************/

void clearReceive(void)
{
    clsbody();
}

/***************************** SubMenuHelp functions *************************/

void author(void)
{
    char str[256];
    snprintf(str,sizeof(str),"Author: itas109 Email: itas109@qq.com Github: https://github.com/itas109"); 
    infomsg(str);
}

void CSerialPortAbout(void)
{
    char str[256];
    snprintf(str,sizeof(str),"CSerialPort(%s) cross-platform serial port library",const_cast<char*>(m_serialPort.getVersion().c_str())); 
    infomsg(str);
}

void commliteAbout(void)
{
    char str[256];
    snprintf(str,sizeof(str),"CommLite Based CSerialPort - https://github.com/itas109/CommLite"); 
    infomsg(str);
}

/***************************** start main menu  ***************************/

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "");

    char title[256];
    snprintf(title,sizeof(title),"CommLite v%s Based CSerialPort - https://github.com/itas109/CSerialPort", COMMLITE_VERSION); 

    startmenu(MainMenu, title);

    return 0;
}