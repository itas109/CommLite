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

#include <cstdlib> // std::atoi
#include <iostream>

#include "tui.h"

#define COMMLITE_VERSION "1.0.0"

// CSerialPort
#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"
#include <vector>
using namespace itas109;
using namespace std;

bool isShowHex = false;

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
            if(!isShowHex)
            {
                str[recLen] = '\0';
                bodymsg(str);
            }
            else
            {
                for (int i = 0; i < recLen; i++)
                {
                    snprintf(hexChar,sizeof(hexChar), "%02X ", str[i] & 0xFF);// two bit hex +  one bit space + '\0'
                    hexChar[sizeof(hexChar)-1] = '\0';
                    bodymsg(hexChar);
                }
            }
        }
	};

private:
	mySlot(){};

private:
	CSerialPort m_sp;

    int recLen;
    char hexChar[4];// two bit hex +  one bit space + '\0'
	char str[1024];
};

std::string ParityArray[] = {"None", "Odd", "Even", "Mark", "Space" };
std::string StopArray[] = {"1", "1.5", "2"};

std::string m_portName = "";
std::string m_BaudRate = "9600";
std::string m_Parity = "None";
std::string m_DataBit = "8";
std::string m_StopBit = "1";

std::string m_portSetting = "";
std::string m_sendData = "https://github.com/itas109/CSerialPort";

vector<SerialPortInfo> m_availablePortsList;
CSerialPort m_serialPort;
mySlot receive(m_serialPort);

std::string getPortSetting(void)
{
    std::string result = "";

    char *fieldname[6] = {"PortName", "BaudRate", "Parity", "DataBit", "StopBit", (char *)0 };

    char *fieldbuf[5];
    int i, field = 50;

    for (i = 0; i < 5; i++)
    {
        fieldbuf[i] = NULL;
    	fieldbuf[i] = new char[field + 1];
    }
    
    if(m_portName.empty())
    {
        m_availablePortsList = CSerialPortInfo::availablePortInfos();
        if(m_availablePortsList.size() > 0)
        {
            fieldbuf[0] = const_cast<char*>(m_availablePortsList[0].portName.c_str());
        }
        else
        {
            fieldbuf[0] = const_cast<char*>(m_portName.c_str());
        }
        
    }
    else
    {
        fieldbuf[0] = const_cast<char*>(m_portName.c_str());
    }
    
    fieldbuf[1] = const_cast<char*>(m_BaudRate.c_str());
    fieldbuf[2] = const_cast<char*>(m_Parity.c_str());
    fieldbuf[3] = const_cast<char*>(m_DataBit.c_str());
    fieldbuf[4] = const_cast<char*>(m_StopBit.c_str());

    if (getstrings(fieldname, fieldbuf, field) != KEY_ESC)
    {
        m_portName = std::string(fieldbuf[0]);
        m_BaudRate = std::string(fieldbuf[1]);
        m_Parity = std::string(fieldbuf[2]);
        m_DataBit = std::string(fieldbuf[3]);
        m_StopBit = std::string(fieldbuf[4]);
        
        result = result.append(m_portName)
                    .append(",").append(m_BaudRate)
                    .append(",").append(m_Parity)
                    .append(",").append(m_DataBit)
                    .append(",").append(m_StopBit);
    }

    // if(fieldbuf)
    // {
    //     delete [] fieldbuf;
    // }
    
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
    int i, field = 50;

    for (i = 0; i < 1; i++)
    {
    	fieldbuf[i] = new char[field + 1];
    }
    
    fieldbuf[0] = const_cast<char*>(m_sendData.c_str());

    if (getstrings(fieldname, fieldbuf, field) != KEY_ESC)
    {
        m_sendData = std::string(fieldbuf[0]);
        result = m_sendData;
    }
    
    // if(fieldbuf)
    // {
    //     delete [] fieldbuf;
    // }

    return result;
}

/***************************** utils functions ***********************/

char * HexStrFilter(char *charStr, char *fiterStr)
{
    if (charStr == NULL || fiterStr == NULL)
    {
        return NULL;
    }

    int len = strlen(charStr);

    char *pTemp = fiterStr;

    for(int i = 0; i < len; i++)
    {
        if( ((charStr[i] >= '0') && (charStr[i] <= '9')) ||
            ((charStr[i] >= 'A') && (charStr[i] <= 'F')) ||
            ((charStr[i] >= 'a') && (charStr[i] <= 'f')) )
        {
            *fiterStr++ = charStr[i];
        }
    }

    return pTemp;
}

char CombineHexChar(char charH,char charL) /// CombineHexChar(A,B) result=1011;
{
    char result;

    if(charH >= '0' && charH <= '9')
    {
        result = (charH - '0');
    }
    else if(charH >= 'a' && charH <= 'f')
    {
        result = (charH - 'a'+10);
    }
    else if(charH >= 'A' && charH <= 'F')
    {
        result = (charH - 'A' + 10);
    }
    else
    {
        result = 0;/// need to fiter non-hex character
    }

    result <<= 4;

    if(charL >= '0' && charL <= '9')
    {
        result += (charL - '0');
    }
    else if(charL >= 'a' && charL <= 'f')
    {
        result += (charL - 'a'+10);
    }
    else if(charL >= 'A' && charL <= 'F')
    {
        result += (charL - 'A' + 10);
    }
    else
    {
        result += 0;
    }

    return result;
}

