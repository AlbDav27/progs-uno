//leer un archivo cada 16 bytes
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>             /* File Control Definitions           */
#include <termios.h>      /* POSIX Terminal Control Definitions */
#include <unistd.h>         /* UNIX Standard Definitions            */
#include <errno.h>           /* ERROR Number Definitions           */
#include <sys/ioctl.h>   /* ioctl() */
#include <unistd.h> 	//para funcion sleep()  ...delay
#include <string.h>
#include <math.h>

FILE *fp;

int k;
int bn[24];
unsigned char nch[3];
int fd;/*File Descriptor*/

int nc=24;
int nf=4;


void binario ( int n )
{
	if ( n != 0)
	{
		binario ( n / 2);
		printf ("%d", n % 2);
		bn[k]=n%2;
		k++;
	}	
}
void conv(int numero){
	int bin[24];
	int p, sum;
	k=0;
	while (k<24){
		bin[k]=0;
		bn[k]=0;
		k++;	
	}
	printf ("\nvectores en 0 \n");
	k=1;
	binario(numero);
	p=k;
	k=0;
	while(p>0){
		bin[k]=bn[p-1];
		p--;
		k++;	
	}
	printf (" \nEL número invertido es : ");
	p=0;
	while (p<24){
		printf ("%d", bin[p]);
		p++;	
	}
	printf (" \n");
	p=0;
	sum = 0;
	while(p<8){
		if (bin[p]==1){
			sum = sum+ (pow(2,p));
		}
		p++;
	}
	printf("\nel valor de la suma : %d\n", sum);
	nch[2]=sum;
	printf("\nel caracter es : %d\n", nch[2]);
	sum=0;
	while(p<16){
		if (bin[p]==1){
			sum = sum+ (pow(2,p%8));
		}
		p++;
	}
	nch[1]=sum;
	printf("\nel valor de la suma : %d\n", sum);
	printf("\nel caracter es : %d\n", nch[1]);
	sum=0;
	while(p<24){
		if (bin[p]==1){
			sum = sum+ (pow(2,p%8));
		}
		p++;
	}
	nch[0]=sum;
	printf("\nel valor de la suma : %d\n", sum);
	printf("\nel caracter es : %d\n", nch[0]);
	sum =0;
}

unsigned char CheckSumByte(unsigned char *DatosaEnviar, unsigned char NumBytes){
	unsigned char CheksumResult, i;
	CheksumResult = DatosaEnviar[0];
	for(i = 1; i < NumBytes; i++){
	    CheksumResult ^= DatosaEnviar[i];
	}
	CheksumResult &= 0x7F;
	return CheksumResult;
}

void act_sflc(){
	long lf;
	int ll;
	int cc=1;
	int fin=0;
	int c=1;
	int ck=1;
	int nf;		//nf- number of frames
	int fok;
	int bytes_written;
	int bytes_read;
	//char xm[170];
	char rec[7];
	unsigned char frame[17];
	char mx[30];
	//char bin[25];
	char n[5];
	unsigned char ch;

	
	//fp = fopen ("/home/alberto/Documents/ecob/acsof/ebike_application.bin","r");
	fp = fopen ("/home/upd/lc/ebike_application.bin","r");
	printf("\nEl valor de fp es : %d\n", fp);
	if (fp!=NULL){
		fseek(fp, 0L, SEEK_END);
		lf=ftell(fp);
		//rewind(fp);
		printf("\nel archivo de actualizacion tiene una logitud de %d bytes\n", lf);
		conv(lf);
		//leer el documento///
		nf=lf/16;
		if ((nf*16)<lf){
			nf++;
		}
		printf("\nEl archivo tiene: %d frames\n", nf);
		while (c<=nc){
			strcpy(mx, "cl");
			if ((c/10)==0){
				strcat(mx,"0");
			}
			sprintf(n,"%d",c);
			strcat(mx,n);
			strcat(mx,"/");
			strcat(mx, nch);
			printf("\n El comando a enviar actualizacion al lockcontroller %d es: %s\n", c, mx);
			//bytes_written = write(fd,mx, 17);
			usleep(100);
			rewind(fp);
			ck = 1;
			
			while(ck<=nf){
				fok=0;
				while(cc<17 && fin<1){
					if (!feof(fp)){
						ch= fgetc(fp);
						frame [cc-1]= ch;
						cc++;
					}
					else{
						//frame [cc-1]= ch;
						fin =1;
					}
				}
				frame[16] = CheckSumByte(frame, 16);
				printf("\nSe agrego byte de Cheksum\n");

				if (fin ==0&&ck!=nf){
					printf("\n Se envia el frame %d : %s\n", ck, frame);
					while(fok==0){
						bytes_written = write(fd,mx, 17);
						
					
						usleep(100000);
						bytes_read = read(fd,rec, 7);
						if(rec[0]=='f'&&rec[1]=='r'&&rec[2]=='a'&&rec[3]=='m'&&
							rec[4]=='e'&&rec[5]=='o'&&rec[6]=='k'){
							fok=1;
						}
					}										
				}else{
					printf("\n Se envia el ultimo frame %d : %s\n", ck, frame);
					//bytes_written = write(fd,mx, cc);
					while(fok==0){
						bytes_written = write(fd,mx, cc-1);	//para vsalidar si hay error, modificar ultimo parametro a cc-2
						
					
						usleep(100000);
						bytes_read = read(fd,rec, 7);
						if(rec[0]=='f'&&rec[1]=='r'&&rec[2]=='a'&&rec[3]=='m'&&
							rec[4]=='e'&&rec[5]=='o'&&rec[6]=='k'){
							fok=1;
						}
					}
					fin =0;
				}
				cc=1;
				ck++;
			}
			ck=1;
			usleep(1000);
			printf("\nSe actualizo LC: %d\n", c);
			c++;
		}
		fclose(fp);
		printf("\nSe actualizaron todas las lock controller\n");
		//system("rm /home/upd/lc/ebike_application.bin");
		sleep(2);
	}else{
		printf("\nNo existe actualizacion de las Lock Controller\n");
	}
}

