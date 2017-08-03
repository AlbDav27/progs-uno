#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>   /* ioctl() */
#include <unistd.h> 	//para funcion sleep()  ...delay
#include <string.h>
//#include <json/json.h>

int main(){
	char com[250];
	char res[200];
	strcpy(com,"");
	strcpy(res,"");
	char t[3];
	int n=1;
	int num;
	int lt, lr;
	FILE *fp;
	while (n<10){
		strcpy(com, "curl -X POST -H \"Content-Type: application/json\" -d '{\"value\":");
		num=rand()%100;
		sprintf(t,"%d",num);
		strcat(com, t);
		strcat(com, "}' http://things.ubidots.com/api/v1.6/variables/5980b617c03f972586d81d43/values?token=A1E-j6ajtC9PV1TyqrCBsx4quttdB8CFt3 >/home/alberto/Documents/cu/response");
		printf("\n\n\nejecuto post %d : %s \n", n, com);
		lt= strlen(com);
		printf("\n la longitud del comando es : %d", lt);
		system (com);
		fp = fopen ("response","r");
		fgets(res, 200, fp);
		fclose(fp);
		lr=strlen(res);
		printf("\n la longitud de la cadena recibida es : %d", lr);
		printf("\n la cadena recibida es: %s", res);
		n++;
		sleep(2);
	}
	printf ("\n\n");
}
