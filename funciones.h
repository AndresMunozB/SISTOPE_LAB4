#ifndef _FUNCIONES_H
#define _FUNCIONES_H
#include "estructuras.h"

//ENTRADA: Enteros n y m, que representan las filas y columnas respectivamente
//SALIDA: Puntero a la estructura Matriz
//Esta función se encarga de crear una nueva matriz e inicializar cada posicion con caracter espacio.
Matriz* matriz_create(int n,int m){
    Matriz* matriz = (Matriz*) malloc(sizeof(Matriz));
    matriz->row = n;
    matriz->col = m;
    matriz->data = (int**) malloc (sizeof(int*)*n);
    int i,j;
    for(i=0;i<n;i++){
        matriz->data[i] = (int*) malloc (sizeof(int)*m);
        for (j=0;j<m;j++){
        	matriz->data[i][j] = 0;
        }
    }
    return matriz;
}

//ENTRADA: Puntero a la estructura Matriz
//SALIDA: Void (Nada)
//Función se encarga de liberar la memoria utilizada por la matriz.
void matriz_destroy(Matriz* matriz){
    int i;  
    for(i=0;i<matriz->row;i++){
        free(matriz->data[i]);
    }
    free(matriz->data);
    free(matriz);
}

//ENTRADA: Puntero a la estructura Matriz
//SALIDA: Void (Nada)
//Función al cual le corresponde mostrar los datos de la matriz por consola
void matriz_show(Matriz *matriz){
    int i,j;
    for (i=0;i<matriz->row;i++){
        for(j=0;j<matriz->col;j++){
            printf("%02d ",matriz->data[i][j]);
        }
        printf("\n");
    }
}


#endif