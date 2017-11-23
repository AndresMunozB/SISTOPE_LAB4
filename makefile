all:
	gcc -o main main.c funciones.c -Wall
	echo "./main -i cuadro.bmp -s salida1.txt -g salida2.txt -n 8 -m 13 -o 1 -d"
	
