#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_structures.h"
#include "lecteur_csv.h"

void parseur_csv(char * fileName, TileStruct * pile)
{
    FILE * fichier = fopen(fileName,"r");
    int nb, index = 0;
    char temp[1024];
    while(fgets(temp,1024,fichier)){
        TileStruct tile;
        nb = 0;
        char * value = strtok(temp,", ");
        while(value != NULL){
            //printf("%s ",value);
            if(nb < 4) strcpy(tile.cotes[nb],value);
            else strcpy(tile.centre,value);
            nb++;
            value = strtok(NULL,", ");
        }
        //printf("%d ", index);
        pile[index] = tile;
        index++;
    }

    fclose(fichier);
}
