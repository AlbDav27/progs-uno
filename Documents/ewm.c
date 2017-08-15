#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>   /* ioctl() */
#include <unistd.h> 	//para funcion sleep()  ...delay
#include <string.h>

int main(){
	char com[100];
	char lin[100];
	char res[100];
	int n=0;
	FILE *fp;

	system ("cd");
	system ("cd /usr/advantech/utility/3g-module");
	system ("./3g_ublox.sh >/home/cnf.txt");

	fp = fopen ("response.txt","r");

	
	while (n<50){
		fgets(lin,100,fp);
		if (n=27){
			strcpy (res, lin);
		}
	}
	printf("\n%s\n", res);
	
	fclose(fp);

//fgets(res, 100, fp);
	/*while((caracter = fgetc(fp)) != EOF)
	{		
		printf("%c",caracter);
		res[n]=caracter;
		n++;
		
	}
	n=107;
	printf("\n\n la cadena recibida guardadA en res es : ");
	while ((n<164)&&(z<83)){		
		printf("%d-%c  ", n, res[n]);
		y=n%2;
		if (y==0&&n!=106&&n!=160){
			if (res[n]!=92)
			{
				ci[z]=res[n];
				z++;
			}
		}
		n++;
	}*/
}
