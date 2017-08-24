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
	int z=0;
	int u=0;
	int lt;
	FILE *fp;

	printf("\nantes de ejecutar 3g_ublox\n");
	system ("/usr/advantech/utility/3g-module/3g_ublox.sh >/home/com/cnf.txt");
	//printf("\nproblema en la conf de DNS\n");
	sleep(8);
	strcpy(com, "");
	strcpy(lin, "");
	strcpy(ipd, "");
	system ("sed '1,40d' /home/com/cnf.txt >/home/com/cn.txt");

	fp = fopen ("/home/com/cn.txt","r");

	printf("\n");
	fgets(lin, 100, fp);
	while (z==0&&u<100){
		if (lin[0]=='p'){
			z=1;
			u=0;
		}else{
			fgets(lin, 100, fp);
			u++;
		}
	}
	fclose(fp);
	if (u==0){
		printf("La linea que leo del archivo es: %s  //", lin);
		
		lt = strlen(lin);
		printf("\n la longitud de la cadena completa es: %d\n", lt);
		while (n<lt){
			c=lin[n];
			if (c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||c=='6'||c=='7'||c=='8'||c=='9'||c=='0'||c=='.'){
				ipd[x]=c;
				x++;
			}
			n++;
		}
		ipd[x]='\0';
		lt = strlen(ipd);
		printf("\n la dir ip del ser dns es: %s\n", ipd);
		printf("\n la long de la dir ip: %d\n", lt);
		strcpy(com, "echo nameserver ");
		strcat(com, ipd);
		strcat (com, " >/etc/resolv.conf");
		system(com);
		printf("\n El comando para agregar el serv DNS es: %s\n", com);
	}else{
		printf("\nerror al leer archivo de DNS\n");
	}
	//system ("rm /home/com/cnf.txt");
	//system ("rm /home/com/cn.txt");
	//system ("ping google.com");
}
