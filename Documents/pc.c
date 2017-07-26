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
	SerialPortSettings.c_cc[VMIN] =  32; /* Read at least 32 character */
	SerialPortSettings.c_cc[VTIME] = 10;  /* Wait 1s   */

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
	/*---------- Setting the Attributes of the serial port using termios structure --------- */
	

 	/////////declaración de variables//////////////////////////////////

	char read_buffer[32];   /* Buffer to store the data received              */
    char com [300];
    char cmx[5];
    char temp[3];
	char fuentes[200];
	char control[200];

    int  bytes_read = 0;    /* Number of bytes read by the read() system call */
    int  bytes_written  = 0;  	/* Value for storing the number of bytes written to the port */
    int i =0;
    int n;
	int f;
	int dec=0;

	/////////////////////////////////////////variables qque cambian paraa cada estación
	int nc=36;					//número de controladores de la estación
	int id_st=0;				//identificador de la estación
	
	//////////////Inicialización de variables/////////////////////////////
	strcpy (com, "");
	strcpy (fuentes, "");
	strcpy (control, "");
	f = 1;
	n = 1;
	
	sleep(5);				 ////////retardo para asegurar que se envie una vez que el receptor este listo para recibir
	
	tcflush(fd, TCIFLUSH);			//limpiar buffer de entrada
	tcflush(fd, TCOFLUSH);			//limpiar buffer de salida
		
	
//Este es el bucle que se va a repetir para estar enviando y recibiendo constantemente información

	while (1)
	{	
		
		strcat(cmx,"");
		printf("\n\n n= %i  f = %i    ", n , f);
		if (f<7)
		{		
			sprintf(temp,"%d",f);
			strcpy(cmx, "$AA");					
			strcat(cmx, temp);
			strcat(cmx,"*");
			printf("\nSolicitud: %s/findecad", cmx);
			
		}else{
			strcpy(cmx, "c:");
			dec=n/10;
			if (dec==0){
				strcat(cmx,"0");	
			}
			sprintf(temp,"%d",n);					
			strcat(cmx, temp);
			strcat(cmx,"*");
			printf("\nSolicitud: %s/findecad", cmx);
		}
		
		bytes_written = write(fd,cmx,5);			//enviar cadena cmx
		tcflush(fd, TCIOFLUSH);
		printf("\n\n Listo para recibir");
		usleep(50000);

		bytes_read = read(fd,read_buffer,32);		//leer datos y almacenarlos en el array read_buffer
        printf("\n Recibo: /");
        
		for(i=0;i<bytes_read;i++)              /*printing only the received characters*/
			printf("%c",read_buffer[i]);

		printf("/fincadena \n +----------------------------------+\n\n");
		bytes_read=0;
		tcflush(fd, TCIOFLUSH);
		if (n==32)								//si es el último controlador
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
			strcpy (com, "");
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

			//reinicio de las variables, vaciar los buffers
			strcpy (com, "  ");
			strcpy(fuentes, "  ");
			strcpy(control, "  ");
		
			sleep(2);
		}
		else if (f<=6)							//almacena la información de las fuentes
		{
			if(f!=1){
				strcat(fuentes, "; ");
			}
			strcat(fuentes, read_buffer);			
			f++;
			usleep(500000); //aqui se ingresa el tiempo de espera en segundos entre la solicitud a un controlador 
			//para que cada minuto se solicite 1 vez la info de cada disp se divide 60s / 38 disp = 1.57

		}else{									//almacena la información de los controladores, excepto el último
			if(n!=1){
				strcat(control, "; ");
			}
			strcat(control, read_buffer);
			n++;
			usleep(500000);
		}
		printf ("\n acabe un ciclo");
		usleep(400000);						//este reatardo debe ser mucho mayor que el de los controladores para que cuando vuelva a enviar info el receptor ya este leyendo
		
	}
	
    close(fd); /* Close the serial port */
}
