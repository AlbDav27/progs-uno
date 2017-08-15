#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>   /* ioctl() */
#include <unistd.h> 	//para funcion sleep()  ...delay
#include <string.h>

int main(){
	char com[100];
	char lin[100];
	char res[100];
	char caracter;
	int n=0;
	FILE *fp;

	system ("/usr/advantech/utility/3g-module/3g_ublox.sh >/home/root/progs-uno/Documents/cn.txt");
	system ("cat /home/root/progs-uno/Documents/cn.txt >/home/root/progs-uno/Documents/cnf.txt");

	fp = fopen ("cnf.txt","r");

	
	while (n<44){
		fgets(lin, 100, fp);		
		printf("\nn= %d , %s\n", n, lin);
		if (n==42){
			strcpy (res, lin);
		}
		n++;
	}
	fclose(fp);
	printf("\nLa cadena de DNS: %s\n", res);
}
