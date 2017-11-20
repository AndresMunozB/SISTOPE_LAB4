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
BMP* funcion(BMP* imagen, int modo, int cantidad_pixeles, int iteraciones);
void print_imagen(BMP *imagen);

#endif