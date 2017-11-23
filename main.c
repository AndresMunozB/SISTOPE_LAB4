#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include "funciones.h"


#define IMAGEN_TRATADA "tratada.bmp"

int main(int argc,char** argv){

	/*  -i: nombre archivo de entrada
		-s: nombre archivo de salida método 1
		-g: nombre archivo de salida método 2
		-n: cantidad de iteraciones a reducir la imagen
		-m: cantidad de pixeles a promediar
		-o: método a utilizar (1,2 o 3)
		-d: bandera que muestra resultados de tiempo de ejecución.
	*/

	char ivalue[300];
	char svalue[300];
	char gvalue[300];
	int nvalue,mvalue,ovalue,dflag;
	if(!opt_get(argc,argv,ivalue,svalue,gvalue,&nvalue,&mvalue,&ovalue,&dflag)){
        return 0;  
	}
   
    if(verifyArguments(ivalue,svalue,gvalue,nvalue,mvalue,ovalue) == 0) {
        return 0;
    }

	
	BMP img;	
	BMP imgRed;			
	char IMAGEN[45];
	/*
	time_t startTime, endTime;
	unsigned int seconds;
	startTime = time(NULL);*/

	strcpy(IMAGEN,ivalue);
	abrir_imagen(&img,IMAGEN,1);
	
	copiar_bitmap(&img,&imgRed);
	//printf("%c%c\n",imgRed.bm[0],imgRed.bm[1] );
	print_imagen(&imgRed);
	init_new_imagen(&imgRed,3,2);
	printf("\n" );
	print_imagen(&imgRed);
	/*reduce_imagen(&img,ovalue,mvalue,nvalue,&imgRed);
	
	endTime = time(NULL);
	seconds = endTime - startTime;
	if(dflag == 1)
		printf("Tiempo de ejecucion: %d seg.\n", seconds);
	
	crear_imagen(&imgRed,IMAGEN_TRATADA);*/
	exit (0);
}
