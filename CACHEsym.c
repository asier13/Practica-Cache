#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define LIM_CACHE 4
# define LIM_BYTE 8
# define LIM_HEXA 16
# define LIM_CACHE_AUX 16

typedef   struct
{ short int ETQ;
  char datos[8];
}T_LINEA_CACHE;

typedef   T_LINEA_CACHE tpCacheSym[LIM_CACHE];

typedef   char tpByte[LIM_BYTE];

typedef char tpConvertHexa[LIM_HEXA];
typedef char *tpConvertBin[LIM_HEXA];



/************ inicializa la cache  ***********************************/
void initCache (T_LINEA_CACHE *cache){
int i,j;
for (i=0;i<LIM_CACHE;i++){
  cache[i].ETQ=0xFF;
  for (j=0;j<LIM_BYTE; j++) {
	cache[j].datos[i]=0;
  }
}

}
/************ inicializa la cache  ***********************************/
/************ convierte hexadecimal en binario ***********************/
char * coverHEXA_BINA (char cad[4]){
	tpConvertHexa convertHexa= {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	tpConvertBin convertBin= {"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"};
	tpByte vbyte;
    int i=0;
    int encontrado=0;
	while ((!encontrado)&&(i<LIM_HEXA)){
	    //printf("\ncad[2]= %c == corvertHexa[%d]=%c", cad[2],i, convertHexa[i]);
		encontrado=(cad[2]==convertHexa[i]);
		if (!encontrado && i<LIM_HEXA){
		    i++;}

	}

	strcpy(vbyte,convertBin[i]);

	i=0;encontrado=0;
    while (i<LIM_HEXA&&!encontrado){
		encontrado=(cad[3]==convertHexa[i]);
        //printf("\ncad[3]= %c == corvertHexa[%d]=%c", cad[3],i, convertHexa[i]);
		if(!encontrado && i<LIM_HEXA)i++;
	}

    strcat(vbyte,convertBin[i]);
	return vbyte;

}
/************ convierte hexadecimal en binario ***********************/
/************ convierte binario a hexadecimal ***********************/
void coverBINA_HEXA (char datos[8]){
	tpConvertHexa convertHexa= {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	tpConvertBin convertBin= {"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"};
	tpByte vbyte;
    int i=0;
    int j=0;
    int k=0,m=0;
    int encontrado=0;
    char menosSig[4];
    char masSig[4];

    strncpy(menosSig,datos,4);
    for(k=5;k<LIM_BYTE;k++){
        masSig[m]=datos[k];
        m++;
    }
	while ((!encontrado)&&(i<LIM_HEXA)){
        encontrado=strcmp(menosSig,convertBin[i]);
		if (!encontrado && i<LIM_HEXA){
		    i++;}

	}
	encontrado=0;
	while ((!encontrado)&&(j<LIM_HEXA)){
        encontrado=!strcmp(masSig,convertBin[j]);
		if (!encontrado && j<LIM_HEXA){
		    j++;}

	}
    if(encontrado){
        printf("%c%c", convertHexa[i], convertHexa[i]);
        }



}
/************ convierte binario a hexadecimal  ***********************/



/************ Guarda en la cache los accesos a memoria ****************/
 void procesaCache(char cad[4], tpCacheSym cacheSym){

 int  linea=cad[1]-48; /*resto el valor que ocupa el caracter número en ascii el 48 que es el del cero*/

 strcpy(cacheSym[linea].datos,coverHEXA_BINA(cad)); /**** copio el codigo binario en los datos de la cache**/

 }

/************ Guarda en la cache los accesos a memoria ****************/

/************************visualiza la cache****************************/
void visualizaCache (tpCacheSym cacheSym){
int i,j;

for (i=0;i<LIM_CACHE;i++){
	printf("\nNumero de linea %d, tiene como Etiqueta %d ",
	 i,
	 cacheSym[i].ETQ);
	 for (j=0;j<LIM_BYTE;j++) {
	    printf("y como dato %d \n",cacheSym[i].datos[j]);
	    }


}

}
/************************visualiza la cache****************************/

int etiquetaDireccionIGUAL_ETQ(char cad[4], tpCacheSym cacheSym) {
 int linea=cad[1]-48;
 int etiqueta=cad[0]-48;
 return(cacheSym[linea].ETQ==etiqueta);
}
/************ programa principal***************************************/
int main( ){ //int arc; char *argv[]

int tiempoGlobal = 1;
int i,j,k, numfallos = 0;

tpCacheSym cacheSym;



FILE * filaRAM;
FILE * fichAccesosMemoria;

unsigned char cad[5], dato[4];

char texto[100];

int leidos=0;
char memoriaRAM[1024];

initCache (cacheSym);

filaRAM = fopen ( "RAM.bin", "rb" );
fichAccesosMemoria = fopen ( "accesos_memoria.txt" , "r" );
if (filaRAM == NULL ){
      printf ( " Error al abrir el fichero RAM.bin. \n " );
	 /*exit(-1);*/
    }
   else {
	   while (!feof(filaRAM))
          leidos = fread(memoriaRAM,1,1024, filaRAM);
	   }

if (fichAccesosMemoria == NULL ){
      printf ( " Error al abrir el fichero accesos_memoria.txt. \n " );
	  exit(-1);
    }
   else {
     i=0;j=0;k=0;
	 while ((i<12)&&(fgets(cad,5,fichAccesosMemoria ) != NULL)){
		 if (!etiquetaDireccionIGUAL_ETQ(cad, cacheSym)) {
		     numfallos++;
             tiempoGlobal*=10;
            // strcpy(dato,cad[2]);
            // strcat(dato,cad[3]);
		     printf("\nT: %d, Fallo de CACHE %d, ADDR %04X ETQ %X linea %02X palabra %02X bloque %02X",
		      tiempoGlobal,0,0,cad[0],cad[1],0,cad[2] );
             printf("\nCARGANDO EL BLOQUE %d EN LA LINEA %d \n ", cad[2],cad[1]);


		     procesaCache(cad,cacheSym);

	     }
	     else {

	         printf("T: %d, Acierto de CACHE %d, ADDR %04X ETQ %X linea %02X palabra %02X DATO %02X",
		      tiempoGlobal,0,0,cad[0],cad[1],0,cad[2] );
             printf("ETQ: %c  ", cad[0], cad[2], cad[3]);


	         }
	      i++;
      }
   }

fclose(filaRAM);
fclose(fichAccesosMemoria);

return 0;
}

/************ programa principal***************************************/
