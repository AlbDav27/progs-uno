#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>   /* ioctl() */
#include <unistd.h> 	//para funcion sleep()  ...delay
#include <string.h>

int main(){
	char com[100];
	char lin[50];
	char ipd[20];
	char c;
	int n=0;
	int x=0;
	int lt;
	FILE *fp;

	system ("/usr/advantech/utility/3g-module/3g_ublox.sh >/home/root/progs-uno/Documents/cnf.txt");
	sleep(7);
	strcpy(com, "");
	strcpy(lin, "");
	strcpy(ipd, "");
	system ("sed '1,41d' /home/root/progs-uno/Documents/cnf.txt >/home/root/progs-uno/Documents/cn.txt");

	fp = fopen ("cn.txt","r");

	printf("\n");
	fgets(lin, 100, fp);
	printf(" %s  //", lin);
	fclose(fp);
	lt = strlen(lin);
	printf("\n la longitud de la cadena completa es: %d\n", lt);
	while (n<lt){
		c=lin[n];
		if (c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||c=='6'||c=='7'||c=='8'||c=='9'||c=='.'){
			ipd[x]=c;
			x++;
		}
		n++;
	}
	lt = strlen(ipd);
	printf("\n la dir ip del ser dns es: %s\n", ipd);
	printf("\n la long de la dir ip: %d\n", lt);
}
