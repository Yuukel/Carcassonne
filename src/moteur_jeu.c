#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "game_structures.h"
#include "debug.h"
#include "title_screen.h"
#include "lecteur_csv.h"

// A faire : dispatcher dans plusieurs fichiers etc
// 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

//*************************************

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

int main(int argc, char * argv[])
{
    // parseur_csv("tuiles_base_simplifiees.csv", game.pile);

    InitNcurses();

    TitleScreen();

    game.playerList[0] = (PlayerStruct){1, RED, 0, 5, 1};
    game.playerList[1] = (PlayerStruct){2, GREEN, 0, 5, 0};
    game.playerList[2] = (PlayerStruct){0, 0, 0, 0, -1};
    game.playerList[3] = (PlayerStruct){0, 0, 0, 0, -1};
    game.playerList[4] = (PlayerStruct){0, 0, 0, 0, -1};
    game = SelectPlayers(game);

    PrintPlayers(game);
    getch();
    erase();
    TileStruct t = {{'N','E','S','O'},'C'};
    PrintTileDbg(t);
    getch();

    endwin();
    return 0;
}

void InitNcurses(){
    initscr();
    cbreak();
    noecho();
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
}

void PrintPlayers(GameStruct game){
    for(int j = 0 ; j < game.nbPlayers ; j++){
        printw("+");
        for(int i = 0 ; i < 18 ; i++){
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
        printw(" = %d", 999);
        printw(" |");

        printw("\t\t\t");
    }
    printw("\n");
    for(int j = 0 ; j < game.nbPlayers ; j++){
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

// void Tour(){
//     int choice;
//     PrintTileDbg(pile[1]);
//     while(scanf("%d",&choice) != 0){
//         pile[1] = RotateTile(pile[1], choice);
//         system("clear");
//         PrintTileDbg(pile[1]);
//     }
// }