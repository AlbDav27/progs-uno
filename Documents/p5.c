#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>             /* File Control Definitions           */
#include <termios.h>      /* POSIX Terminal Control Definitions */
#include <unistd.h>         /* UNIX Standard Definitions            */
#include <errno.h>           /* ERROR Number Definitions           */
#include <sys/ioctl.h>   /* ioctl() */
#include <unistd.h> 	//para funcion sleep()  ...delay
#include <string.h>
#include <json/json.h>

///////////variables globales/////////////////////////
int control[37][5];		//tabla donde se almacenan los datos de las Lock Controller
int fuente[9][3];		//tabla donde se almacenan los datos de las fuentes
int v[2];				//2 valores de configuración recibidos
int x =0;				//variable que distingue entr variables de configuración
char js[2000];			//cadena json a enviar
int b, slots, st;

/////////////////////////////////////////////////////funciones para parseo JSON////////////////////////////////////////////////////////////////////////

void print_json_value(json_object *jobj){
  enum json_type type;

  //printf("\ntype: ");
  type = json_object_get_type(jobj); /*Getting the type of the json object*/
  switch (type) {
    case json_type_boolean:
                         break;
    case json_type_double: 
                         break;
    case json_type_int: 
						v[x]=json_object_get_int(jobj);
                         break;
    case json_type_string: 
                         break;
  }
  if (x==0){
  		x++;	
  }else{
  		x=0;
  }
  
}

/*Parsing the json object*/
void json_parse(json_object * jobj) {
  
  enum json_type type;
  json_object_object_foreach(jobj, key, val) { //Passing through every array element//
    
    type = json_object_get_type(val);
    switch (type) {
      case json_type_boolean: 
      case json_type_double: 
      case json_type_int: 
      case json_type_string: print_json_value(val);
                           break; 
    }
  }
} 
/////////////////////////////////////////////////////////////////funciones para transformar cadena recibida a valores enteros
int conv_st_int3(char str[3]){
	int num;
	int cent=0;
	int dec=0;
	int m=1;
	if (str[0]=='1'||str[0]=='2'||str[0]=='3'||str[0]=='4'||str[0]=='5'||str[0]=='6'||str[0]=='7'||
		str[0]=='8'||str[0]=='9'){
				
				cent=str[0]-48;

	}else if (str[0]=='-'){
		m=-1;
	}
	if (str[1]=='1'||str[1]=='2'||str[1]=='3'||str[1]=='4'||str[1]=='5'||str[1]=='6'||str[1]=='7'||
		str[1]=='8'||str[1]=='9'){
				
				dec=str[1]-48;

	}
	num=str[2]-48;
	num = (cent*100)+(dec*10)+num;
	num = num*m;
	return num;
}
int conv_st_int5(char str[5]){
	int num;
	int cent=0;
	int dec=0;
	int um=0;
	int dm=0;
	int m=1;
	if (str[0]=='1'||str[0]=='2'||str[0]=='3'||str[0]=='4'||str[0]=='5'||str[0]=='6'||str[0]=='7'||
		str[0]=='8'||str[0]=='9'){
				
				dm=str[0]-48;

	}else if (str[0]=='-'){
		m=-1;
	}
	if (str[1]=='1'||str[1]=='2'||str[1]=='3'||str[1]=='4'||str[1]=='5'||str[1]=='6'||str[1]=='7'||
		str[1]=='8'||str[1]=='9'){
				
				um=str[1]-48;

	}
	if (str[2]=='1'||str[2]=='2'||str[2]=='3'||str[2]=='4'||str[2]=='5'||str[2]=='6'||str[2]=='7'||
		str[2]=='8'||str[2]=='9'){
				
				cent=str[2]-48;

	}
	if (str[3]=='1'||str[3]=='2'||str[3]=='3'||str[3]=='4'||str[3]=='5'||str[3]=='6'||str[3]=='7'||
		str[3]=='8'||str[3]=='9'){
				
				dec=str[3]-48;

	}
	num=str[4]-48;
	num = (dm*10000)+(um*1000)+(cent*100)+(dec*10)+num;
	num = num*m;
	return num;
}
//////////////////////////////////////////esto se debe de quitar o modificar// dejar en la tabla valores default, asi si no se registra 
// los valores de alguna Lock Controller por que cortocircuito o falla electrica, se envian los valores default la tabla 
void def_table(int n){
	int y;
	for (y=0;y<n+1;y++){
		control[y][0]=y;
		control[y][1]=-1;
		control[y][2]=-1;
		control[y][3]=-1;
		control[y][4]=0;
	}

}
void df_tabl(int n){
	int y;
	for (y=0;y<n+1;y++){
		fuente[y][0]=y;
		fuente[y][1]=-1;
		fuente[y][2]=-1;
	}
}

