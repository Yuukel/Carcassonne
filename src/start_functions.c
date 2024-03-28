#include <stdio.h>
#include <string.h>

#include "start_functions.h"

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
            if(value[4] == 'a'){ // village
                if(nb < 4) tile.cotes[nb] = 'f';
                else tile.centre = 'f';
            }
            else{
                if(nb < 4) tile.cotes[nb] = value[0];
                else tile.centre = value[0];
            }
            nb++;
            value = strtok(NULL,", ");
        }
        tile.tileType = 1;
        pile[index] = tile;
        index++;
    }

    fclose(fichier);
}

// fonction pour placer la tuile de départ sur la grille

// fonction pour mélanger la pile