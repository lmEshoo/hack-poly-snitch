#include "SerialPort.h"

/*
Port Naming Reference :
Do not use the following reserved device names for the name of a file:
CON, PRN, AUX, NUL, COM1, COM2, COM3, COM4, COM5, COM6, COM7, COM8, 
COM9, LPT1, LPT2, LPT3, LPT4, LPT5, LPT6, LPT7, LPT8, and LPT9
"\\\\.\\COM1" Also works
Baud Rate Could be :
	300 600 1200 2400 4800 9600 14400 19200 38400 57600 115200
*/

// Constructor of _Serial class for initialize
_Serial::_Serial() {
	this->fLog = fopen("ComLog.txt" , "w" );
	this->CurrentPort = "COM7";
	this->CurrentBaudRate = 115200;
	this->bConnected = this->SetComPort(this->CurrentPort,this->CurrentBaudRate);
	}

// _Serial class destructor for quit
_Serial::~_Serial() {
	// Close file ComLog
	if( this->fLog ) fclose( this->fLog );
	// Close the serial handler
	if( this->hSerial ) CloseHandle(this->hSerial); 
	}
// _Serial class destructor for quit END

// Com Port Configuration
bool _Serial::SetComPort(char *portName , int BaudRate) {
	this->bConnected = 0 ;  
	// Update current Com port & Baud rate
	this->CurrentPort = portName ;
	this->CurrentBaudRate = BaudRate;
	fprintf(this->fLog,"Set to COM Port : %s , Baud Rate : %6d \n",portName , BaudRate );
	// Close the serial handler
	if( this->hSerial ) CloseHandle(this->hSerial); 
	//Try to connect to the given port throuh CreateFile
	this->hSerial = CreateFile(
			portName,
			GENERIC_READ | GENERIC_WRITE,
			0, // ShareMode OFF. Prevent from been access by other process
			NULL, // the handle returned by CreateFile cannot be inherited by any child processes
			OPEN_EXISTING, // Opens a file or device, only if it exists.
			FILE_ATTRIBUTE_NORMAL, // The file does not have other attributes set.
			NULL); // When opening an existing file, CreateFile ignores this parameter.
	//Check if the connection was successfull
	if(this->hSerial==INVALID_HANDLE_VALUE) {
		//If got an error flag , display it
		if(GetLastError()==ERROR_FILE_NOT_FOUND) {
			//Print Error if neccessary
			fprintf(this->fLog,"COM Port : %s is not available\n",portName);
			}
		else fprintf(this->fLog,"Invalid Serial Handle\n");
		return 0; // Return if fail to get serial handle
		}
	switch(BaudRate) {
		case 300: strcpy( this->DCBSetting , "300,n,8,1"); break;
		case 600: strcpy( this->DCBSetting , "600,n,8,1"); break;
		case 1200: strcpy( this->DCBSetting , "1200,n,8,1"); break;
		case 2400: strcpy( this->DCBSetting , "2400,n,8,1"); break;
		case 4800: strcpy( this->DCBSetting , "4800,n,8,1"); break;
		case 9600: strcpy( this->DCBSetting , "9600,n,8,1"); break;
		case 14400: strcpy( this->DCBSetting , "14400,n,8,1"); break;
		case 19200: strcpy( this->DCBSetting , "19200,n,8,1"); break;
		case 38400: strcpy( this->DCBSetting , "38400,n,8,1"); break;
		case 57600: strcpy( this->DCBSetting , "57600,n,8,1"); break;
		case 115200: strcpy( this->DCBSetting , "115200,n,8,1"); break;
		default:
			fprintf(this->fLog,"Use Default Baud Rate : 9600\n");
			strcpy( this->DCBSetting , "9600,n,8,1");
			break;
		}
	// Initialize DCB Device Control Block
	FillMemory(&this->dcb, sizeof(this->dcb), 0);
	// DCBlength should be set before calling SetCommState to update the settings
	this->dcb.DCBlength = sizeof(this->dcb); 
	// Initialize DCB
/*	// (1) Another way to initialize DCB.  Not necessary in here.
	//if(!GetCommState( this->hSerial , &this->dcb ) ) {
	//	fprintf(this->fLog,"Unable to GetCommState.   \n");
	//	}
*/
	// (2) Build comm DCB after GetCommState
	if (!BuildCommDCB( this->DCBSetting , &this->dcb)) {   
		fprintf(this->fLog,"COM Port : %s is not available\n",portName);
		return 0; 
		}

	// Update DCB Setting
	if(!SetCommState( this->hSerial , &this->dcb ) ) {
		fprintf(this->fLog,"Unable To SetCommState.  Update DCB Fail.\n");
		return 0; 
		} 
	// Call GetCommTimeouts() before SetCommTimeouts()
	if( !GetCommTimeouts( this->hSerial , &this->ComTimeOuts) ) {
		fprintf(this->fLog,"GetCommTimeouts() Fail.\n");
		return 0; 
		}
	// ReadTotalTimeout = ReadTotalTimeOutMultiplier*BytesToRead + ReadTotalTimeoutConstant 
	// Set Read complete immediately without waiting for any new data to arrive
	this->ComTimeOuts.ReadIntervalTimeout         = MAXDWORD;
	this->ComTimeOuts.ReadTotalTimeoutMultiplier  = 0;
	this->ComTimeOuts.ReadTotalTimeoutConstant    = 0;	
	this->ComTimeOuts.WriteTotalTimeoutMultiplier = 0;
	this->ComTimeOuts.WriteTotalTimeoutConstant   = 0;
	// Set Com Port Read/Write Timeout to Prevent Program Stuck
	if(!SetCommTimeouts( this->hSerial , &this->ComTimeOuts)) {
		fprintf(this->fLog,"SetCommTimeouts() Fail.\n");
		return 0; 
		}
	// fprintf(this->fLog,"COM Port : %s With Baud Rate %d\n",portName,BaudRate);
	this->bConnected = 1 ;  
	return 1; // Set Com Port Done!
	}
