#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>   /* ioctl() */
#include <unistd.h> 	//para funcion sleep()  ...delay
#include <string.h>

char js[2500];
int control[37][5];		//tabla donde se almacenan los datos de las Lock Controller
int fuente[8][3];		//tabla donde se almacenan los datos de las fuentes
int slots, b, st;

void def_table1(int n){
	int z,y;
	int num;
	int r;
	for (y=1; y<=n; y++){
		control[y][0]=y;
		r=rand()%7;						//este numero es para que no se publique todas las Lock controller
	}
	if (r==1){
		for (z=1;z<5;z++){
			for (y=1;y<37;y++){
				
				control[y][z]=rand()%100;
			}

		}
	}
	
}
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

void def_table2(int n){
	int z,y;
	int num;
	for (y=1; y<=n; y++){
		fuente[y][0]=y;
	}
	for (z=1;z<3;z++){
		for (y=1;y<7;y++){
			fuente[y][z]=rand()%1;
		}

	}
}

void getgval(){
	int s=1;
	slots=0;
	st=0;
	while (s<=36){
		if (control[s][4]==0){
			slots++;				//cuando una lock no tiene e-bike conectada
		}
	}
	s=1;
	while (s<=6){
		if(fuente[s][1]==1||fuente[s][2]==1){
			st++;
		}
	}
	b=36-slots;
}


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

int main(){
	char com[3000];
	char res[2500];
	char t[4];
	int is;
	is = rand()%60;
	strcpy(com,"");
	strcpy(res,"");
	int nc=36;
	int nf=6;
	def_table(nc);
	df_tabl(nf);
	def_table2(nf);
	def_table1(nc);
	to_jsonc();
	int n=1;
	int lt, lr;
	FILE *fp;
	while (n<5){
		strcpy(com, "curl -X POST -H \"Content-Type: application/json\" -d '");
		strcat(com, js);
		strcat(com, "' https://my.rayven.io:8082/api/main?uid=111848e7eda9ff3b47e3aba02197e37a6a94&deviceid=st_");
		sprintf(t,"%d",is);
		strcat(com, " >/Users/albertodaniel/Desktop");
		printf("\n\n\nejecuto post %d : %s \n", n, com);
		lt= strlen(com);
		printf("\n la longitud del comando es : %d", lt);
		system (com);
		fp = fopen ("response","r");
		fgets(res, 2500, fp);
		fclose(fp);
		lr=strlen(res);
		printf("\n la longitud de la cadena recibida es : %d", lr);
		printf("\n la cadena recibida es: %s", res);
		n++;
		sleep(60);
	}
	printf ("\n\n");
}