///////////////////////esta funcion determina los valores generales de la estación/////////////////
void getgval(int nc){
	int s=1;
	slots=0;
	st=0;
	while (s<=36){
		if (control[s][4]==0){
			slots++;				//cuando una lock no tiene e-bike conectada
		}
		s++;
	}
	s=1;
	while (s<=6){
		if(fuente[s][1]==1||fuente[s][2]==1){
			st++;
		}
		s++;
	}
	b=nc-slots;
}
///////////////////////////////////////////////////////// Esta funcion genera la cadena json que se va a publicar
void to_jsonc(){
	char t [5];
	int q;
	strcpy(js,"");
	strcpy(js,"{\"id_st\":");
	sprintf(t,"%d",27);
	strcat(js, t);
	if (st==0){
		strcat(js, ", \"status\":\"ok\", \"bikes\":");
	}else{
		strcat(js, ", \"status\":\"nok\", \"bikes\":");
	}
	sprintf(t,"%d", b);
	strcat(js, t);
	strcat(js, ", \"slots\":");
	sprintf(t,"%d", slots);
	strcat(js, t); 
	strcat(js, ", \"chargers\":[");
	for(q=1;q<=6;q++){
		strcat(js,"{\"id_f\":");
		sprintf(t,"%d",fuente[q][0]);			//id_c se encuentra en las posiciones [0][q] de control
		strcat(js, t);
		strcat(js,", \"temperature\":");
		sprintf(t,"%d",fuente[q][1]);			//id_b se encuentra en las posiciones [4][q] de control
		strcat(js, t);
		strcat(js,", \"status\":");
		sprintf(t,"%d",fuente[q][2]);			//st_ch se encuentra en las posiciones [1][q] de control
		strcat(js, t);
		if (q<6){
			strcat(js, "},");
		}
		else {
			strcat(js, "}],");
		}
	}
	strcat(js, "\"locks\":[");
	for(q=1;q<=36;q++){
		strcat(js,"{\"id_c\":");
		sprintf(t,"%d",control[q][0]);			//id_c se encuentra en las posiciones [0][q] de control
		strcat(js, t);
		strcat(js,", \"id_b\":");
		sprintf(t,"%d",control[q][4]);			//id_b se encuentra en las posiciones [4][q] de control
		strcat(js, t);
		strcat(js,", \"st_ch\":");
		sprintf(t,"%d",control[q][1]);			//st_ch se encuentra en las posiciones [1][q] de control
		strcat(js, t);
		strcat(js,", \"hs_ba\":");
		sprintf(t,"%d",control[q][2]);			//hs_ba se encuentra en las posiciones [2][q] de control
		strcat(js, t);
		strcat(js,", \"nc\":");
		sprintf(t,"%d",control[q][3]);			//nc se encuentra en las posiciones [3][q] de control
		strcat(js, t);
		if (q<36){
			strcat(js, "},");
		}
		else {
			strcat(js, "}]");
		}
	}
	
	strcat(js, "}");
}

