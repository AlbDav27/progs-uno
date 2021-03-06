#include <stdio.h>
#include <fcntl.h>       /* File Control Definitions           */
#include <termios.h>     /* POSIX Terminal Control Definitions */
#include <unistd.h>      /* UNIX Standard Definitions 	       */ 
#include <errno.h>       /* ERROR Number Definitions           */
#include <sys/ioctl.h>   /* ioctl()                            */
#include <string.h>
void main(void)
{
        	int fd;/*File Descriptor*/
		
		printf("\n +----------------------------------+");
		printf("\n |        USB To RS485 Write        |");
		printf("\n +----------------------------------+");

		/*------------------------------- Opening the Serial Port -------------------------------*/

		/* Change /dev/ttyUSB0 to the one corresponding to your system */

        	fd = open("/dev/ttyS0",O_RDWR | O_NOCTTY);	/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
			   						/* O_RDWR Read/Write access to serial port           */
									/* O_NOCTTY - No terminal will control the process   */
									                                
									
        	if(fd == -1)						/* Error Checking */
            	   printf("\n  Error! in Opening ttyUSB0  ");
        	else
            	   printf("\n  ttyUSB0 Opened Successfully ");

	
		/*---------- Setting the Attributes of the serial port using termios structure --------- */
		
		struct termios SerialPortSettings;	/* Create the structure                          */

		tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */

		cfsetispeed(&SerialPortSettings,B9600); /* Set Read  Speed as 9600                       */
		cfsetospeed(&SerialPortSettings,B9600); /* Set Write Speed as 9600                       */

		SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
		SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
		SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
		SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */
	
		SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
		SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */ 
		
		
		SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
		SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */

		SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/

        SerialPortSettings.c_cc[VMIN] =  1; 		//pure time read 
		SerialPortSettings.c_cc[VTIME] = 0;  /* Wait 1s   */	//VTIME TIEMPO EN DECIMAS DE SEGUNDO

		if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
		    printf("\n  ERROR ! in Setting attributes");
		else
                    printf("\n  BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none");

		

	        /*------------------------------- Write data to serial port -----------------------------*/

		char write_buffer[7];	/* Buffer containing characters to write into port	     */	
		char readb[9];
		strcpy(write_buffer, "$016");
		write_buffer[4]=13;
		printf("\n\n envio a ADAM4053 : %s \n", write_buffer);
		
		int  bytes_written  = 0;  	/* Value for storing the number of bytes written to the port */
		int bytes_read=0; 

		bytes_written = write(fd,write_buffer, 5);	
		bytes_read = read (fd, readb,8);
		printf("\n Recibo de ADAM4053 : %s\n", readb);
		close(fd);/* Close the Serial port */

}

