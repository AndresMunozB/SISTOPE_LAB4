#ifndef _FUNCIONES_H
#define _FUNCIONES_H
#include "estructuras.h"

long int traductor(int i, int j, int ancho,int offset);
void cargar_pixel(int i,int j,FILE* archivo,BMP* imagen);
void cargar_matriz_fila(BMP* imagen,FILE* archivo);
void cargar_matriz_columna(BMP* imagen, FILE* archivo);
void abrir_imagen(BMP *imagen, char *ruta, int modo);
void crear_imagen(BMP *imagen, char ruta[]);
void reducir_imagen_fila(BMP* imagen,int cantidad_pixeles, BMP* imagenNueva);
void reducir_imagen_columna(BMP* imagen,int cantidad_pixeles, BMP* imagenNueva);
void reduce_imagen(BMP* imagen, int modo, int cantidad_pixeles, int iteraciones,BMP* imagenReducida);
void print_imagen(BMP *imagen);
int opt_get(int argc, char** argv, char ivalue[300],char svalue[300],char gvalue[300],int* nvalue, int* mvalue, int* ovalue,int* dvalue);
int verifyArguments(char* ivalue, char* svalue, char* gvalue, int nvalue, int mvalue, int ovalue);
int fileExists(char* nombreArchivo);

#endif