#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/times.h>
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
            //printf("%d,%d\n",i,j );
            j++;
        }

        i++;
    }
    //printf("columna\n");
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
    fread(&imagen->ancho,sizeof(int),1, archivo);   
    fread(&imagen->alto,sizeof(int),1, archivo);    
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
    if (imagen->profundidadColor!= 24) 
    {
        printf ("La imagen debe ser de 24 bits.\n"); 
        exit(1);
    }

    //Reservar memoria para el arreglo que tendra la imÃ¡gen en escala de grises (Arreglo de tamaÃ±o "img.ancho X img.alto")
    int i;
    imagen->pixel=(Pixel**) malloc (imagen->alto* sizeof(Pixel*)); 
    for( i=0; i<imagen->alto; i++){
        imagen->pixel[i]=(Pixel*) malloc (imagen->ancho* sizeof(Pixel)); 
    }
    

    if (modo==1){
        //CARGAR POR FILA
        cargar_matriz_fila(imagen,archivo);
    }
    else if(modo == 2){
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
    fwrite(&imagen->ancho,sizeof(int),1, archivo);  
    fwrite(&imagen->alto,sizeof(int),1, archivo);   
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

void copiar_bitmap(BMP* src, BMP* dest){
    dest->bm[0] = src->bm[0];
    dest->bm[1] = src->bm[1];
    dest->tamano = src->tamano ;
    dest->reservado = src->reservado;
    dest->offset = src->offset;
    dest->tamanoMetadatos = src->tamanoMetadatos;
    dest->alto = src->alto;
    dest->ancho = src->ancho;
    dest->numeroPlanos = src->numeroPlanos ;
    dest->profundidadColor = src->profundidadColor;
    dest->tipoCompresion = src->tipoCompresion;
    dest->tamanoEstructura = src->tamanoEstructura;
    dest->pxmh = src->pxmh;
    dest->pxmv = src->pxmv;
    dest->coloresUsados = src->coloresUsados;
    dest->coloresImportantes = src->coloresImportantes;
}
 
void init_new_imagen(BMP* imagen,int cantidad_pixeles,int modo){
    if (modo == 1){
        if(imagen->ancho%cantidad_pixeles == 0){
            imagen->ancho = imagen->ancho/cantidad_pixeles;
        }
        else{
            imagen->ancho = (imagen->ancho/cantidad_pixeles) + 1;
        }
    }
    if (modo == 2){
        if(imagen->alto%cantidad_pixeles == 0){
            imagen->alto = imagen->alto/cantidad_pixeles;
        }else{
            imagen->alto = (imagen->alto/cantidad_pixeles) + 1;
        }
    }
    imagen->tamano = imagen->offset + (imagen->ancho*imagen->alto*(sizeof( unsigned char))) ;
     
    int i;
    imagen->pixel=(Pixel**) malloc (imagen->alto * sizeof(Pixel*)); 
    for( i=0; i<imagen->alto; i++){
        imagen->pixel[i]=(Pixel*) malloc (imagen->ancho* sizeof(Pixel)); 
    }
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
    int i = 0,j = 0;
    int m = cantidad_pixeles;
    int sumB = 0, sumR = 0, sumG = 0;
    print_imagen(imagenNueva);
    int k = 0;
    int counter = 0;

    for(i=0;i<imagen->ancho;i++){
        for(j=0;j<imagen->alto;j++){
            counter++;
            sumB += imagen->pixel[j][i].B;
            sumR += imagen->pixel[j][i].R;
            sumG += imagen->pixel[j][i].G;
            /*printf("%d,%d,%d\n",j,i,k);
            printf("counter: %d\n",counter );
            printf("%d\n",sumG );
            printf("%d\n",sumR );
            printf("%d\n",sumB );*/

            if (j==imagen->alto-1){
                /*printf("hola\n");
                printf("%d\n", sumR / counter);
                printf("hola2\n");*/
                imagenNueva->pixel[k][i].B = sumB / counter;
                imagenNueva->pixel[k][i].R = sumR / counter;
                imagenNueva->pixel[k][i].G = sumG / counter;
                //printf("chao\n");
                sumB = 0;
                sumR = 0;
                sumG = 0;
                k = 0;
                counter = 0;
            }
            //printf("counter fuera: %d\n", counter);
            if (counter == m){
                //printf("m : %d\n",counter );
                //printf("%d\n",sumB );
                imagenNueva->pixel[k][i].B = sumB / m;
                imagenNueva->pixel[k][i].R = sumR / m;
                imagenNueva->pixel[k][i].G = sumG / m;
                sumB = 0;
                sumR = 0;
                sumG = 0;
                counter = 0;
                k++;
            }

        }
        j++;
    }
   
}

void reduce_imagen(BMP* imagen, int modo, int cantidad_pixeles, int iteraciones,BMP* new_imagen){
    int i;
    copiar_bitmap(imagen,new_imagen);
    init_new_imagen(new_imagen,cantidad_pixeles,modo);
    if(modo == 1){
        printf("%d\n", new_imagen->ancho);
        printf("%d\n", new_imagen->alto);
        for (i = 0; i < iteraciones; i++)
        {
            reducir_imagen_fila(imagen,cantidad_pixeles,new_imagen);
        }
    }
    else if(modo == 2){
        printf("%d\n", new_imagen->ancho);
        printf("%d\n", new_imagen->alto);
        for (i = 0; i < iteraciones; i++)
        {
            reducir_imagen_columna(imagen,cantidad_pixeles,new_imagen);
        }
    }

}

void print_imagen(BMP *imagen){
    printf("Bm %c%c\n",imagen->bm[0],imagen->bm[1]);
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

   /* printf("PIXEL:\n");
    for(int i = 0; i < imagen->alto; i++){
        printf("%d - ", i+1);
        for(int j = 0; j < imagen->ancho; j++){
            printf("(%d,",imagen->pixel[i][j].R);
            printf("%d,",imagen->pixel[i][j].G);
            printf("%d) ",imagen->pixel[i][j].B);
        }
        printf("\n");
    }*/
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

void clock_start(Clock* clock){
    clock->startTime = times(&clock->startTms);
}
void clock_end(Clock* clock){
    clock->endTime = times(&clock->endTms);
    clock->micros = clock->endTime - clock->startTime;
    clock->ticksPerSec = sysconf(_SC_CLK_TCK);
}
void clock_print(Clock* clock){
    printf("CLK_TCK = %ld\n", clock->ticksPerSec);
    printf("Elapsed time (us): %lu\n", clock->micros*1000000/clock->ticksPerSec);
    printf("CPU user time (us): %lu\n",
    (clock->endTms.tms_utime - clock->startTms.tms_utime)*1000000/clock->ticksPerSec);
    printf("CPU system time (us): %lu\n",
    (clock->endTms.tms_stime - clock->startTms.tms_stime)*1000000/clock->ticksPerSec);
}