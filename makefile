all:
	gcc -o main main.c funciones.c -Wall
	echo "./main -i cuadro.bmp -s metodo1.bmp -g metodo2.bmp -n 8 -m 3 -o 3 -d"
	
