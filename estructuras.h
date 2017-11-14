#ifndef _STRUCTS_H_
#define _STRUCTS_H_

typedef struct pixel
{
    unsigned char R;
    unsigned char G;
    unsigned char B;

}Pixel;


//Estructura para almacenar la cabecera de la imagen BMP y un apuntador a la matriz de pixeles
typedef struct BMP
{
    char bm[2];                 //(2 Bytes) BM (Tipo de archivo)
    int tamano;                 //(4 Bytes) TamaÃ±o del archivo en bytes
    int reservado;                  //(4 Bytes) Reservado
    int offset;                     //(4 Bytes) offset, distancia en bytes entre la img y los pÃ­xeles
    int tamanoMetadatos;            //(4 Bytes) TamaÃ±o de Metadatos (tamaÃ±o de esta estructura = 40)
    int alto;                       //(4 Bytes) Ancho (numero de pÃ­xeles horizontales)
    int ancho;                  //(4 Bytes) Alto (numero de pixeles verticales)
    short int numeroPlanos;         //(2 Bytes) Numero de planos de color
    short int profundidadColor;     //(2 Bytes) Profundidad de color (debe ser 24 para nuestro caso)
    int tipoCompresion;             //(4 Bytes) Tipo de compresiÃ³n (Vale 0, ya que el bmp es descomprimido)
    int tamanoEstructura;           //(4 Bytes) TamaÃ±o de la estructura Imagen (Paleta)
    int pxmh;                   //(4 Bytes) PÃ­xeles por metro horizontal
    int pxmv;                   //(4 Bytes) PÃ­xeles por metro vertical
    int coloresUsados;              //(4 Bytes) Cantidad de colores usados 
    int coloresImportantes;         //(4 Bytes) Cantidad de colores importantes
    Pixel** pixel;          //Puntero a una tabla dinamica de caracteres de 2 dimenciones almacenara el valor del pixel en escala de gris (0-255)

}BMP;




#endif