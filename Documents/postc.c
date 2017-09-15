#include <stdio.h>

#include <stdlib.h>

#include <sys/ioctl.h>   /* ioctl() */

#include <unistd.h> 	//para funcion sleep()  ...delay

#include <string.h>



char js[2500];

int control[37][5];		//tabla donde se almacenan los datos de las Lock Controller

int fuente[8][3];		//tabla donde se almacenan los datos de las fuentes

int slots, b, st;

int bike [401];

int bd[6];

float lat, lon;

int bilckd=0, biroad=0;



void vac_bike(){

	int g=0;

	while (g<401){

		bike[g]=0;

		g++;

	}

}



void def_table1(int n){

	int z,y;

	int num;

	int w;

	int d;

	int r;

	for (z=1;z<=n;z++){

		r=rand()%2;

		d=rand()%401;

		if (r!=1)

		{			

			if (bike[d]==0){

				bilckd++;

				bike[d]=1;

				for (y=1;y<5;y++){

					w=rand()%100;

					if (y==4){

						control [z][y]=d;

					}

					else{

						control[z][y]=w;

					}

				}

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

	int r;

	for (z=1;z<=n;z++){

		r=rand()%25;

		if (r==1)

		{			

			fuente[z][1]=rand()%2;

			fuente[z][2]=rand()%2;

		}else {

			fuente[z][1]=0;

			fuente[z][2]=0;

		}

	}

	r=rand()%4;

	if (r==1)

	{			

		fuente[8][2]=rand()%2;

	}

}



void getgval(int nc, int nf){

	int s=1;

	slots=0;

	st=0;

	while (s<=nc){

		if (control[s][4]==0){

			slots++;				//cuando una lock no tiene e-bike conectada

		}

		s++;

	}

	s=1;

	while (s<=nf){

		if(fuente[s][1]==1||fuente[s][2]==1){

			st++;

		}

		s++;

	}

	b=nc-slots;

}





void to_jsonc(int id_st){

	char t [5];

	int nf=4;

	int nc=34;

	int q;

	//char rfid[17];

	int k=0;

	strcpy(js,"");

	strcpy(js,"{\"id_st\":");

	sprintf(t,"%d",id_st);

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

	if (fuente[8][2]==0){

		strcat(js, ", \"door\":\"closed\"");

	}else{

		strcat(js, ", \"door\":\"opened\"");

	}

	strcat(js, ", \"chargers\":[");

	for(q=1;q<=nf;q++){

		strcat(js,"{\"id_f\":");

		sprintf(t,"%d",fuente[q][0]);			//id_f se encuentra en las posiciones fuentes[0][q] de fuente

		strcat(js, t);

		strcat(js,", \"temperature\":");

		sprintf(t,"%d",fuente[q][2]);			//t se encuentra en las posiciones fuetes[q][2] de fuente

		strcat(js, t);

		strcat(js,", \"status\":");

		sprintf(t,"%d",fuente[q][1]);			//ok se encuentra en las posiciones fuentes[q][1] de fuente

		strcat(js, t);

		if (q<nf){

			strcat(js, "},");

		}

		else {

			strcat(js, "}],");

		}

	}



	strcat(js, "\"locks\":[");



	for(q=1;q<=nc;q++){



		/*while (k<16){

			rfid[k]=brfid[q][k];

			k++;

		}

		rfid[16]='\0';*/

		k=0;



		strcat(js,"{\"id_c\":");

		sprintf(t,"%d",control[q][0]);			//id_c se encuentra en las posiciones control[q][0] de control

		strcat(js, t);

		strcat(js,", \"id_b\":\"bike_");

		sprintf(t,"%d",control[q][4]);			//id_b se encuentra en las posiciones control[q][4] de control

		strcat(js, t);

		strcat(js,"\", \"st_ch\":");

		sprintf(t,"%d",control[q][1]);			//st_ch se encuentra en las posiciones control[q][1] de control

		strcat(js, t);

		strcat(js,", \"hs_ba\":");

		sprintf(t,"%d",control[q][2]);			//hs_ba se encuentra en las posiciones control[q][2] de control

		strcat(js, t);

		strcat(js,", \"nc\":");

		sprintf(t,"%d",control[q][3]);			//nc se encuentra en las posiciones control[q][3] de control

		strcat(js, t);

		if (q<nc){

			strcat(js, "},");

		}

		else {

			strcat(js, "}]");

		}

	}

	strcat(js, "}");

}



void to_jsoncb(){

	char t [15];

	int q;

	int l;

	strcpy(js,"");

	strcpy(js,"{\"Id_b\":\"bike_");

	sprintf(t,"%d",bd[0]);

	strcat(js, t);

	strcat(js, "\", \"lat\":");

	sprintf(t,"%f",lat);

	strcat(js, t);

	strcat(js, ", \"lon\":");

	sprintf(t,"%f",lon);

	strcat(js, t);

	//printf("\n desp de convertir lon \n");

	strcat(js,", \"st_ch\":");

	sprintf(t,"%d",bd[3]);

	strcat(js,t);

	strcat(js,", \"lckd\":");

	sprintf(t,"%d",bd[4]);

	strcat(js,t);

	strcat(js,", \"lo_ch\":");

	sprintf(t,"%d",bd[5]);

	strcat(js,t);

	strcat(js, "}");

	

}

void def_tableb(int ib){

	int rp;

	biroad++;

	lat= 19.045773;

	lon=-99.373112;

	float l;

	l=rand()%556344;

	l=l/1000000;

	lat= lat+l;

	l=rand()%43621;

	l=l/100000;

	lon= lon+l;

	//printf("\nlat = %f \n", lat);

	//printf("\nlon = %f \n", lon);

	bd[0]=ib;

	bd[3]=rand()%101;

	rp=rand()%5;

	if (rp==1){

		bd[4]=rand()%2;

	}else{

		bd[4]=0;

	}

	if (bd[3]<30){

		bd[5]=1;	

	}

	else{

		bd[5]=0;	

	}	

	//bd[5]=rand()%2;

}



int main(){

	srand(time(NULL));

	vac_bike();

	int btot;

	char com[3000];

	//char res[2500];

	char t[4];

	int is;

	strcpy(com,"");

	//strcpy(res,"");

	int nc=36;

	int nf=6;

	int n=1;

	int lt, lr;

	FILE *fp;

	while(1){

		vac_bike();

		btot=0;

		biroad=0;

		bilckd=0;	

		while (n<61){

			def_table(nc);

			df_tabl(nf);

			def_table2(nf);

			def_table1(nc);

			getgval(nc, nf);

			is = n;

			

			

			to_jsonc(is);

			strcpy(com, "curl -X POST -H \"Content-Type: application/json\" -d '");

			strcat(com, js);

			strcat(com, "' \"https://my.rayven.io:8082/api/main?uid=111848e7eda9ff3b47e3aba02197e37a6a94&deviceid=st_");

			sprintf(t,"%d",is);

			strcat(com, t);

			strcat(com, "\"");

			printf("\n\n\nejecuto post %d : %s \n", n, com);

			lt= strlen(com);

			printf("\n la longitud del comando es : %d", lt);

			system (com);

			

			n++;

			usleep(50000);

		}

		n=1;

		while (n<401){

			//printf("\nn = %d \n", n);

			if (bike[n]==0){



				def_tableb(n);

				to_jsoncb();



				strcpy(com, "curl -X POST -H \"Content-Type: application/json\" -d '");

				strcat(com, js);

				strcat(com, "' \"https://my.rayven.io:8082/api/main?uid=11186e7efa59e8f64e479009692c22d0ed72&deviceid=bike_");

				sprintf(t,"%d",n);

				strcat(com, t);

				strcat(com, "\"");

				printf("\n\n\nejecuto post %d : %s \n", n, com);

				lt= strlen(com);

				printf("\n la longitud del comando es : %d", lt);

				system (com);

				usleep(50000);

			}

			n++;

		}

		n=1;

		printf ("\nE-Bikes on the road = %d \n", biroad);

		printf("\nE-Bikes locked in a station = %d \n", bilckd);

		btot=biroad+bilckd;

		printf("\nE-Bikes total = %d \n", btot);

	}

	

}