int Char2Hex(char *charStr,char *hexStr)/// character to hex, return value is hexStr length
{
    if (charStr == NULL || hexStr == NULL)
    {
        return 0;
    }

    int hexStrCount = 0;

    char *fiterStr = NULL;
    fiterStr = new char[strlen(charStr) + 1];
    memset(fiterStr,0,strlen(charStr) + 1);

    HexStrFilter(charStr,fiterStr);///filter non-hex character

    int len = strlen(fiterStr);

    // warn: if charStr length not even, the last charactor will lost
    for(int i=0;i < len/2; i++)
    {
        *hexStr++ = CombineHexChar(fiterStr[i*2],fiterStr[i*2+1]);
        hexStrCount++;
    }

    if(fiterStr)
    {
        delete [] fiterStr;
        fiterStr = NULL;
    }

    return hexStrCount;
}

int getParityIndex(std::string parity)
{
	// std::string ParityArray[] = {"None", "Odd", "Even", "Mark", "Space" };
	int iRet = 0;
	
	if( "None" == parity )
	{
		// iRet = 0;
	}
	else if( "Odd" == parity )
	{
		iRet = 1;
	}
	else if( "Even" == parity )
	{
		iRet = 2;
	}
	else if( "Mark" == parity )
	{
		iRet = 3;
	}
	else if( "Space" == parity )
	{
		iRet = 4;
	}
	else
	{
		// iRet = 0;
	}
	
	return iRet;
}

int getStopBitIndex(std::string parity)
{
	// std::string StopArray[] = {"1", "1.5", "2"};
	
	int iRet = 0;
	
	if( "1" == parity )
	{
		// iRet = 0;
	}
	else if( "1.5" == parity )
	{
		iRet = 1;
	}
	else if( "2" == parity )
	{
		iRet = 2;
	}
	else
	{
		// iRet = 0;
	}
	
	return iRet;
}
/***************************** forward declarations ***********************/
// menu level 1
void subSetting(void), subSend(void), subReceive(void),subHelp(void);
// menu level 2
void portSetting(void), open(void), close(void),getCommStatus(void),getAvailablePort(void);
void send(void), sendHex(void);
void clearReceive(void),receiveShowHex(void),receiveShowChar(void);
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
    { "AvailablePort", getAvailablePort, "get Available Port Info" },
    { "PortSetting", portSetting, "set port name(COM1 or /dev/ttyS0), baudrate(9600), parity(None), databit(8), stopbit(1)" },
    { "Open", open, "open serial port" },
    { "Close", close, "close serial port" },
    { "CommStatus", getCommStatus, "get Comm Status,such as [ opened ] - /dev/ttyS0,9600,None,8,1" },
    //{ "Exit", DoExit, "Exit Appliction" },
    { "", (FUNC)0, "" }
};

menu SubMenuSend[] =
{
    { "SendChar", send, "send char data to serial port" },
    { "SendHex", sendHex, "send Hex data to serial port" },
    { "", (FUNC)0, "" }
};

menu SubMenuReceive[] =
{
    { "ClearReceive", clearReceive, "clear receive" },
    { "ReceiveShowChar", receiveShowChar, "receive show char, default option" },
    { "ReceiveShowHex", receiveShowHex, "receive show hex" },
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

void getAvailablePort(void)
{
    char str[1024];
    std::string portInfo;

    m_availablePortsList = CSerialPortInfo::availablePortInfos();

    int availablePortCount = m_availablePortsList.size();

    snprintf(str,sizeof(str),"availableFriendlyPorts : ");

    for (int i = 0; i < availablePortCount; i++)
    {
        portInfo.append(m_availablePortsList[i].portName)
                .append(" ")
                .append(m_availablePortsList[i].description)
                .append("\n");
    }

    if (m_availablePortsList.size() != 0)
    {
        snprintf(str,sizeof(str),"\navailableFriendlyPorts : \n%s",portInfo.c_str());
    }
    else
    {
        snprintf(str,sizeof(str),"\navailableFriendlyPorts : No valid port\n");
    }

    bodymsg(str);
}

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

    m_serialPort.init(m_portName,//windows:COM1 Linux:/dev/ttyS0
                      std::atoi(m_BaudRate.c_str()),
					  itas109::Parity(getParityIndex(m_Parity)),
                      itas109::DataBits(std::atoi(m_DataBit.c_str())),
                      itas109::StopBits(getStopBitIndex(m_StopBit))
					  );

	m_serialPort.open();
	
	if(m_serialPort.isOpened())
	{
        char str[256];
        snprintf(str,sizeof(str),"open success. %s,%d,%s,%d,%s\n",
                const_cast<char*>(m_serialPort.getPortName().c_str()),
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
    snprintf(str,sizeof(str),"[ %s ] - %s,%d,%s,%d,%s",
             m_serialPort.isOpened()?"opened":"closed",
             const_cast<char*>(m_serialPort.getPortName().c_str()),
             m_serialPort.getBaudRate(),
             const_cast<char*>(ParityArray[(int)(m_serialPort.getParity())].c_str()),
             (int)(m_serialPort.getDataBits()),
             const_cast<char*>(StopArray[(int)(m_serialPort.getStopBits())].c_str()));

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

void sendHex(void)
{
    if(m_serialPort.isOpened())
    {
        std::string str = getSendData();
        char* charStr = const_cast<char*>(str.c_str());
        int len = strlen(charStr);
        char *hexStr = NULL;
        hexStr = new char[len + 1];
        memset(hexStr,0,len + 1);

        int hexStrLen = 0;
        hexStrLen = Char2Hex(charStr,hexStr);

        m_serialPort.writeData(hexStr,hexStrLen);

        if(hexStr)
        {
            delete [] hexStr;
            hexStr = NULL;
        }
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

void receiveShowChar(void)
{
    isShowHex = false;
    infomsg("set receive show char");
}

void receiveShowHex(void)
{
    isShowHex = true;
    infomsg("set receive show hex");
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