void f_data(char cad[8]){
	int n=1;
	int v=8;

	//fuente[v][0]=0;

	while (n<5){
		if (cad[n-1]=='0'){
			
			fuente [v][1]=0;			//fuente[v][1]  aqui se encuentra la variable temperautre
			fuente [v][2]=0;			//fuente[v][2]	aqui se encuentra la variable status
			fuente [v-1][1]=0;
			fuente [v-1][2]=0;
		}
		else if (cad[n-1]=='1'){
			fuente [v][1]=1;
			fuente [v][2]=0;
			fuente [v-1][1]=0;
			fuente [v-1][2]=0;
		}
		else if (cad[n-1]=='2'){
			fuente [v][1]=0;
			fuente [v][2]=1;
			fuente [v-1][1]=0;
			fuente [v-1][2]=0;
		}
		else if (cad[n-1]=='3'){
			fuente [v][1]=1;
			fuente [v][2]=1;
			fuente [v-1][1]=0;
			fuente [v-1][2]=0;
		}
		else if (cad[n-1]=='4'){
			fuente [v][1]=0;
			fuente [v][2]=0;
			fuente [v-1][1]=1;
			fuente [v-1][2]=0;
		}
		else if (cad[n-1]=='5'){
			fuente [v][1]=1;
			fuente [v][2]=0;
			fuente [v-1][1]=1;
			fuente [v-1][2]=0;
		}
		else if (cad[n-1]=='6'){
			fuente [v][1]=0;
			fuente [v][2]=1;
			fuente [v-1][1]=1;
			fuente [v-1][2]=0;
		}
		else if (cad[n-1]=='7'){
			fuente [v][1]=1;
			fuente [v][2]=1;
			fuente [v-1][1]=1;
			fuente [v-1][2]=0;
		}
		else if (cad[n-1]=='8'){
			fuente [v][1]=0;
			fuente [v][2]=0;
			fuente [v-1][1]=0;
			fuente [v-1][2]=1;
		}
		else if (cad[n-1]=='9'){
			fuente [v][1]=1;
			fuente [v][2]=0;
			fuente [v-1][1]=0;
			fuente [v-1][2]=1;
		}
		else if (cad[n-1]=='A'){
			fuente [v][1]=0;
			fuente [v][2]=1;
			fuente [v-1][1]=0;
			fuente [v-1][2]=1;
		}
		else if (cad[n-1]=='B'){
			fuente [v][1]=1;
			fuente [v][2]=1;
			fuente [v-1][1]=0;
			fuente [v-1][2]=1;
		}
		else if (cad[n-1]=='C'){
			fuente [v][1]=0;
			fuente [v][2]=0;
			fuente [v-1][1]=1;
			fuente [v-1][2]=1;
		}
		else if (cad[n-1]=='D'){
			fuente [v][1]=1;
			fuente [v][2]=0;
			fuente [v-1][1]=1;
			fuente [v-1][2]=1;
		}
		else if (cad[n-1]=='E'){
			fuente [v][1]=0;
			fuente [v][2]=1;
			fuente [v-1][1]=1;
			fuente [v-1][2]=1;
		}
		else if (cad[n-1]=='F'){
			fuente [v][1]=1;
			fuente [v][2]=1;
			fuente [v-1][1]=1;
			fuente [v-1][2]=1;
		}
		v--;v--;
		n++;
	}
}

