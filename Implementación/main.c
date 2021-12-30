#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "alsp.h"

int main(){
    char instancia[5];
    printf("Ingresar numero de instancia: ");
    //scanf(" %c", &instancia);
    fgets(instancia, 20, stdin);

    instancia[strcspn(instancia, "\n")] = 0;
    char archivo[20];
    strcat(strcpy(archivo, "airland"), instancia);

    strcat(archivo, ".txt");

    FILE* alsp = fopen(archivo, "r");
    if (alsp == NULL){
        printf("El archivo no existe.\n");
        exit(1);
    }
    lecturaArchivo(alsp, instancia);
    return 0;
}