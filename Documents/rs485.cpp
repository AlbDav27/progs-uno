
#include <mosquittopp.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>             /* File Control Definitions           */

#include <termios.h>      /* POSIX Terminal Control Definitions */

#include <unistd.h>         /* UNIX Standard Definitions            */

#include <errno.h>           /* ERROR Number Definitions           */

#include <sys/ioctl.h>   /* ioctl() */

#include <unistd.h> 	//para funcion sleep()  ...delay
#include <string.h>

int main(void)
{

	int fd;/*File Descriptor*/

        printf("\n +----------------------------------+");

        printf("\n |       USB To RS485 Read          |");
	
	printf("\n +----------------------------------+");

 
/*------------------------------- Opening the Serial Port -------------------------------*/

/* Change /dev/ttyUSB0 to the one corresponding to your system */

	fd = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY);           /* ttyUSB0 is the FT232 based USB2SERIAL Converter   */  
								//dependiendo el puerto debe de modificarse ttyUSB0 al puerto serial que se va a ocupar

                                                               /* O_RDWR   - Read/Write access to serial port       */

                                                              /* O_NOCTTY - No terminal will control the process   */

                                                              /* Open in blocking mode,read will wait              */

                                                                                                                                               
	if(fd == -1)                                                                                           /* Error Checking */

		printf("\n  Error! in Opening ttyUSB0  ");

	else

		printf("\n  ttyUSB0 Opened Successfully ");

 

               
/*---------- Setting the Attributes of the serial port using termios structure --------- */

                               
	struct termios SerialPortSettings;              /* Create the structure                          */

 
	tcgetattr(fd, &SerialPortSettings);            /* Get the current attributes of the Serial port */

 
	/* Setting the Baud rate */

        cfsetispeed(&SerialPortSettings,B9600); /* Set Read  Speed as 9600                       */

        cfsetospeed(&SerialPortSettings,B9600); /* Set Write Speed as 9600                       */

	/* 8N1 Mode */

        SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */

        SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */

        SerialPortSettings.c_cflag &= ~CSIZE;      /* Clears the mask for setting the data size             */

        SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */

        SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */

        SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */

        SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */

	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */

	SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/

	/* Setting Time outs */

	SerialPortSettings.c_cc[VMIN] =  10; /* Read at least 10 character */

        SerialPortSettings.c_cc[VTIME] = 0;  /* Wait indefinetly   */

	if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
		printf("\n  ERROR ! in Setting attributes");

        else
		printf("\n  BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none");

 
	int RTS_flag,DTR_flag;

	RTS_flag = TIOCM_RTS; /* Modem Constant for RTS pin */

        DTR_flag = TIOCM_DTR; /* Modem Constant for DTR pin */

 
	ioctl(fd,TIOCMBIS,&RTS_flag);/* ~RTS = 0,So ~RE pin of MAX485 is LOW,Receive Mode enabled   */

        ioctl(fd,TIOCMBIS,&DTR_flag);/* ~DTR = 0,So  DE pin of MAX485 is LOW,                       */

 

	char read_buffer[32];   /* Buffer to store the data received              */

       	int  bytes_read = 0;    /* Number of bytes read by the read() system call */

        int i =0;
	char com [100];
	char fin [2];
	strcpy (com,"mosquitto_pub -h iot.eclipse.org -t testkd -m  'recibo por rs485:");
	strcpy (read_buffer,"no_lei_nada");
	strcpy (fin," '");
	int n= 1;
	sleep(2);
	tcflush(fd, TCIFLUSH);   /* Discards old data in the rx buffer            */
	char write_buffer[2];
                    

/*------------------------------- Read data from serial port -----------------------------*/ 
//Este es el bucle que se va a repetir para estar recibiendo constantemente información

 

// se debe de verificar si linux permite que se ejecute indefinidamente el programa, o hay que ejecutar periodicamente el mismo

	while (1)
	{		
		write_buffer[0] = n;	/* Buffer containing characters to write into port	    */	
		int  bytes_written  = 0;  	/* Value for storing the number of bytes written to the port */ 
		bytes_written = write(fd,write_buffer,sizeof(write_buffer));/* use write() to send data to port                                            */
										    /* "fd"                   - file descriptor pointing to the opened serial port */
										    /*	"write_buffer"         - address of the buffer containing data	           */
										    /* "sizeof(write_buffer)" - No of bytes to write                               */	
		printf("\n  %s written to ttyUSB0",write_buffer);
		printf("\n  %d Bytes written to ttyUSB0", bytes_written);
		printf("\n +----------------------------------+\n\n");
		sleep(2);
		tcflush(fd, TCIFLUSH);   /* Discards old data in the rx buffer            */	

		while (bytes_read==0)
			bytes_read = read(fd,&read_buffer,32); /* Read the data                   */


                printf("\n\n  Bytes Rxed -%d", bytes_read); /* Print the number of bytes read */

                printf("\n\n  ");

		for(i=0;i<bytes_read;i++)              /*printing only the received characters*/
			printf("%c",read_buffer[i]);

		printf("\n +----------------------------------+\n\n\n");
		strcat(com, read_buffer);
		strcat(com, fin);
		printf("\n  %s  \n",com);

		//este  código solo es para probar comunicación serial
		
		system(com);
		system("clear");
		printf("reinicio de lectura %i", n);
		//reinicio de variables
		bytes_read=0;
		strcpy (com,"mosquitto_pub -h iot.eclipse.org -t testkd -m 'recibo por rs485:");
	
		
		if (n==32)
		{
			n=1;
			sleep(30); //aqui se ingresa el tiempo de espera en segundos en el que despues de una solicitud de 
			//todos los controladores de la estación se vuelve a solicitar info a todos--------en este caso 30 seg
		}
		else
		{
			n++;
			sleep(5); //aqui se ingresa el tiempo de espera en segundos entre la solicitud a un controlador 
			// y el siguiente-----en este caso 5 seg
		}

		
	}
	
        close(fd); /* Close the serial port */
}
