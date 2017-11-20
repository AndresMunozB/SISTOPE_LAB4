#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include "funciones.h"


#define IMAGEN_TRATADA "tratada.bmp"

int main(int argc,char** argv){

	char ivalue[300];
	char svalue[300];
	char gvalue[300];
	int nvalue,mvalue,ovalue,dflag;
	if(!opt_get(argc,argv,ivalue,svalue,gvalue,&nvalue,&mvalue,&ovalue,&dflag))
        return 0;  
   
    if(verifyArguments(ivalue,svalue,gvalue,nvalue,mvalue,ovalue) == 0) 
        return 0;

	//int i,j; 				//Variables auxiliares para loops
	BMP img;				//Estructura de tipo imÃ¡gen
	char IMAGEN[45];		//AlmacenarÃ¡ la ruta de la imagen

	time_t startTime, endTime;
	unsigned int seconds;
	startTime = time(NULL);

	//******************************************************************	
	//Si no se introdujo la ruta de la imagen BMP 
	//******************************************************************	
	//Si no se introduce una ruta de imÃ¡gen
	/*if (argc!=2) 
	{
		printf("\nIndique el nombre del archivo a codificar - Ejemplo: [user@equipo]$ %s imagen.bmp\n",argv[0]);
		exit(1);
	} 
	//Almacenar la ruta de la imÃ¡gen
	strcpy(IMAGEN,argv[1])*/
		
	strcpy(IMAGEN,ivalue);
	//***************************************************************************************************************************
	//0 Abrir la imÃ¡gen BMP de 24 bits, almacenar su cabecera en la estructura img y colocar sus pixeles en el arreglo img.pixel[][]
	//***************************************************************************************************************************	
	abrir_imagen(&img,IMAGEN,1);
	printf("\n*************************************************************************");
	printf("\nIMAGEN: %s",IMAGEN);
	printf("\n*************************************************************************");
	printf("\nDimensiones de la imÃ¡gen:\tAlto=%d\tAncho=%d\n",img.alto,img.ancho);

	reduce_imagen(&img,ovalue,mvalue,nvalue);
	//*************************************************************
	//1 Tratamiento de los pixeles
	//*************************************************************
	//Imprimir el valor de los pixeles
	/*for (i=0;i<img.alto;i++)
		for (j=0;j<img.ancho;j++);*/
			//img.pixel[i][j]=img.pixel[i][j]-10;
			//printf("%u\t",img.pixel[i][j]);
			//printf("hola\n");
			
	endTime = time(NULL);
	seconds = endTime - startTime;
	printf("Elapsed time (s): %d\n", seconds);
	
	//***************************************************************************************************************************
	//2 Crear la imÃ¡gen BMP a partir del arreglo img.pixel[][]
	//***************************************************************************************************************************	
	crear_imagen(&img,IMAGEN_TRATADA);
	printf("\nImÃ¡gen BMP tratada en el archivo: %s\n",IMAGEN_TRATADA);
	
	//Terminar programa normalmente	
	exit (0);
}
