/*
|******** SerialPort *********|

This program is for opening and communicating with a serial port.

Function :
	- Open a com port and set the Baud Rate for communication
	- Read / Write data to a com port
	- Communication log for debug

PS:	If found any bug , please also email me the solution.

bdragon
2009.Sep.6
bdragong@gmail.com

Revised on 2010.Feb.2 , 2010.Feb.4
|*****************************************************************|
Reference :
	- Serial Communications in Win32
		http://msdn.microsoft.com/en-us/library/ms810467.aspx
	- CreateFile Function
		http://msdn.microsoft.com/en-us/library/aa363858%28VS.85%29.aspx
	- Creating and Opening Files
		http://msdn.microsoft.com/en-us/library/aa363874%28VS.85%29.aspx
	- ReadFile Function
		http://msdn.microsoft.com/en-us/library/aa365467%28VS.85%29.aspx
	- WriteFile Function
		http://msdn.microsoft.com/en-us/library/aa365747%28VS.85%29.aspx
	- RS232(For Windows & Linux) by Teunis van Beelen
	- Arduino and C++ (for Windows)
		http://www.arduino.cc/playground/Interfacing/CPPWindows
*/

#ifndef SERIALPORT_H
#define SERIALPORT_H

#define MaxReadBytes 256 // Limit the maximum number of bytes to be read
#define MaxWriteBytes 64 // Limit the maximum number of bytes to be read

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

class _Serial {
	private:
		HANDLE hSerial; // Serial Communication Handler
		COMSTAT ComStatus; // Get various information about the connection
		COMMTIMEOUTS ComTimeOuts; // For Setting Communication TimeOut
		DWORD Error; //Keep track of last error
		DCB dcb; // Device Control Block
		DWORD NoOfBytesRead; // Internal Usage For Read
		DWORD NoOfBytesWritten; // Internal Usage For Write
		char DCBSetting[16]; // For DCB Configuration 9600,n,8,1
	public:
		FILE *fLog ; // Open A Log file
		_Serial(); // Constructor
		~_Serial(); // Release Serial Handle
		// Com Port Configuration
		bool SetComPort(char *portName , int BaudRate);
		bool bConnected ; // True if made a successful connection
		char *CurrentPort ; // Current Com port
		int CurrentBaudRate; // Current Baud Rate 
		unsigned char rxData[MaxReadBytes]; // Received Data
		unsigned char rxByte; // Received Byte
		char txData[MaxWriteBytes]; // Data For Written
		// Read Data from com port , return NoOfBytesRead ;
		DWORD ReadData( unsigned char *DataToRead , DWORD NoOfBytesToRead);
		// Read a byte from com port return a byte when got one
		unsigned char ReadByte( void );
        // Write data to com port
        bool WriteData(unsigned char *DataToWrite, DWORD NoOfBytesToWrite);
        // Overloading WriteData( )
        bool WriteData(char *DataToWrite, DWORD NoOfBytesToWrite);
		// Write a byte to com port
		bool WriteByte(unsigned char ByteToWrite);
		// Write a constant string to Com Port
		void WriteString(const char *text);
		// Check Current BytesInQue
		DWORD BytesInQue( void );
	};

#endif 
// SERIALPORT_H END