//////////////////////////////////////////////		INT MAIN	//////////////////////////////////////////////////////
int main()
{

	int fd;/*File Descriptor*/

 
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
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */

	SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/
		
		/* Setting Time outs */
	SerialPortSettings.c_cc[VMIN] =  0; 		//pure time read 
	SerialPortSettings.c_cc[VTIME] = 10;  /* Wait 1s   */	//VTIME TIEMPO EN DECIMAS DE SEGUNDO

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

	char read_buffer[22];   /* Buffer to store the data received              */
    char com [3000];
    char cmx[10];
    char temp[3];
    char t[5];
	char subs[5];
	char sol[70];
	char ci[30];

    int  bytes_read = 0;    /* Number of bytes read by the read() system call */
    int  bytes_written  = 0;  	/* Value for storing the number of bytes written to the port */
    int i =0;
    int n;
	int f;
	int dec=0;
	int cen=0;
	int res=0;
	int num;
	int cp=420;
	int lr;

	FILE *fp;

	/////////////////////////////////////////variables qque cambian paraa cada estación
	int nc=36;					//número de controladores de la estación
	int id_st=1;				//identificador de la estación
	int nf = 8;					//numero de fuentes
	
	///////////////////////este valor es el mismo para todas las estaciones inicialmente
	int pbr=420;			//post before request configuration data
							//una estacion opera 20 h en total, y una estacion postea todos los datos de los dispositivos
							//cada minuto,  420 post = 420min = 7h aproximadamente
	
	cp=pbr;
	//////////////Inicialización de variables/////////////////////////////
	strcpy (com, "");
	strcpy (read_buffer, "");
	strcpy (cmx, "");
	strcpy (ci, "");
	strcpy (subs, "");

	f = 1;
	n = 1;
	
	sleep(5);				 ////////retardo para asegurar que se envie una vez que el receptor este listo para recibir
	
	tcflush(fd, TCIFLUSH);			//limpiar buffer de entrada
	tcflush(fd, TCOFLUSH);			//limpiar buffer de salida
	def_table(nc);
	df_tabl(nf);

/////////////////////Este es el bucle que se va a repetir para estar enviando y recibiendo constantemente información

	while (1)
	{	
		strcpy(cmx, "");
		strcpy(read_buffer, "");
		//strcpy(com, "");
		if (cp==pbr)
		{
			//Primero se realiza una solicitud de la info de conf a la plataforma
			strcpy(com, "curl GET https://my.rayven.io:8082/api/main?uid=111848e7eda9ff3b47e3aba02197e37a6a94&deviceid=conf_data >/home/../response");
			
			printf("\n%s\n", com);
			fp = fopen ("response","r");
			fgets(ci, 100, fp);
			fclose(fp);
			lr=strlen(ci);
			printf("\n la longitud de la cadena recibida es : %d", lr);
			printf("\n la cadena recibida es: %s", ci);
			//system(com);		///esta instruccion ejecuta la solicitud rest desde sh y recibe en ci el resultado
			strcpy (com, "");
			strcpy(ci, "{'b_s':27,'rel_r':72}");		//esta cadena simula la respuesta de la plataforma con los parametros de configuración
			json_object * jobj = json_tokener_parse(ci);     
			json_parse(jobj);
			printf("\nLos valores del json son: %i, %i\n", v[0], v[1]);
			strcpy (com, "");

			///////////se envian los datos a los controladores en Bradcast/////////////////////////////////////

			////////////////////////////////////configuracion de broadcast de carga de liberación
			strcpy(cmx, "cp00/");
			strcpy(temp,"");
			cen = v[0]/100;
			if (cen == 0){
				strcat(cmx, "0");	
			}
			res = v[0]-(cen*100);
			dec = res/10;
			if (dec==0){
				strcat(cmx,"0");	
			}
			sprintf(temp,"%d",v[0]);
			strcat(cmx, temp);
			printf("\n El broadcast carga a las Lock Controller es : %s/fin_cad\n", cmx);
			

			bytes_written = write(fd,cmx,8);						//envio de primer broadcast
			usleep(10000);			
			tcflush(fd, TCIOFLUSH);
			strcpy(cmx, "");
			
			////////////////////////////////////configuracion de broadcast de velocidad
			strcpy(cmx, "cv00/");
			strcpy(temp,"");
			cen = v[1]/100;
			if (cen == 0){
				strcat(cmx, "0");
			}
			res = v[1]-(cen*100);
			dec = res/10;
			if (dec==0){
				strcat(cmx,"0");	
			}
			sprintf(temp,"%d",v[1]);
			strcat(cmx, temp);
			printf("\n El broadcast velocidad a las Lock Controller es : %s/fin_cad\n", cmx);
			

			bytes_written = write(fd,cmx,8);						//envio de segundo broadcast
			usleep(10000);			
			tcflush(fd, TCIOFLUSH);
			cp=0;
		}

		strcpy(cmx,"");
		printf("\n\n n= %i  f = %i    ", n , f);
		////////////solicitud de datos a algun canal del multiplexor, s2 canales de l mux son los datos de 1 fuente
		if (f==1)
		{		
			strcpy(cmx, "$016");
			printf("\nSolicitud: %s/findecad", cmx);
			cmx[4]=13;					
			
			bytes_written = write(fd,cmx,5);
			bytes_read = read(fd,read_buffer,8);		//leer datos y almacenarlos en el array read_buffer
			if (bytes_read>0)
			{
				subs[0]=read_buffer[1];
				subs[1]=read_buffer[2];
				subs[2]=read_buffer[3];
				subs[3]=read_buffer[4];
				f_data(subs);
			}
			
			strcpy(subs,"");
			tcflush(fd, TCIOFLUSH);
			strcpy(cmx, "");
        	printf("\n Recibo: /");
        	for(i=0;i<bytes_read;i++)              /*printing only the received characters*/
				printf("%c",read_buffer[i]);
			printf("/fincadena \n +----------------------------------+\n\n");
			bytes_read=0;
        	f=0;
        	usleep(500000);
	
		}else{					//////////solicitud de datos a lock controller
			strcpy(cmx, "cr");
			dec=n/10;
			if (dec==0){
				strcat(cmx,"0");	
			}
			sprintf(temp,"%d",n);					
			strcat(cmx, temp);
			strcat(cmx,"/***");
			printf("\nSolicitud: %s/findecad", cmx);
			bytes_written = write(fd,cmx,8);	//envio de primer broadcast
			usleep(50000);			
			tcflush(fd, TCIOFLUSH);
			strcpy(cmx, "");
			printf("\n\n Listo para recibir");
			usleep(50000);

			bytes_read = read(fd,read_buffer,22);		//leer datos y almacenarlos en el array read_buffer
        	printf("\n Recibo: /");
        	for(i=0;i<bytes_read;i++)              /*printing only the received characters*/
				printf("%c",read_buffer[i]);
			printf("/fincadena \n +----------------------------------+\n\n");
			bytes_read=0;
			tcflush(fd, TCIOFLUSH);

        	//almacena la información de los controladores
			
			if (bytes_read>0)
			{
				control[n][0]= n;					//id_c : id del controlador

				subs[0]=read_buffer[4];				//st_ch : estado de carga
				subs[1]=read_buffer[5];
				subs[2]=read_buffer[6];
				num=conv_st_int3(subs);
				control[n][1]=num;
				strcpy(subs,"");

				subs[0]=read_buffer[8];				//hs_ba : estado de salud de la bateria
				subs[1]=read_buffer[9];
				subs[2]=read_buffer[10];
				num=conv_st_int3(subs);
				control[n][2]=num;
				strcpy(subs,"");

				subs[0]=read_buffer[12];			//nc : numero de ciclos
				subs[1]=read_buffer[13];
				subs[2]=read_buffer[14];
				subs[3]=read_buffer[15];
				subs[4]=read_buffer[16];
				num=conv_st_int5(subs);
				control[n][3]=num;
				strcpy(subs,"");

				subs[0]='0';
				subs[1]=read_buffer[18];			//id_b : E-Bike ID
				subs[2]=read_buffer[19];
				subs[3]=read_buffer[20];
				subs[4]=read_buffer[21];
				num=conv_st_int5(subs);
				control[n][4]=num;
				strcpy(subs,"");	
			}
			if (n==nc)								//si es el último controlador
			{

				getgval(nc);
				to_jsonc();
				/////////////////////////////////////////////aqui se debe de enviar la informacion
				strcpy(com, "curl -X POST -H \"Content-Type: application/json\" -d '");
				strcat(com, js);
				strcat(com, "' https://my.rayven.io:8082/api/main?uid=111848e7eda9ff3b47e3aba02197e37a6a94&deviceid=st_");
				sprintf(t,"%d",id_st);
				strcat(com, t);
				strcat(com, " >/home/alberto/Documents/rayven/response");
				printf("\n\n\nejecuto post %d : %s \n", n, com);
				//system (com);
				//////////////////////////////////////
				//reinicio de las variables, vaciar los buffers
				strcpy (com, "");
				cp++;
				n=0;
				f=1;
				def_table(nc);		//estabñece en la tabla control los valores por default
				df_tabl(nf);			//establece en la tabla fuente los valores por default
				printf ("\n acabe un ciclo de todos los dispositivos");
				sleep(2);
			}
			n++;
			usleep(500000);
		}
		printf ("\n acabe un disp");
		usleep(400000);						//este reatardo debe ser mucho mayor que el de los controladores para que cuando vuelva a enviar info el receptor ya este leyendo
	}
    close(fd); /* Close the serial port */
}
