#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

int main(int argc,char** argv){

	Matriz* matriz = matriz_create(10,10);
	matriz_show(matriz);
	matriz_destroy(matriz);
	return 1;
}
