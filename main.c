#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <string.h>
#include "funciones.h"


#define IMAGEN_TRATADA "tratada.bmp"

int main(int argc,char** argv){

	Clock clock;
	clock_start(&clock);

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
	
	strcpy(IMAGEN,ivalue);
	printf("hola\n");
	abrir_imagen(&img,IMAGEN,ovalue);
	printf("chao\n");
	print_imagen(&img);
	reduce_imagen(&img,ovalue,mvalue,nvalue,&imgRed);
	print_imagen(&imgRed);
	
	/*if(dflag == 1){
		//printf("Tiempo de ejecucion: %d seg.\n", seconds);
	}*/
	
	crear_imagen(&imgRed,IMAGEN_TRATADA);
	clock_end(&clock);
	clock_print(&clock);


	return 0;
}
