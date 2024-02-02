#include <stdio.h>
#include <stdlib.h>
#include "game_structures.h"
#include "lecteur_csv.h"

// A faire : dispatcher dans plusieurs fichiers etc
// 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

//*************************************

#define DIMENSION_MAX 10 // En attendant ?

//*************************************

// Fonctions de debug
void PrintTileDbg(TileStruct t);
void PrintGridDbg(TileStruct grid[DIMENSION_MAX][DIMENSION_MAX]);

enum State {Draw, Tile, Pawn, End}; // Etats du jeu
enum Type {Pre, Route, Ville, Blason, Abbaye, Fin}; //Check les traductions

TileStruct pioche[71];
TileStruct grid[DIMENSION_MAX][DIMENSION_MAX];

int main(int argc, char * argv[])
{
    TileStruct t = { 0, {"V", "R", "P", "R"}, "R" };
    //enum State gameState;

    PrintTileDbg(t);
    system("clear");
    PrintGridDbg(grid);
    system("clear");
    parseur_csv(argv[1]);

    return 0;
    //parseur_csv();
}

void PrintTileDbg(TileStruct t){
    printf("\t%s\t\n",t.cotes[0]);
    printf("%s\t%s\t%s\n", t.cotes[1], t.centre, t.cotes[2]);
    printf("\t%s\t\n",t.cotes[3]);
}

void PrintGridDbg(TileStruct grid[DIMENSION_MAX][DIMENSION_MAX]){
    printf("\t");
    for(int i = 0 ; i < DIMENSION_MAX ; i++){
        printf("%d\t", i);
    }
    printf("\n");
    for(int y = 0 ; y < DIMENSION_MAX ; y++){
        printf("%d\t",y);
        for(int x = 0 ; x < DIMENSION_MAX ; x++){
            printf("x\t");
        }
        printf("\n");
    }
}