// _Serial::SetComPort(char *portName , int BaudRate); END


/*	
********************************************************************************
Coding Reference : Change Communication Setting
		// These can Update DCB rate. (The setting is for Arduino when not
		// using the default rx tx pin , called software serial setting.)
		dcb.BaudRate = CBR_9600 ;
		dcb.ByteSize = 8 ;
		dcb.StopBits = ONESTOPBIT ;
		dcb.Parity = NOPARITY;
		if (!SetCommState(hComm, &dcb)) {
			// Error in SetCommState. Possibly a problem with the communications 
			// port handle or a problem with the DCB structure itself.
            fprintf(this->fLog,"SetCommState( ) Fail\n");
			}
Coding Reference : Purge The Communication Data In Buffer
	BOOL PurgeComm( 
	HANDLE hFile, // handle of communications resource 
	DWORD dwFlags // action to perform 
	); 
	dwFlags :
    	- PURGE_TXABORT: Abort Tx Action 
    	- PURGE_RXABORT: Abort Rx Action
    	- PURGE_TXCLEAR: flush Tx Buffer
    	- PURGE_TXCLEAR: flush Rx Buffer
********************************************************************************
*/



// Read Data from com port
DWORD _Serial::ReadData( unsigned char *DataToRead , DWORD NoOfBytesToRead) {
	if( NoOfBytesToRead > MaxReadBytes ) NoOfBytesToRead = MaxReadBytes ;
    //Use the ClearCommError function to get status info on the Serial port
    ClearCommError(this->hSerial, &this->Error, &this->ComStatus);
    //Check if there is something to read
    if(this->ComStatus.cbInQue > 0 ) {
        //Read data and return the bytes number of data
        if( ReadFile(this->hSerial, DataToRead , NoOfBytesToRead , &this->NoOfBytesRead , NULL) ) {
			//fprintf(this->fLog,"Got Data : \n%s\n" , DataToRead );
			return this->NoOfBytesRead ;
			}
		}
	//Nothing to read
	//fprintf(this->fLog,"No Incoming Data\n");
	return 0;
	}

// Read a Byte from com port
unsigned char _Serial::ReadByte( void ) {
	this->rxByte = 0;
	ReadFile(this->hSerial, &this->rxByte , 1 , &this->NoOfBytesRead , NULL) ;
	//fprintf(this->fLog,"%3d:%c\n" , ByteToRead , ByteToRead );
	return this->rxByte ;
	}

// Write Data to Com port
bool _Serial::WriteData(unsigned char *DataToWrite, DWORD NoOfBytesToWrite) {
	//Write the data on the Serial port
	if(!WriteFile(this->hSerial, DataToWrite , NoOfBytesToWrite , &this->NoOfBytesWritten, NULL )) {
		//Fail to write , get comm error mesage and return false
		ClearCommError(this->hSerial, &this->Error, &this->ComStatus);
		fprintf(this->fLog,"Data Written Error\n");
		return false;
		}
return true;
}
// Write Data to Com port (Overloading)
bool _Serial::WriteData(char *DataToWrite, DWORD NoOfBytesToWrite) {
	//Write the data on the Serial port
	if(!WriteFile(this->hSerial, DataToWrite , NoOfBytesToWrite , &this->NoOfBytesWritten, NULL )) {
		//Fail to write , get comm error mesage and return false
		ClearCommError(this->hSerial, &this->Error, &this->ComStatus);
		fprintf(this->fLog,"Data Written Error\n");
		return false;
		}
//fprintf(this->fLog,"Data Written\n");
return true;
}

// Write a byte to Com port
bool _Serial::WriteByte(unsigned char ByteToWrite) {
	//Write the data on the Serial port
	if(!WriteFile(this->hSerial, &ByteToWrite , 1 , &this->NoOfBytesWritten, NULL )) {
		//Fail to write , get comm error mesage and return false
		ClearCommError(this->hSerial, &this->Error, &this->ComStatus);
		fprintf(this->fLog,"Byte Written Fail\n");
		return false;
		}
return true;
}

// Write a string to Com Port
void _Serial::WriteString(const char *text) {
  while(*text != 0) WriteByte( *(text++) );
}


// Return Current Bytes In Queue
DWORD _Serial::BytesInQue( void ) {
    ClearCommError(this->hSerial, &this->Error, &this->ComStatus);
	return this->ComStatus.cbInQue;
	}
