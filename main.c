#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "funciones.h"


#define IMAGEN_TRATADA "tratada.bmp"

int main(int argc,char** argv){

	//Clock clock;
	//clock_start(&clock);

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

    Clock clock_1;
    Clock clock_2;
    Clock clock_3;

	BMP original;
	BMP imagen_fila;
	BMP imagen_columna;
	//printf("nvalue:%d\n",nvalue );
	if (ovalue == 1){
		clock_start(&clock_1);
		abrir_imagen(&original,ivalue,ovalue);
		print_imagen(&original);
		reduce_imagen(&original,ovalue,mvalue,nvalue,&imagen_fila);
		crear_imagen(&imagen_fila,svalue);
		clock_print(&clock_1,1);
		destruir_imagen(&original);
		destruir_imagen(&imagen_fila);

	}
	else if(ovalue == 2){
		clock_start(&clock_2);
		abrir_imagen(&original,ivalue,ovalue);
		reduce_imagen(&original,ovalue,mvalue,nvalue,&imagen_columna);
		crear_imagen(&imagen_columna,gvalue);
		clock_print(&clock_2,2);
		destruir_imagen(&original);
		destruir_imagen(&imagen_columna);
	}
	else if(ovalue == 3){
		clock_start(&clock_3);

		clock_start(&clock_1);
		abrir_imagen(&original,ivalue,1);
		reduce_imagen(&original,1,mvalue,nvalue,&imagen_fila);
		crear_imagen(&imagen_fila,svalue);
		clock_print(&clock_1,1);
		destruir_imagen(&original);
		destruir_imagen(&imagen_fila);

		clock_start(&clock_2);
		abrir_imagen(&original,ivalue,2);
		reduce_imagen(&original,2,mvalue,nvalue,&imagen_columna);
		crear_imagen(&imagen_columna,gvalue);
		clock_print(&clock_2,2);
		destruir_imagen(&original);
		destruir_imagen(&imagen_columna);
		clock_end(&clock_3);
		clock_print(&clock_3,3);
	}



	return 0;
}
