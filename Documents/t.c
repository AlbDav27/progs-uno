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

int v[3];

void transform(char dat1[3], char dat2[3], char dat3[3]){
	int un=0;
	int de=0;
	int ce=0;
	int dat;
	if (dat1[2]!=0){
		ce=dat1[0]-48;
		de=dat1[1]-48;
		un=dat1[2]-48;
	}
	else if (dat1[1]!=0){
		ce=0;
		de=dat1[0]-48;
		un=dat1[1]-48;
	}else{
		ce=0;
		de=0;
		un=dat1[0]-48;
	}
	dat=(100*ce)+(10*de)+un;
	v[0]=dat;					//dato de porcentaje de carga

	if (dat2[2]!=0){
		ce=dat2[0]-48;
		de=dat2[1]-48;
		un=dat2[2]-48;
	}
	else if (dat2[1]!=0){
		ce=0;
		de=dat2[0]-48;
		un=dat2[1]-48;
	}else{
		
		ce=0;
		de=0;
		un=dat2[0]-48;
	}
	dat=(100*ce)+(10*de)+un;
	v[1]=dat;					//dato de velocidad de la E/Bike

	if (dat3[2]!=0){
		ce=dat3[0]-48;
		de=dat3[1]-48;
		un=dat3[2]-48;
	}
	else if (dat3[1]!=0){
		ce=0;
		de=dat3[0]-48;
		un=dat3[1]-48;
	}else{
		
		ce=0;
		de=0;
		un=dat3[0]-48;
	}
	dat=(100*ce)+(10*de)+un;
	v[2]=dat;					//dato de nivel de asistencia
}

int main(){
	FILE *fp;
	int n;
	int cor;
	int cmm;
	int z;
	int band;
	int crchr;
	int lf;
	char com [3000];
	char re [500];
	//int y;
	//int lr;
	unsigned char caracter;
	char cpc[3];		//dato de porcentaje de carga en caracteres
	char cvl[3];		//dato de velocidad en caracteres
	char cas[3];		//dato de nivel de asistencia en caracteres
	strcpy(com, "curl -X GET \"https://my.rayven.io:8082/api/main?uid=111848e7eda9ff3b47e3aba02197e37a6a94&deviceid=conf_data\" >/home/alberto/Documents/ecob/UNO/r1.txt");
	system(com);
	fp = fopen ("/home/alberto/Documents/ecob/UNO/r1.txt","r");
	n=0;
	cor =0;
	z=0;
	cmm=0;
	crchr=0;
	fseek(fp, 0L, SEEK_END);
	lf=ftell(fp);
	printf("\n Longitud del archivo : %i \n", lf);
	rewind(fp);
	while(((caracter = fgetc(fp)) != EOF)&& n<200)
	{		
		re[n]=caracter;		
		//caracter = fgetc(fp);
		printf("%c",caracter);
		
		n++;
	}

	printf("\nre = %s\n", re);
	while(crchr<200)
	{		
		
		printf("\ncrchr : %i\n", crchr);
		caracter= re[crchr];
		printf("%c",caracter);
		if (caracter=='{'){
			cor++;
		}
		if (cor==2){
			if (caracter!=','){
				if (caracter=='0'||caracter=='1'||caracter=='2'||caracter=='3'||caracter=='4'||caracter=='5'||
					caracter=='6'||caracter=='7'||caracter=='8'||caracter=='9'){
					if (cmm==0){
						cpc[z]=caracter;
					}
					if (cmm==1){
						cvl[z]=caracter;
					}
					if (cmm==2){
						cas[z]=caracter;
					}
					z++;
				}
				
			}else{
				cmm++;
				z=0;
			}
			
		}
		crchr++;
	}
	cas[2]=0;
	cas[1]=0;
	cvl[2]=0;
	cpc[2]=0;
	printf("\n Las cadenas recolectadas son : %s , %s , %s \n", cpc, cvl, cas);
	transform(cpc, cvl,  cas),
	//getdata(ci);
	printf("\nLos valores del json son: %i, %i, %i \n", v[0], v[1], v[2]);
	//strcpy (com, "");
}
