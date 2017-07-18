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
		

		//IO_Blocking(fd,FALSE);
		/*
		COMMTIMEOUTS timeouts = { 0 };

		timeouts.ReadIntervalTimeout         = 50;
		timeouts.ReadTotalTimeoutConstant    = 50;
		timeouts.ReadTotalTimeoutMultiplier  = 10;
		timeouts.WriteTotalTimeoutConstant   = 50;
		timeouts.WriteTotalTimeoutMultiplier = 10;		

		if (SetCommTimeouts(hComm, &timeouts) == FALSE)
			printf("\n   Error! in Setting Time Outs");
		else
			printf("\n\n   Setting Serial Port Timeouts Successfull");	*/

		
		/*
		if (Status == TRUE)
				printf("\n\n    DE of MAX485 is Low (Receive Mode)");
		else
				printf("\n\n   Error! in EscapeCommFunction(hComm, SETDTR)");

		Status = EscapeCommFunction(hComm, SETRTS);//SETRTS will make FT232 RTS-LOW(inverted),~RE-LOW for Reception

		if (Status == TRUE)
				printf("\n   ~RE of MAX485 is Low (Receive Mode)");
		else
				printf("\n   Error! in EscapeCommFunction(hComm, SETRTS)");

			/*------------------------------------ Setting WaitComm() Event   ----------------------------------------*/	
		Status = SetCommMask(hComm, EV_RXCHAR);	
		printf("\n\n    Waiting for Data Reception");

		//tcflush(fd,TCIFLUSH);
		char read_buffer[5];   /* Buffer to store the data received
		             */
		//char reead_buff[6];
		int  bytes_read = 0;    /* Number of bytes read by the read() system call */
		int i =0;		//float resp;
		//char write_buffer[5];
		int  bytes_written  = 0;
		char r[10];
		strcpy(read_buffer,"");
		char c[10];
		strcpy(c,"");
		char write_buffer[6];
		strcpy(write_buffer, "");
		
		

		sleep(2);
		FlushFileBuffers(hComm);


		
		
		
		while (1)
		{
			FlushFileBuffers(hComm);
			Status = WaitCommEvent(hComm, &dwEventMask, NULL);
			if (Status == FALSE)
			{
				printf("\n    Error! in Setting WaitCommEvent()");
			}
			else //If  WaitCommEvent()==True Read the RXed data using ReadFile();
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
			printf("\n  Bytes Rxed -%d", bytes_read); /* Print the number of bytes read */
			printf("\n ");
			for (i=5; i<8;i++)
					read_buffer[i]='\0';

			printf("\n%s", read_buffer);
			printf("\n +----------------------------------+\n\n\n");
			

			
			if (read_buffer[0]=='$'){		
				
				
				
				strcpy(c,"");
				strcpy(r,"");
	
				strcpy(r,"okf");
				
							
				c[0]= read_buffer[3];
				c[1]= '*';
				//printf ("\n c tiene:%s/\n", c);
				for (i=2; i<8;i++)
					c[i]='\0';
							
				strcpy(r,"okf");
				strcat(r,c);
				printf ("\n Respuesta : %s/fincadena\n", r);
				strcpy(write_buffer,r);
				//dNoOFBytestoWrite = sizeof(r); // Calculating the no of bytes to write into the port

				/*Status = WriteFile(hComm,               // Handle to the Serialport
						   write_buffer,            // Data to be written to the port 
						    dNoOFBytestoWrite,   // No of bytes to write into the port
						    &dNoOfBytesWritten,  // No of bytes written to the port
						   NULL);
*/
			}else{
				
				
				strcpy(c,"");
				c[0]= read_buffer[2];
				c[1]= read_buffer[3];
				//printf("\nc= %s/", c);
				strcpy(r,"");
				strcpy(r,"okc");
				strcat(r,c);
				printf("\nr= %s", r);
				strcpy(write_buffer,r);



				//dNoOFBytestoWrite = sizeof(r); // Calculating the no of bytes to write into the port

				/*Status = WriteFile(hComm,               // Handle to the Serialport
						   write_buffer,            // Data to be written to the port 
						    dNoOFBytestoWrite,   // No of bytes to write into the port
						    &dNoOfBytesWritten,  // No of bytes written to the port
						   NULL);*/



				printf ("\n Respuesta : %s/fincadena\n", r);
			}

			FlushFileBuffers(hComm);
			Sleep(400);

			dNoOFBytestoWrite = sizeof(r); // Calculating the no of bytes to write into the port

			/*Status = WriteFile(hComm,               // Handle to the Serialport
					   		r,            // Data to be written to the port 
					   		6,   // No of bytes to write into the port
					   		&dNoOfBytesWritten,  // No of bytes written to the port
					   		NULL);
*/

			
			bytes_read=0;
			strcpy(read_buffer,"");
			strcpy(write_buffer,"");
			strcpy(r,"");
			//Sleep(2);
			Sleep(100);

			//memset(read_buffer, '\0', 32);
			//memset(write_buffer, '\0', 10);

			
		/* Close the Serial port */
		}
		CloseHandle(hComm);
}


		