void act_sfcn(){
long lf;
	int ll;
	int cc=1;
	int fin=0;
	int c=1;
	int ck=1;
	int nf;		//nf- number of frames
	int fok;
	int bytes_written;
	int bytes_read;
	//char xm[170];
	char rec[7];
	unsigned char frame[17];
	char mx[30];
	//char bin[25];
	char n[5];
	unsigned char ch;

	
	//fp = fopen ("/home/alberto/Documents/ecob/acsof/ebike_application.bin","r");
	fp = fopen ("/home/upd/lc/ebike_application.bin","r");
	printf("\nEl valor de fp es : %d\n", fp);
	if (fp!=NULL){
		fseek(fp, 0L, SEEK_END);
		lf=ftell(fp);
		//rewind(fp);
		printf("\nel archivo de actualizacion tiene una logitud de %d bytes\n", lf);
		conv(lf);
		//leer el documento///
		nf=lf/16;
		if ((nf*16)<lf){
			nf++;
		}
		printf("\nEl archivo tiene: %d frames\n", nf);
		while (c<=nc){
			strcpy(mx, "cc");
			if ((c/10)==0){
				strcat(mx,"0");
			}
			sprintf(n,"%d",c);
			strcat(mx,n);
			strcat(mx,"/");
			strcat(mx, nch);
			printf("\n El comando a enviar actualizacion al lockcontroller %d es: %s\n", c, mx);
			//bytes_written = write(fd,mx, 17);
			usleep(100);
			rewind(fp);
			ck = 1;
			
			while(ck<=nf){
				fok=0;
				while(cc<17 && fin<1){
					if (!feof(fp)){
						ch= fgetc(fp);
						frame [cc-1]= ch;
						cc++;
					}
					else{
						//frame [cc-1]= ch;
						fin =1;
					}
				}
				frame[16] = CheckSumByte(frame, 16);
				printf("\nSe agrego byte de Cheksum\n");

				if (fin ==0&&ck!=nf){
					printf("\n Se envia el frame %d : %s\n", ck, frame);
					while(fok==0){
						bytes_written = write(fd,mx, 17);
						
					
						usleep(100000);
						bytes_read = read(fd,rec, 7);
						if(rec[0]=='f'&&rec[1]=='r'&&rec[2]=='a'&&rec[3]=='m'&&
							rec[4]=='e'&&rec[5]=='o'&&rec[6]=='k'){
							fok=1;
						}
					}										
				}else{
					printf("\n Se envia el ultimo frame %d : %s\n", ck, frame);
					//bytes_written = write(fd,mx, cc);
					while(fok==0){
						bytes_written = write(fd,mx, cc-1);	//para vsalidar si hay error, modificar ultimo parametro a cc-2
						
					
						usleep(100000);
						bytes_read = read(fd,rec, 7);
						if(rec[0]=='f'&&rec[1]=='r'&&rec[2]=='a'&&rec[3]=='m'&&
							rec[4]=='e'&&rec[5]=='o'&&rec[6]=='k'){
							fok=1;
						}
					}
					fin =0;
				}
				cc=1;
				ck++;
			}
			ck=1;
			usleep(1000);
			printf("\nSe actualizo LC: %d\n", c);
			c++;
		}
		fclose(fp);
		printf("\nSe actualizaron todas las lock controller\n");
		//system("rm /home/upd/lc/ebike_application.bin");
		sleep(2);
	}else{
		printf("\nNo existe actualizacion de las Lock Controller\n");
	}
}
void act_sfsm(){
	int ll;
	int ck=0;
	char xm[170];
	char mx[30];
	//char bin[25];

	strcpy(xm, "curl -X GET \"https://my.rayven.io:8082/api/main?uid=111848e7eda9ff3b47e3aba02197e37a6a94&deviceid=sm_prog\" >/home/upd/sm.c");
	printf("\n\n\nejecuto post : %s \n", xm);
	ll= strlen(xm);
	printf("\n la longitud del comando es : %d\n\n", ll);
	//system (xm);
	if (ck==1){
		//system ("rm /home/prog/p5.c");
		//system ("rm /home/prog/p");
		//system ("mv /home/upd/sm.c /home/prog/p5.c");
		//system("gcc /home/prog/p5.c -o /home/prog/p");
		//fp = fopen ("sm.c","r");
	
		sleep(2);
		//fclose(fp);
	}
	
}

