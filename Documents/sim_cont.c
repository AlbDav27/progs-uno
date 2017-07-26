#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
void main(void)
{
		
		HANDLE hComm;                          // Handle to the Serial port
		char   ComPortName[] = "\\\\.\\COM5"; // Name of the Serial port(May Change) to be opened,
		BOOL   Status;
		//char   lpBuffer[] = "A";		       // lpBuffer should be  char or byte array, otherwise write wil fail
		DWORD  dNoOFBytestoWrite;              // No of bytes to write into the port
		DWORD  dNoOfBytesWritten = 0;          // No of bytes written to the port
		DWORD dwEventMask;
		DWORD NoBytesRecieved;
		char  TempChar;

		printf("\n\n +==========================================+");
		printf("\n |  RS485 Mode - Transmission (Win32 API)   |");
		printf("\n +==========================================+\n");
		/*----------------------------------- Opening the Serial Port --------------------------------------------*/

		hComm = CreateFile( ComPortName,                       // Name of the Port to be Opened
							GENERIC_READ | GENERIC_WRITE,      // Read/Write Access
							0,                                 // No Sharing, ports cant be shared
							NULL,                              // No Security
							OPEN_EXISTING,                     // Open existing port only
							FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH,                                 // Non Overlapped I/O
							NULL);                             // Null for Comm Devices

		if (hComm == INVALID_HANDLE_VALUE)
			printf("\n   Error! - Port %s can't be opened", ComPortName);
		else 
			printf("\n   Port %s Opened\n ", ComPortName);

		
		/*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/

		DCB dcbSerialParams = { 0 };                        // Initializing DCB structure
		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
		
		Status = GetCommState(hComm, &dcbSerialParams);     //retreives  the current settings

		if (Status == FALSE)
			printf("\n   Error! in GetCommState()");

		dcbSerialParams.BaudRate = CBR_9600;      // Setting BaudRate = 9600
		dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
		dcbSerialParams.StopBits = 1;		//ONESTOPBIT;    // Setting StopBits = 1
		dcbSerialParams.Parity   = NOPARITY;      // Setting Parity = None 

		Status = SetCommState(hComm, &dcbSerialParams);  //Configuring the port according to settings in DCB 

		if (Status == FALSE)
		{
			printf("\n   Error! in Setting DCB Structure");
		}
		else
		{
			printf("\n   Setting DCB Structure Successfull\n");
			printf("\n       Baudrate = %d", dcbSerialParams.BaudRate);
			printf("\n       ByteSize = %d", dcbSerialParams.ByteSize);
			printf("\n       StopBits = %d", dcbSerialParams.StopBits);
			printf("\n       Parity   = %d", dcbSerialParams.Parity);
		}
		
	
		Status = SetCommMask(hComm, EV_RXCHAR);	
		printf("\n\n    Waiting for Data Reception");
		/*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/

		//////////////////////////////////////declaración de variables/////////////////////////////////////////////
		char read_buffer[5];
		char r[32]; 
		char c[10]; 
		char write_buffer[6]; 
		char in[32];

		int  bytes_read = 0;    
		int i =0;		
		int  bytes_written  = 0;
		
		//////////////////////////////////////////inicialización de variables////////////////////////////////////////
		strcpy(read_buffer,"");
		strcpy(c,"");
		strcpy(write_buffer, "");
		int y=0;
		int x=0;

		Sleep(2000);			////////////////////////este retardo es mucho menor que el de UNO-1252G para asegurar que este dispositivo lea antes de que UNO envie
		
		while (1)
		{
			PurgeComm(hComm ,PURGE_RXCLEAR && PURGE_TXCLEAR);
			Status = WaitCommEvent(hComm, &dwEventMask, NULL);
			if (Status == FALSE)
			{
				printf("\n    Error! in Setting WaitCommEvent()");
			}
			else 						//If  WaitCommEvent()==True Read the RXed data using ReadFile();
			{
				printf("\n\n    Characters Received");
									//printf("\n ReadFile();");
				
             	Status = ReadFile( hComm,
                                read_buffer,
             					sizeof(read_buffer),
                   				&NoBytesRecieved,
                   				NULL);
			}
			bytes_read=sizeof(read_buffer);
			printf("\n  Bytes Rxed : %d", bytes_read); /* Print the number of bytes read */
			printf("\n ");
			for (i=5; i<8;i++)
					read_buffer[i]='\0';

			printf("\n%s", read_buffer);
			printf("\n +----------------------------------+\n\n\n");

			if (read_buffer[0]=='$'){				//este if simula el comportamiento del mux
				strcpy(c,"");
				strcpy(r,"");
				strcpy(r,"okf");
				c[0]= read_buffer[3];
				c[1]= '*';
				for (i=2; i<8;i++)
					c[i]='\0';
		
				strcpy(r,"okf");
				strcat(r,c);
			}else{									//este if sumula el comportamiento de los controladores
				strcpy(c,"");
				c[0]= read_buffer[2];
				c[1]= read_buffer[3];
				strcpy(r,"");
				strcpy(r,"okc");
				strcat(r,c);
			}
			strcat(r,"123estacadenatiene32caracte");
			printf ("\n Respuesta : %s/fincadena\n", r);
			PurgeComm(hComm ,PURGE_RXCLEAR && PURGE_TXCLEAR);
			Sleep(400);
			if (x==0){
				Status = WriteFile(hComm,               	// Handle to the Serialport
					   		r,          				 // Data to be written to the port 
					   		32,			   				// No of bytes to write into the port
					   		&dNoOfBytesWritten,  		// No of bytes written to the port
					   		NULL);
				x=1;
				printf("\ncambio de x=0 a x=1");
			}
			
			y++;
			Sleep(50);
			Status = WriteFile(hComm,               	// Handle to the Serialport
					   		r,          				 // Data to be written to the port 
					   		32,			   				// No of bytes to write into the port
					   		&dNoOfBytesWritten,  		// No of bytes written to the port
					   		NULL);
				
			y==0;
			
			bytes_read=0;
			strcpy(read_buffer,"");
			strcpy(write_buffer,"");
			strcpy(r,"");
			printf ("\n acabe un ciclo");
			PurgeComm(hComm ,PURGE_RXCLEAR && PURGE_TXCLEAR);
			Sleep(50);
		}
		CloseHandle(hComm);			/* Close the Serial port */
}
