#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>   /* ioctl() */
#include <unistd.h> 	//para funcion sleep()  ...delay
#include <string.h>
#include <json/json.h>

int v[3];
int x=0;

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

int main(){
	char com[250];
	char res[200];
	char ci [100];
	strcpy(com,"");
	strcpy(res,"");
	strcpy(ci,"");
	char t[3];
	int n=0;
	int z=0;
	int num;
	int lt, lr;
	FILE *fp;
	char caracter;
	int y;
	
	
	strcpy(com, "curl -X GET \"https://my.rayven.io:8082/api/main?uid=111848e7eda9ff3b47e3aba02197e37a6a94&deviceid=conf_data\" >/home/alberto/Documents/rayven/response.txt");
	printf("\n\n\nejecuto post %d : %s \n", n, com);
	lt= strlen(com);
	printf("\n la longitud del comando es : %d\n\n", lt);
	system (com);
	fp = fopen ("response.txt","r");
	//fgets(res, 100, fp);
	while((caracter = fgetc(fp)) != EOF)
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
	}
	ci[z]='\0';
	z++;	
	printf("\n\n n= %d \n", n); 
	printf("\n la cadena recibida es: %s", ci);
	fclose(fp);
	lr=strlen(ci);
	printf("\n la longitud de la cadena recibida ci es : %d", lr);
	printf("\n la cadena recibida ci es: %s", ci);
	n++;
	json_object * jobj = json_tokener_parse(ci);     
	json_parse(jobj);
	printf("\nLos valores del json son: %i, %i\n", v[0], v[1]);
	sleep(2);
	printf ("\n\n");
}