int main()
{
	int numero;
	nch[0]=0;
	nch[1]=0;
	nch[2]=0;

	/*------------------------------- Opening the Serial Port -------------------------------*/
	/* Change /dev/ttyS0 to the one corresponding to your system */
	fd = open("/dev/ttyS0",O_RDWR | O_NOCTTY);           /* ttyS0 corresponde al puerto serial rs485 de UNO-1252G   */  
                                                              /* O_RDWR   - Read/Write access to serial port       */
                                                              /* O_NOCTTY - No terminal will control the process   */
                                                              /* Open in blocking mode,read will wait              */
	///////SI CON EL PUERTO ABIERTO NORMALMENTE SIGUE SIN RESPETARSE LAS CONDICIONES VMIN Y VTIME, SE DEBE AGREGAR A LA INSTRUCCION OPEN | O_NDELAY
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
	SerialPortSettings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */
	SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/
		/* Setting Time outs */
	SerialPortSettings.c_cc[VMIN] =  0; 		//pure time read 
	SerialPortSettings.c_cc[VTIME] = 2;  /* Wait 1s   */	//VTIME TIEMPO EN DECIMAS DE SEGUNDO
	/*int RTS_flag,DTR_flag;
		RTS_flag = TIOCM_RTS;	/* Modem Constant for RTS pin 
		DTR_flag = TIOCM_DTR;	/* Modem Constant for DTR pin 
		ioctl(fd,TIOCMBIS,&RTS_flag);/* ~RTS = 1,So ~RE pin of MAX485 is HIGH                       
		ioctl(fd,TIOCMBIS,&DTR_flag);/* ~DTR = 1,So  DE pin of MAX485 is HIGH,Transmit Mode enabled */ 
	if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0){ 
		    printf("\n  ERROR ! in Setting attributes");
	}
	else{
             printf("\n  BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none");
 	}
 	tcflush(fd, TCIFLUSH);			//limpiar buffer de entrada
	tcflush(fd, TCOFLUSH);			//limpiar buffer de salida

	act_sflc();
	//system ("rm /home/upd/lc/ebike_application.bin");
	tcflush(fd, TCIFLUSH);			//limpiar buffer de entrada
	tcflush(fd, TCOFLUSH);			//limpiar buffer de salida
	sleep(3);
	act_sfcn();
	//system ("rm /home/upd/cn/ebike_application.bin");
	tcflush(fd, TCIFLUSH);			//limpiar buffer de entrada
	tcflush(fd, TCOFLUSH);			//limpiar buffer de salida
	sleep(3);
	//act_sfsm();
	//sleep (3);
	//system("rm /home/root/Documents/ecob/acosf/ebike_application.bin");
	//se borra archivo de actualizacion de lock controllers
	//system("rm /home/root/Documents/ecob/acosf/ebike_application.bin");
	//se borra el archivo de actualizacion de controladores 

	//si no se borran los archivos de actualizacion el sistema de monitoreo envia 
	//en cada reinicio el mismo programa  esto es muy tardado.
	return 0;
}
