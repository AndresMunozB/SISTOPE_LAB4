#include <stdio.h> #include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "funciones.h"

long int traductor(int i, int j, int ancho,int offset){
    long int pixel_offset = sizeof(char) * 3;
    long int result = (pixel_offset*((i*ancho)+j)) + offset;
    return result;
}

void cargar_pixel(int i,int j,FILE* archivo,BMP* imagen){
    long int posicion = traductor(i,j,imagen->ancho,imagen->offset);
    fseek(archivo,posicion,SEEK_SET);
    fread(&imagen->pixel[i][j].B,sizeof(char),1, archivo);  //Byte Blue del pixel
    fread(&imagen->pixel[i][j].G,sizeof(char),1, archivo);  //Byte Green del pixel
    fread(&imagen->pixel[i][j].R,sizeof(char),1, archivo);  //Byte Red del pixel
}

void cargar_matriz_fila(BMP* imagen,FILE* archivo){
    int i=0;
    int j=0;
    while(i<imagen->alto){
       
        
        if(i%2 == 0){
             j=0;
            while(j<imagen->ancho){
                //printf("%d,%d ",i,j );
                cargar_pixel(i,j,archivo,imagen);
                j++;
            }
            //printf("\n");
        }
        else{
            j--;
            while(j>=0){
                //printf("%d,%d ",i,j );
                cargar_pixel(i,j,archivo,imagen);
                j--;
            }
            //printf("\n");
        }
        i++;
    }
}

void cargar_matriz_columna(BMP* imagen, FILE* archivo){
    int i=0;
    int j=0;
    while(i<imagen->ancho){
        j=0;
        while(j<imagen->alto){
            cargar_pixel(j,i,archivo,imagen);
            j++;
        }
        i++;
    }
}

void abrir_imagen(BMP *imagen, char *ruta, int modo){
    FILE *archivo;  //Puntero FILE para el archivo de imÃ¡gen a abrir
    //int i,j;
    
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
    int i;
    imagen->pixel=(Pixel**) malloc (imagen->alto* sizeof(Pixel*)); 
    for( i=0; i<imagen->alto; i++)
        imagen->pixel[i]=(Pixel*) malloc (imagen->ancho* sizeof(Pixel)); 
    

    if (modo==1){
        //CARGAR POR FILA
        cargar_matriz_fila(imagen,archivo);
    }
    else if(modo == 0){
        //CARGAR POR COLUMNA
        cargar_matriz_columna(imagen,archivo);
    }
    
    
    
    //Cerrrar el archivo
    fclose(archivo);    
}

void destruir_imagen(BMP* imagen){
    int i;
    for (i=0;i<imagen->alto;i++){
        free(imagen->pixel[i]);
    }
    free(imagen->pixel);
    free(imagen);
}

