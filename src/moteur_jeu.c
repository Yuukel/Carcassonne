#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "game_structures.h"
#include "lecteur_csv.h"

// A faire : dispatcher dans plusieurs fichiers etc
// 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

//*************************************

#define DIMENSION_MAX 10 // En attendant ?

#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;34m"
#define PINK "\e[0;35m"

#define RESET "\e[0m"
#define ROUTE "\e[1;30m\e[103m"
#define PRE "\e[1;30m\e[42m"
#define VILLE "\e[1;30m\e[43m"
#define BLASON "\e[1;30m\e[46m"
#define ABBAYE "\e[1;30m\e[41m"

//*************************************

// Fonctions de debug
void PrintTileDbg(TileStruct t);
void SimplePrintTileDbg(TileStruct t);
void PrintGridDbg(TileStruct grid[DIMENSION_MAX][DIMENSION_MAX]);
void PrintPlayers(int nb);
TileStruct RotateTile(TileStruct t, int sens);
void Tour();

TileStruct pile[72];
TileStruct grid[DIMENSION_MAX][DIMENSION_MAX];

int main(int argc, char * argv[])
{
    //system("clear");
    parseur_csv("tuiles_base_simplifiees.csv", pile);

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    PrintPlayers(5);
    getch();
    
    endwin();
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

void SimplePrintTileDbg(TileStruct t){
    printf(" %s %s \n",Couleur(t.cotes[0]),RESET);
    printf("%s %s",Couleur(t.cotes[3]),RESET);
    printf("%s %s",Couleur(t.centre),RESET);
    printf("%s %s\n",Couleur(t.cotes[1]),RESET);
    printf(" %s %s \n",Couleur(t.cotes[2]),RESET);
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
            SimplePrintTileDbg(pile[0]);
        }
        printf("\n");
    }
}

void PrintPlayers(int nb){
    for(int j = 0 ; j < nb ; j++){
        printw("+");
        for(int i = 0 ; i < 18 ; i++){
            printw("-");
        }
        printw("+");

        printw("\t\t\t");
    }
    printw("\n");
    for(int j = 0 ; j < nb ; j++){
        printw("| ");
        printw("Joueur : %d",j+1);
        printw(" = %d", 999);
        printw(" |");

        printw("\t\t\t");
    }
    printw("\n");
    for(int j = 0 ; j < nb ; j++){
        printw("+");
        for(int i = 0 ; i < 18 ; i++){
            printw("-");
        }
        printw("+");

        printw("\t\t\t");
    }
    printw("\n\n");
    refresh();
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

void Tour(){
    int choice;
    PrintTileDbg(pile[1]);
    while(scanf("%d",&choice) != 0){
        pile[1] = RotateTile(pile[1], choice);
        system("clear");
        PrintTileDbg(pile[1]);
    }
}