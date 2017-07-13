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
	
	 

	if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0){ 
		    printf("\n  ERROR ! in Setting attributes");
	}
	else{
                    printf("\n  BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none");
 	}
	
	

	int RTS_flag,DTR_flag;

	RTS_flag = TIOCM_RTS; /* Modem Constant for RTS pin */

        DTR_flag = TIOCM_DTR; /* Modem Constant for DTR pin */

 
	ioctl(fd,TIOCMBIS,&RTS_flag);/* ~RTS = 0,So ~RE pin of MAX485 is LOW,Receive Mode enabled   */

        ioctl(fd,TIOCMBIS,&DTR_flag);/* ~DTR = 0,So  DE pin of MAX485 is LOW,                       */

 	/////////apartir de aqui va el programa//////////////////////////////////

	char read_buffer[32];   /* Buffer to store the data received              */
       	int  bytes_read = 0;    /* Number of bytes read by the read() system call */
        int i =0;
	char com [500];
	strcpy (com, "");
	
	
	int n;
	int f;
	f= 1;
	n = 1;
	sleep(2);
	tcflush(fd, TCIFLUSH);   /* Discards old data in the rx buffer            */
	char cmx[5];	
	int  bytes_written  = 0;  	/* Value for storing the number of bytes written to the port */ 
        char temp[3];
	char fuentes[200];
	char control[200];
	strcpy (fuentes, "");
	strcpy (control, "");

/*------------------------------- Read data from serial port -----------------------------*/ 
//Este es el bucle que se va a repetir para estar recibiendo constantemente información

 
	
// se debe de verificar si linux permite que se ejecute indefinidamente el programa, o hay que ejecutar periodicamente el mismo

	while (1)
	{		
		tcflush(fd, TCIFLUSH);
		//tcflush(fd, TCOFLUSH);
		printf("\n\n n= %i  f = %i    \n\n", n , f);
		//////////////////////Cada ciclo completo de medición de todas las fuentes y controladores, incluye el envio de información///////
		if(f<=6)
		{
						
			sprintf(temp,"%d",f);
			strcpy(cmx, "$AA");					
			strcat(cmx, temp);
		}else{
			sprintf(temp,"%d",n);
			strcpy(cmx, "cont:");					
			strcat(cmx, temp);
		}
		
		
		strcat(cmx,"*");
		printf("\n   Solicitud: %s/findecad", cmx);
		
		bytes_written = write(fd,cmx,sizeof(cmx));		
			
		//memset(read_buffer, 0, 32);
		//while (bytes_read==0)
		bytes_read = read(fd,&read_buffer,32); /* Read the data                   */


                printf("\n Recibo:   ");

		for(i=0;i<bytes_read;i++)              /*printing only the received characters*/
			printf("%c",read_buffer[i]);

		printf("\n +----------------------------------+\n\n");
		
		bytes_read=0;
		
		
		
		if (n==32)
		{
			n=1;
			f=1;
			printf("\n antes de cat ultimo ; \n");
			sleep(2);
			strcat(control, ";");
			printf("\n antes de cat ultimo dato \n");
			sleep(2);
			strcat(control, read_buffer);
			
			printf("\n cadena datos fuentes = %s \n",fuentes);
			printf("\n cadena datos controladores = %s \n", control);
			sleep(2);
			
			//memset(com1, 0, 250);
			strcpy (com, "");
			//esta parte envia la información de controladores y fuentes//
			//strcpy (fin," '");
			strcat(com,"mosquitto_pub -h iot.eclipse.org -t estacion1/fuentes -m 'recibo por rs485:");
			printf("\n antes de 1er cat %s\n",com);
			sleep(2);
			//strcat(com, fuentes);
			printf("\n antes de 2 cat %s\n",com);
			sleep(2);
			strcat(com, " '");
			printf("\n %s \n",com);
			system(com);
			//system("clear");
			strcpy (com, "");
			//memset(com1, 0, 250);
			
			
			strcat (com,"mosquitto_pub -h iot.eclipse.org -t estacion1/controladores -m 'recibo por rs485:");
			printf("\n antes de 1er cat \n");
			sleep(2);
			//strcat(com, control);
			printf("\n antes de 2 cat \n");
			sleep(2);
			strcat(com, " '");
			printf("\n %s \n",com);
			system(com);
			//system("clear");
			strcpy (com, "  ");
			
			strcpy(fuentes, "  ");
			strcpy(control, "  ");
			
			//tcflush(fd, TCIFLUSH);
			sleep(2);
		}
		else if (f<=6)
		{
			if(f!=1){
				strcat(fuentes, "; ");
			}
			strcat(fuentes, read_buffer);			
			f++;
			usleep(1500000); //aqui se ingresa el tiempo de espera en segundos entre la solicitud a un controlador 
			//para que cada minuto se solicite 1 vez la info de cada disp se divide 60s / 38 disp = 1.57

		}else{
			if(n!=1){
				strcat(control, "; ");
			}
			strcat(control, read_buffer);
			n++;
			usleep(1500000);
		}
		
		
	}
	
        close(fd); /* Close the serial port */
}
