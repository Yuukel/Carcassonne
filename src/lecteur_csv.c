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
    TileStruct tile;
    while(fgets(temp,1024,fichier)){
        nb = 0;
        char * value = strtok(temp,", ");
        while(value != NULL){
            if(nb < 5) tile.cotes[nb] = value;
            else tile.centre = value;
            nb++;
            value = strtok(NULL,", ");
        }
        pile[index] = tile;
        index++;
    }

    fclose(fichier);
}
