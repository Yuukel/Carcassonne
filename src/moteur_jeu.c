#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "game_structures.h"
#include "debug.h"
#include "title_screen.h"
#include "lecteur_csv.h"

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

//*************************************

// Fonctions pour ncurses
void InitNcurses();

// Fonctions d'affichage
void PrintPlayers(GameStruct game);

// Fonctions de tuiles
TileStruct RotateTile(TileStruct t, int sens);
void Tour();

GameStruct game;
// TileStruct pile[72];
TileStruct grid[DIMENSION_MAX][DIMENSION_MAX];

//afficher la grille en dessous (je vais dormir)
void PrintGameScreenDbg(GameStruct game){
    int coordXMin = -5;
    int coordYMin = -5;
    int ch;
    game.turn.currentPlayer = game.playerList[0];
    game.turn.currentTile = game.pile[1];
    do{
        erase();
        PrintPlayers(game);
        PrintGridDbg(game,coordXMin,coordYMin);
        PrintCurrentTileDbg(game);

        ch = getch();
        if(ch == ' ') break;

        // Déplacement de la caméra du plateau
        if(ch == KEY_UP && coordYMin > -71) coordYMin--;
        if(ch == KEY_DOWN && coordYMin < 61) coordYMin++;
        if(ch == KEY_LEFT && coordXMin > -71) coordXMin--;
        if(ch == KEY_RIGHT && coordXMin < 61) coordXMin++;
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

    TileStruct t = {{'N','E','S','O'},'C'};
    for(int i = 0 ; i < 143 ; i++){
        for(int j = 0 ; j < 143 ; j++){
            game.grid[i][j] = (TileStruct){{'0','0','0','0'},'0'};
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
}

void PrintPlayers(GameStruct game){
    for(int j = 0 ; j < game.nbPlayers ; j++){
        printw("+");
        for(int i = 0 ; i < 19 ; i++){
            printw("-");
        }
        printw("+");

        printw("\t\t\t");
    }
    printw("\n");
    for(int j = 0 ; j < game.nbPlayers ; j++){
        printw("| ");
        attron(COLOR_PAIR(game.playerList[j].color));
        printw("Joueur : %d",j+1);
        attroff(COLOR_PAIR(game.playerList[j].color));
        printw(" = %d", 9999);
        printw(" |");

        printw("\t\t\t");
    }
    printw("\n");
    for(int j = 0 ; j < game.nbPlayers ; j++){
        printw("|   ");
        for(int i = 0 ;  i < 7 ; i++){
            attron(COLOR_PAIR(game.playerList[j].color));
            printw("o ");
            attroff(COLOR_PAIR(game.playerList[j].color));
        }
        printw("  |");
        printw("\t\t\t");
    }
    printw("\n");
    for(int j = 0 ; j < game.nbPlayers ; j++){
        printw("+");
        for(int i = 0 ; i < 19 ; i++){
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

// void Tour(){
//     int choice;
//     PrintTileDbg(pile[1]);
//     while(scanf("%d",&choice) != 0){
//         pile[1] = RotateTile(pile[1], choice);
//         system("clear");
//         PrintTileDbg(pile[1]);
//     }
// }