#include <stdio.h>
#include <stdlib.h>
#include "game_structures.h"
#include "lecteur_csv.h"

// A faire : dispatcher dans plusieurs fichiers etc
// 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

//*************************************

#define DIMENSION_MAX 10 // En attendant ?

#define RESET "\e[0m"
#define ROUTE "\e[1;30m\e[103m"
#define PRE "\e[1;30m\e[42m"
#define VILLE "\e[1;30m\e[43m"
#define BLASON "\e[1;30m\e[46m"
#define ABBAYE "\e[1;30m\e[41m"

//*************************************

// Fonctions de debug
void PrintTileDbg(TileStruct t);
void PrintGridDbg(TileStruct grid[DIMENSION_MAX][DIMENSION_MAX]);
TileStruct RotateTile(TileStruct t, int sens);

enum State {Draw, Tile, Pawn, End}; // Etats du jeu
enum Type {Pre, Route, Ville, Blason, Abbaye, Fin}; //Check les traductions

TileStruct pile[72];
TileStruct grid[DIMENSION_MAX][DIMENSION_MAX];

int main(int argc, char * argv[])
{
    //system("clear");
    parseur_csv("tuiles_base_simplifiees.csv", pile);
    //for(int i = 0 ; i < 4 ; i++) printf("%s ", pile[0].cotes[i]);
    //printf("%c ", pile[0].centre);
    // printf("\n");
    // for(int i = 0 ; i < 10 ; i++){
    //     PrintTileDbg(pile[i]);
    //     printf("\n");
    // }

    PrintTileDbg(pile[0]);
    printf("\n");
    pile[0] = RotateTile(pile[0],1);
    PrintTileDbg(pile[0]);
    printf("\n");
    pile[0] = RotateTile(pile[0],-1);
    PrintTileDbg(pile[0]);
    printf("\n");
    pile[0] = RotateTile(pile[0],-1);
    PrintTileDbg(pile[0]);

    return 0;
}

char * Couleur(char c){
    if(c == 'r') return ROUTE;
    if(c == 'v') return VILLE;
    if(c == 'a') return ABBAYE;
    if(c == 'b') return BLASON;
    if(c == 'p') return PRE;

    return "";
}

void PrintTileDbg(TileStruct t){
    printf("   %s %c %s   \n",Couleur(t.cotes[0]),t.cotes[0],RESET);
    printf("%s %c %s",Couleur(t.cotes[3]),t.cotes[3],RESET);
    printf("%s %c %s",Couleur(t.centre),t.centre,RESET);
    printf("%s %c %s\n",Couleur(t.cotes[1]),t.cotes[1],RESET);
    printf("   %s %c %s   \n",Couleur(t.cotes[2]),t.cotes[2],RESET);
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

TileStruct RotateTile(TileStruct t, int sens){
    char temp; 
    if(sens > 0){
        temp = t.cotes[3];
        t.cotes[3] = t.cotes[2];
        t.cotes[2] = t.cotes[1];
        t.cotes[1] = t.cotes[0];
        t.cotes[0] = temp;
    }
    else if(sens < 0){
        temp = t.cotes[0];
        t.cotes[0] = t.cotes[1];
        t.cotes[1] = t.cotes[2];
        t.cotes[2] = t.cotes[3];
        t.cotes[3] = temp;
    }

    return t;
}