void crear_imagen(BMP *imagen, char ruta[]){
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

void reducir_imagen_fila(BMP* imagen,int cantidad_pixeles, BMP* imagenNueva){
    int i=0,j=0;
    int avgB, avgG, avgR; //Promedio de R,G,B
    int sumB=0, sumG=0, sumR=0; 
    int m = cantidad_pixeles;
    int filaNueva;
    int contadorAvg = 0;
    int lastCount;

    while(i < imagen->alto){
        if(i%2 == 0){
            filaNueva = 0;
            j=0;
            while(filaNueva < imagenNueva->ancho){
                while(j < imagen->ancho){
                    if(m == 0){
                        break;
                    }else{
                        sumB = sumB  + imagen->pixel[i][j].B;
                        sumR = sumR  + imagen->pixel[i][j].R;
                        sumG = sumG  + imagen->pixel[i][j].G;
                        

                        contadorAvg++;
                        m--;
                        j++;
                    }
                }
                m = cantidad_pixeles;
                avgG = (sumG/contadorAvg);
                avgR = (sumR/contadorAvg);
                avgB = (sumB/contadorAvg);

                imagenNueva->pixel[i][filaNueva].B = avgB;
                imagenNueva->pixel[i][filaNueva].G = avgG;
                imagenNueva->pixel[i][filaNueva].R = avgR;
                sumB = 0;
                sumR = 0;
                sumG = 0;
                lastCount = contadorAvg;
                contadorAvg = 0;
                filaNueva++;
            } 
        }else{
            //fila nueva debería ser -1
            filaNueva--;
            j--;
            while(filaNueva>=0){ 
                while(j>=0){
                    if(j == imagen->ancho-1){
                        m = lastCount;
                    }
                    if(m == 0){
                        break;
                    }else{
                        sumB = sumB  + imagen->pixel[i][j].B;
                        sumR = sumR  + imagen->pixel[i][j].R;
                        sumG = sumG  + imagen->pixel[i][j].G;

                        contadorAvg++;
                        m--;
                        j--;
                    }
                }
                m = cantidad_pixeles;
                avgG = (sumG/contadorAvg);
                avgR = (sumR/contadorAvg);
                avgB = (sumB/contadorAvg); 
                
                imagenNueva->pixel[i][filaNueva].B = avgB;
                imagenNueva->pixel[i][filaNueva].G = avgG;
                imagenNueva->pixel[i][filaNueva].R = avgR;
                sumB = 0;
                sumR = 0;
                sumG = 0;
                contadorAvg = 0;
                filaNueva--;
            }
        }
        i++;
    }
}

void reducir_imagen_columna(BMP* imagen,int cantidad_pixeles, BMP* imagenNueva){
    int i,j,m;
    int avgB, avgG, avgR; //Promedio de R,G,B
    int filaNueva=0;
    int** sumPixel;
    sumPixel = (int**)malloc(sizeof(int*)*imagen->alto);
    for(i = 0; i < imagen->alto; i++){
        sumPixel[i] = (int*)malloc(sizeof(int)*3);
    }

    for(i = 0; i < imagen->alto; i++){
        for(j = 0; j < 3; j++){
            sumPixel[i][j] = 0;
        }
    }

    j=0;
    while(j < imagen->ancho){
        m=0;
        while(m < cantidad_pixeles){
            if(j == imagen->ancho){
                break;
            }
            else{
                i=0;
                while(i < imagen->alto){
                    sumPixel[i][0] = sumPixel[i][0] + imagen->pixel[i][j].R;
                    sumPixel[i][1] = sumPixel[i][1] + imagen->pixel[i][j].G;
                    sumPixel[i][2] = sumPixel[i][2] + imagen->pixel[i][j].B;
                    i++;
                }
                m++;
                j++;
            }
        }

        i = 0;
        while(i < imagen->alto){
            avgR = (sumPixel[i][0])/m;
            avgG = (sumPixel[i][1])/m;
            avgB = (sumPixel[i][2])/m;           

            imagenNueva->pixel[i][filaNueva].B = avgB;
            imagenNueva->pixel[i][filaNueva].G = avgG;
            imagenNueva->pixel[i][filaNueva].R = avgR;

            sumPixel[i][0] = 0;
            sumPixel[i][1] = 0;
            sumPixel[i][2] = 0;
            i++;
        }
        filaNueva++;
    } 
}

void reduce_imagen(BMP* imagen, int modo, int cantidad_pixeles, int iteraciones){
    int i; 
    BMP* imagenReducida;
    BMP* imagenRedCol;
   
    imagenReducida = (BMP*)malloc(sizeof(BMP));
    imagenReducida->bm[0] = imagen->bm[0];
    imagenReducida->bm[1] = imagen->bm[1];  
    imagenReducida->tamano = imagen->tamano;
    imagenReducida->reservado = imagen->reservado;
    imagenReducida->offset = imagen->offset;
    imagenReducida->tamanoMetadatos = imagen->tamanoMetadatos;
    imagenReducida->alto = imagen->alto;
    imagenReducida->numeroPlanos = imagen->numeroPlanos;
    imagenReducida->profundidadColor = imagen->profundidadColor;
    imagenReducida->tipoCompresion = imagen->tipoCompresion;
    imagenReducida->tamanoEstructura = imagen->tamanoEstructura;
    imagenReducida->pxmh = imagen->pxmh;
    imagenReducida->pxmv = imagen->pxmv;
    imagenReducida->coloresUsados = imagen->coloresUsados;
    imagenReducida->coloresImportantes = imagen->coloresImportantes;
    if(imagen->ancho%cantidad_pixeles == 0){
        imagenReducida->ancho = imagen->ancho/cantidad_pixeles;
    }else{
        imagenReducida->ancho = (imagen->ancho/cantidad_pixeles) + 1;
    }
    imagenReducida->pixel=(Pixel**) malloc (imagenReducida->alto* sizeof(Pixel*)); 
    for(i = 0; i < imagenReducida->alto; i++){
        imagenReducida->pixel[i] = (Pixel*)malloc(sizeof(Pixel)*imagenReducida->ancho);
    }

    //primer parametro -> siempre imagen original
    //método 1 -> fila
    //método 2 -> columnas
    //método 3 -> ambas
    if(modo == 1){
        for(i = 0; i < iteraciones; i++){
            reducir_imagen_fila(imagen,cantidad_pixeles,imagenReducida);
        }
    }else if(modo == 2){
        for(i = 0; i < iteraciones; i++){
            reducir_imagen_columna(imagen,cantidad_pixeles,imagenReducida);
        }
    }else{
        for(i = 0; i < iteraciones; i++){
            reducir_imagen_fila(imagen,cantidad_pixeles,imagenReducida);
        }
        imagenRedCol = (BMP*)malloc(sizeof(BMP));
        imagenRedCol->bm[0] = imagen->bm[0];
        imagenRedCol->bm[1] = imagen->bm[1];
        imagenRedCol->bm[2] = '\0';
        imagenRedCol->tamano = imagen->tamano;
        imagenRedCol->reservado = imagen->reservado;
        imagenRedCol->offset = imagen->offset;
        imagenRedCol->tamanoMetadatos = imagen->tamanoMetadatos;
        imagenRedCol->alto = imagen->alto;
        imagenRedCol->numeroPlanos = imagen->numeroPlanos;
        imagenRedCol->profundidadColor = imagen->profundidadColor;
        imagenRedCol->tipoCompresion = imagen->tipoCompresion;
        imagenRedCol->tamanoEstructura = imagen->tamanoEstructura;
        imagenRedCol->pxmh = imagen->pxmh;
        imagenRedCol->pxmv = imagen->pxmv;
        imagenRedCol->coloresUsados = imagen->coloresUsados;
        imagenRedCol->coloresImportantes = imagen->coloresImportantes;
        if(imagen->ancho%cantidad_pixeles == 0){
            imagenRedCol->ancho = imagen->ancho/cantidad_pixeles;
        }else{
            imagenRedCol->ancho = (imagen->ancho/cantidad_pixeles) + 1;
        }
        imagenRedCol->pixel=(Pixel**) malloc (imagenRedCol->alto* sizeof(Pixel*)); 
        for(i = 0; i < imagenRedCol->alto; i++){
            imagenRedCol->pixel[i] = (Pixel*)malloc(sizeof(Pixel)*imagenRedCol->ancho);
        }
        for(i = 0; i < iteraciones; i++){
            reducir_imagen_columna(imagen,cantidad_pixeles,imagenRedCol);
        }
    }

   // print_imagen(imagen);
    print_imagen(imagenReducida);
    printf("--------------------------------------------------\n");
    if(modo == 3)
        print_imagen(imagenRedCol);
}

void print_imagen(BMP *imagen){
    printf("Bm %s%s\n",&imagen->bm[0],&imagen->bm[1]);
    printf("Tamano %d\n",imagen->tamano);
    printf("reservado %d\n",imagen->reservado);
    printf("offset %d\n",imagen->offset);
    printf("Tamano Metadatos %d\n",imagen->tamanoMetadatos);
    printf("Alto %d\n",imagen->alto);
    printf("Ancho %d\n",imagen->ancho);
    printf("Numero Planos %d\n",imagen->numeroPlanos);
    printf("Profundidad Color %d\n",imagen->profundidadColor);
    printf("Tipo compresion %d\n",imagen->tipoCompresion);
    printf("Tamano Estructua %d\n",imagen->tamanoEstructura);
    printf("pxmh %d\n",imagen->pxmh);
    printf("pxmv %d\n",imagen->pxmv);
    printf("Colores Usados %d\n",imagen->coloresUsados);
    printf("Colores Importantes %d\n",imagen->coloresImportantes);

    printf("PIXEL:\n");
    for(int i = 0; i < imagen->alto; i++){
        printf("%d - ", i+1);
        for(int j = 0; j < imagen->ancho; j++){
            printf("(%d,",imagen->pixel[i][j].R);
            printf("%d,",imagen->pixel[i][j].G);
            printf("%d) ",imagen->pixel[i][j].B);
        }
        printf("\n");
    }
}

int opt_get(int argc, char** argv, char ivalue[300],char svalue[300],char gvalue[300],int* nvalue, int* mvalue, int* ovalue,int* dvalue){

    if(argc > 14){
        printf("Sobran parametros.\n");
        return 0;
    }else if(argc < 13){
        printf("Faltan parametros.\n");
        return 0;
    }
    int c;
    while ((c = getopt(argc,argv,"i:s:g:n:m:o:d")) != -1){
        switch(c){
            case 'i':
                strcpy(ivalue,optarg);
                break;
            case 's':
                strcpy(svalue,optarg);
                break;
            case 'g':
                strcpy(gvalue,optarg);    
                break;
            case 'n':    
                sscanf(optarg, "%d", nvalue);
                break;
            case 'm':
                sscanf(optarg, "%d", mvalue);
                break;
            case 'o':
                sscanf(optarg, "%d", ovalue);    
                break;
            case 'd':
                *dvalue = 1;
                break;
            case '?':
                if(isprint(optopt)){
                    printf("Opcion desconocida.\n");   
                    return 0;
                }
                else{ 
                    printf("Opcion con caracter desconocido\n");
                    return 0;
                }
            default:
                abort();
        }
    }
    return 1;
}

int verifyArguments(char* ivalue, char* svalue, char* gvalue, int nvalue, int mvalue, int ovalue){
    //Falta la verificacion de mvalue mayor que el ancho de la imagen
    if(fileExists(ivalue) == 0){
        printf("ERROR: Archivo no encontrado.\n");
        return 0;
    }else if(ovalue!=1 && ovalue!=2 && ovalue!=3){
        printf("ERROR: Metodo Incorrecto.\n");
        return 0;
    }else if(nvalue<=0){
        printf("ERROR: Cantidad de iteraciones debe ser mayor que 0.\n");
        return 0;
    }else if(mvalue<=0){
        printf("ERROR: Cantidad de pixeles debe ser mayor que 0.\n");
        return 0;
    }else 
        return 1;
    return 1;
}

int fileExists(char* nombreArchivo){
    FILE* archivo;
    archivo = fopen(nombreArchivo, "r");
    if(archivo == NULL)
        return 0;
    else
        return 1;
}