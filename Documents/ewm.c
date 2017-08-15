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

	system ("/usr/advantech/utility/3g-module/3g_ublox.sh >/home/root/progs-uno/Documents/cnf.txt");
	system ("sed '1,16d' cnf.txt >/home/root/progs-uno/Documents/cn.txt")

	fp = fopen ("cn.txt","r");

	printf("\n");
	while ((fgets(lin, 100, fp))!= EOF){
				
		printf("n= %d , %c  //", n, caracter);
		n++;
	}
	fclose(fp);
	printf("\nLa cadena de DNS: %s\n", res);
}
