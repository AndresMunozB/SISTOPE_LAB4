/*---------------------------*/
/*  Ejemplo en C nº 60:      */
/*  C060.C                   */
/*                           */
/*  Información sobre un     */ 
/*  fichero BMP (2)          */
/*                           */
/*  Curso de C,              */
/*    Nacho Cabanes          */
/*---------------------------*/
 
#include <stdio.h>

int main(){
  char nombre[60] = "vicunas.bmp";
  FILE* fichero;
  int compresion, ancho, alto;
  char marca1, marca2;
 
  puts("Comprobador de imágenes BMP\n");
  printf("Dime el nombre del fichero: ");
  //gets(nombre);
  fichero = fopen(nombre, "rb");
  if (fichero==NULL)
    puts("No encontrado\n");
  else {
    marca1 = fgetc(fichero);  /* Leo los dos primeros bytes */
    marca2 = fgetc(fichero);
    if ((marca1 =='B') && (marca2 =='M')) { /* Si son BM */
        printf("Marca del fichero: %c%c\n", marca1, marca2);
        fseek(fichero, 18, SEEK_SET);    /* Posición 18: ancho */
        fread(&ancho, 1, 4, fichero);
        printf("Ancho: %d\n", ancho);    
        fread(&alto, 1, 4, fichero);     /* Siguiente dato: alto */
        printf("Alto: %d\n", alto);    
        fseek(fichero, 4, SEEK_CUR);     /* 4 bytes después: compresión */
        fread(&compresion, 1, 4, fichero);
        fclose(fichero);
        switch (compresion) {
          case 0: puts("Sin compresión"); break;
          case 1: puts("Compresión RLE 8 bits"); break;
          case 2: puts("Compresión RLE 4 bits"); break;
          }
    } else
      printf("No parece un fichero BMP\n");  /* Si la marca no es BM */
  }
 
  return 0;
}