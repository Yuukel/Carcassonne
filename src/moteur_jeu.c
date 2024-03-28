#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "game_structures.h"
#include "title_screen.h"
#include "start_functions.h"
#include "print_functions.h"
#include "tile_functions.h"

// A faire : dispatcher dans plusieurs fichiers etc
// A faire : commenter chaque fonction etc

//*************************************
#define DIMENSION_MAX 143

#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define PINK 5
#define CYAN 6

#define ROUTE 7
#define PRE 8
#define VILLE 9
#define BLASON 10
#define ABBAYE 11
#define FIN 12
#define PLACEMENT 13

//*************************************

// Fonctions pour ncurses
void InitNcurses();

GameStruct game;
TileStruct grid[DIMENSION_MAX][DIMENSION_MAX];

//afficher la grille en dessous (je vais dormir)
void PrintGameScreenDbg(GameStruct game){
    int coordXMin = -5;
    int coordYMin = -5;
    game.turn.currentPlayer = game.playerList[0];
    game.turn.currentTile = game.pile[1];
    do{
        erase();
        PrintPlayers(game);
        // game = CanBePlaced(game);
        PrintGrid(game,coordXMin,coordYMin);
        PrintTurnInfos(game);

        if(WaitingForAction() == 1) break;
    }while(1);
}

int main(int argc, char * argv[])
{
    parseur_csv("tuiles_base_simplifiees.csv", game.pile);

    InitNcurses();

    TitleScreen();

    game.playerList[0] = (PlayerStruct){1, RED, 0, 5, 1};
    game.playerList[1] = (PlayerStruct){2, GREEN, 0, 5, 0};
    game.playerList[2] = (PlayerStruct){0, 0, 0, 0, -1};
    game.playerList[3] = (PlayerStruct){0, 0, 0, 0, -1};
    game.playerList[4] = (PlayerStruct){0, 0, 0, 0, -1};
    game = SelectPlayers(game);

    for(int i = 0 ; i < 143 ; i++){
        for(int j = 0 ; j < 143 ; j++){
            game.grid[i][j] = (TileStruct){{'0','0','0','0'},'0',0};
        }
    }
    game.grid[71][71] = game.pile[0];
    game.grid[73][71] = game.pile[71-5];
    erase();

    PrintGameScreenDbg(game);

    endwin();
    return 0;
}

// A commenter
void InitNcurses(){
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    start_color();

    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(PINK, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);

    init_pair(PRE, COLOR_BLACK, COLOR_GREEN);
    init_pair(ROUTE, COLOR_BLACK, COLOR_YELLOW);
    init_pair(VILLE, COLOR_BLACK, COLOR_CYAN);
    init_pair(BLASON, COLOR_BLACK, COLOR_BLUE);
    init_pair(ABBAYE, COLOR_BLACK, COLOR_RED);
    init_pair(FIN, COLOR_BLACK, COLOR_WHITE);
    init_pair(PLACEMENT, COLOR_BLACK, COLOR_WHITE);
}