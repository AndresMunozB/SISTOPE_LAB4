#ifndef _FUNCIONES_H
#define _FUNCIONES_H
#include "estructuras.h"

void abrir_imagen(BMP *imagen, char *ruta);
void crear_imagen(BMP *imagen, char ruta[]);
long int traductor(int i, int j, int ancho,int offset);
void cargar_pixel(int i,int j,FILE* archivo,BMP* imagen);

#endif