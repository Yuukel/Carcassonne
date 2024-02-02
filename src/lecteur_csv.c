#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lecteur_csv.h"

void parseur_csv(char * fileName)
{
    FILE * fichier = fopen(fileName,"r");

    /*char temp[1024];
    while(fgets(temp,1024,file)){
        char * value = strtok(temp,", ");
        printf("%s",value);
    }*/

    fclose(fichier);
}
