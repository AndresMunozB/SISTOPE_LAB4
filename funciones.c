#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

void abrir_imagen(BMP *imagen, char *ruta)
{
    FILE *archivo;  //Puntero FILE para el archivo de imÃ¡gen a abrir
    int i,j;
    
    //Abrir el archivo de imÃ¡gen
    archivo = fopen( ruta, "rb+" );
    if(!archivo)
    { 
        //Si la imÃ¡gen no se encuentra en la ruta dada
        printf( "La imÃ¡gen %s no se encontro\n",ruta);
        exit(1);
    }

    //Leer la cabecera de la imagen y almacenarla en la estructura a la que apunta imagen

    fseek( archivo,0, SEEK_SET);
    fread(&imagen->bm,sizeof(char),2, archivo);
    fread(&imagen->tamano,sizeof(int),1, archivo);
    fread(&imagen->reservado,sizeof(int),1, archivo);   
    fread(&imagen->offset,sizeof(int),1, archivo);  
    fread(&imagen->tamanoMetadatos,sizeof(int),1, archivo); 
    fread(&imagen->alto,sizeof(int),1, archivo);    
    fread(&imagen->ancho,sizeof(int),1, archivo);   
    fread(&imagen->numeroPlanos,sizeof(short int),1, archivo);  
    fread(&imagen->profundidadColor,sizeof(short int),1, archivo);  
    fread(&imagen->tipoCompresion,sizeof(int),1, archivo);
    fread(&imagen->tamanoEstructura,sizeof(int),1, archivo);
    fread(&imagen->pxmh,sizeof(int),1, archivo);
    fread(&imagen->pxmv,sizeof(int),1, archivo);
    fread(&imagen->coloresUsados,sizeof(int),1, archivo);
    fread(&imagen->coloresImportantes,sizeof(int),1, archivo);  


    //Validar ciertos datos de la cabecera de la imÃ¡gen    
    if (imagen->bm[0]!='B'||imagen->bm[1]!='M') 
    {
        printf ("La imagen debe ser un bitmap.\n"); 
        exit(1);
    }
    /*if (imagen->profundidadColor!= 24) 
    {
        printf ("La imagen debe ser de 24 bits.\n"); 
        exit(1);
    }*/

    //Reservar memoria para el arreglo que tendra la imÃ¡gen en escala de grises (Arreglo de tamaÃ±o "img.ancho X img.alto")
    imagen->pixel=(Pixel**) malloc (imagen->alto* sizeof(Pixel*)); 
    for( i=0; i<imagen->alto; i++)
        imagen->pixel[i]=(Pixel*) malloc (imagen->ancho* sizeof(Pixel)); 
    
    //Pasar la imÃ¡gen a el arreglo reservado en escala de grises
    //unsigned char R,B,G;
    for (i=0;i<imagen->alto;i++)
    {
        for (j=0;j<imagen->ancho;j++)
        {  
            printf("ftell:%li\n",ftell(archivo) );
            printf("traductor: %li\n",traductor(i,j,imagen->ancho,imagen->alto,imagen->offset));
            fread(&imagen->pixel[i][j].B,sizeof(char),1, archivo);  //Byte Blue del pixel
            fread(&imagen->pixel[i][j].G,sizeof(char),1, archivo);  //Byte Green del pixel
            fread(&imagen->pixel[i][j].R,sizeof(char),1, archivo);  //Byte Red del pixel
            //printf("offset: %d\n",imagen->offset );
            //printf("%d,%c ", R);
            //ConversiÃ³n a escala de grises
            //imagen->pixel[i][j]=(unsigned char)((R*0.3)+(G*0.59)+ (B*0.11));  //Formula correcta
            //imagen->pixel[i][j]=(B+G+R)/3;                              //Forma simple (Promedio)
        }   
    }
    //Cerrrar el archivo
    fclose(archivo);    
}

long int traductor(int i, int j, int ancho, int alto,int offset){
    long int pixel_offset = sizeof(char) * 3;
    long int result = (pixel_offset*((i*ancho)+j)) + offset;
    return result;
}

void crear_imagen(BMP *imagen, char ruta[])
{
    FILE *archivo;  //Puntero FILE para el archivo de imÃ¡gen a abrir

    int i,j;

    //Abrir el archivo de imÃ¡gen
    archivo = fopen( ruta, "wb+" );
    if(!archivo)
    { 
        //Si la imÃ¡gen no se encuentra en la ruta dada
        printf( "La imÃ¡gen %s no se pudo crear\n",ruta);
        exit(1);
    }
    
    //Escribir la cabecera de la imagen en el archivo
    fseek( archivo,0, SEEK_SET);
    fwrite(&imagen->bm,sizeof(char),2, archivo);
    fwrite(&imagen->tamano,sizeof(int),1, archivo); 
    fwrite(&imagen->reservado,sizeof(int),1, archivo);  
    fwrite(&imagen->offset,sizeof(int),1, archivo); 
    fwrite(&imagen->tamanoMetadatos,sizeof(int),1, archivo);    
    fwrite(&imagen->alto,sizeof(int),1, archivo);   
    fwrite(&imagen->ancho,sizeof(int),1, archivo);  
    fwrite(&imagen->numeroPlanos,sizeof(short int),1, archivo); 
    fwrite(&imagen->profundidadColor,sizeof(short int),1, archivo); 
    fwrite(&imagen->tipoCompresion,sizeof(int),1, archivo);
    fwrite(&imagen->tamanoEstructura,sizeof(int),1, archivo);
    fwrite(&imagen->pxmh,sizeof(int),1, archivo);
    fwrite(&imagen->pxmv,sizeof(int),1, archivo);
    fwrite(&imagen->coloresUsados,sizeof(int),1, archivo);
    fwrite(&imagen->coloresImportantes,sizeof(int),1, archivo); 
            
    //Pasar la imÃ¡gen del arreglo reservado en escala de grises a el archivo (Deben escribirse los valores BGR)
    for (i=0;i<imagen->alto;i++)
    {
        for (j=0;j<imagen->ancho;j++)
        {  
            //Ecribir los 3 bytes BGR al archivo BMP, en este caso todos se igualan al mismo valor (Valor del pixel en la matriz de la estructura imagen)
            fwrite(&imagen->pixel[i][j].B,sizeof(char),1, archivo);  //Escribir el Byte Blue del pixel 
            fwrite(&imagen->pixel[i][j].G,sizeof(char),1, archivo);  //Escribir el Byte Green del pixel
            fwrite(&imagen->pixel[i][j].R,sizeof(char),1, archivo);  //Escribir el Byte Red del pixel
        }   
    }
    //Cerrrar el archivo
    fclose(archivo);
}