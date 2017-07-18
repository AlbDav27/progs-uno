#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>             /* File Control Definitions           */
#include <termios.h>      /* POSIX Terminal Control Definitions */
#include <unistd.h>         /* UNIX Standard Definitions            */
#include <errno.h>           /* ERROR Number Definitions           */
#include <sys/ioctl.h>   /* ioctl() */
#include <unistd.h> 	//para funcion sleep()  ...delay
#include <string.h>

int main()
{

	int fd;/*File Descriptor*/

 
	/*------------------------------- Opening the Serial Port -------------------------------*/

	/* Change /dev/ttyS0 to the one corresponding to your system */

	fd = open("/dev/ttyS1",O_RDWR | O_NOCTTY);           /* ttyS0 corresponde al puerto serial rs485 de UNO-1252G   */  

                                                               /* O_RDWR   - Read/Write access to serial port       */

                                                              /* O_NOCTTY - No terminal will control the process   */

                                                              /* Open in blocking mode,read will wait              */

                                                                                                                                               
	if(fd == -1){          
		printf("\n  Error! in Opening ttyS0  ");
	}
	else{
		printf("\n  ttyS0 Opened Successfully ");
	}

 

               
	/*---------- Setting the Attributes of the serial port using termios structure --------- */

                             
	struct termios SerialPortSettings;	/* Create the structure                          */

	tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */

		/* Setting the Baud rate */
	cfsetispeed(&SerialPortSettings,B9600); /* Set Read  Speed as 9600                       */
	cfsetospeed(&SerialPortSettings,B9600); /* Set Write Speed as 9600                       */

		/* 8N1 Mode */
	SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
	SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
	SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
	SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */
		
	SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */ 
		
		
	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */

	SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/
		
		/* Setting Time outs */
	SerialPortSettings.c_cc[VMIN] =  10; /* Read at least 10 character */
	SerialPortSettings.c_cc[VTIME] = 0;  /* Wait indefinetly   */

	int RTS_flag,DTR_flag;

		RTS_flag = TIOCM_RTS;	/* Modem Constant for RTS pin */
		DTR_flag = TIOCM_DTR;	/* Modem Constant for DTR pin */

		ioctl(fd,TIOCMBIS,&RTS_flag);/* ~RTS = 1,So ~RE pin of MAX485 is HIGH                       */
		ioctl(fd,TIOCMBIS,&DTR_flag);/* ~DTR = 1,So  DE pin of MAX485 is HIGH,Transmit Mode enabled */ 
	
	 

	if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0){ 
		    printf("\n  ERROR ! in Setting attributes");
	}
	else{
             
             printf("\n  BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none");
 	}
	
	

 	/////////apartir de aqui va el programa//////////////////////////////////

	char read_buffer[8];   /* Buffer to store the data received              */
    int  bytes_read = 0;    /* Number of bytes read by the read() system call */
    int i =0;
	char com [300];
	strcpy (com, "");
	
	
	int n;
	int f;
	f = 1;
	n = 1;
	
	
	char cmx[5];	
	int  bytes_written  = 0;  	/* Value for storing the number of bytes written to the port */ 
    char temp[3];
	char fuentes[200];
	char control[200];
	strcpy (fuentes, "");
	strcpy (control, "");
	tcflush(fd, TCIFLUSH);
	//tcflush(fd, TCOFLUSH);
	
	
	int dec=0;


	while (1)
	{	
		printf("\n\n Listo para recibir");
		


		usleep(100000);

		n=serial_read(fd,read_buffer,5,2000000);
		//printf("\nantes de leer");
		
		

        printf("\n Recibo:   ");
        

		for(i=0;i<bytes_read;i++)              /*printing only the received characters*/
			printf("%c",read_buffer[i]);

		printf("/fincadena \n +----------------------------------+\n\n");
		
		bytes_read=0;

		tcflush(fd, TCIFLUSH);

		
	}
	
     close(fd); /* Close the serial port */
}

int serial_read(int serial_fd, char *data, int size, int timeout_usec)
{
  fd_set fds;
  struct timeval timeout;
  int count=0;
  int ret=0;
  int n;
  int con=0;
  
  //-- Wait for the data. A block of size bytes is expected to arrive
  //-- within the timeout_usec time. This block can be received as 
  //-- smaller blocks.
  do {
      //-- Set the fds variable to wait for the serial descriptor
      FD_ZERO(&fds);
      FD_SET (serial_fd, &fds);
      timeout.tv_sec = 0;  
      timeout.tv_usec = timeout_usec;
      printf("\nret debe ser 0=:%i\n", ret);

      ret=select (serial_fd,&fds, NULL, NULL,&timeout);
      printf("\nret debe cambiar a 1=:%i\n", ret);
    	//-- If there are data waiting: read it
      if (ret==1) {

       		n=read (serial_fd + 1, &data[count], size-count); 
        	count+=n;
        
        	data[count]=0;
        	printf("\nla funcion recibe:%s\n", data);

      }
      
      printf("\n ret = %i, n= %i, count=%i, size=%i",ret, n, count, size);
    	//-- Repeat the loop until a data block of size bytes is received or
    	//-- a timeout occurs
      con= (count<size);
      //printf("\nel valor de la condición es :%i\n", con);
  } while (count<size);
  
  printf("\nse acaba el do while la funcion retorna count\n");
  //-- Return the number of bytes reads. 0 If a timeout has occurred.
  return count;
}
