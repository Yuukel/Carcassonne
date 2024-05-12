#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "draw_functions.h"
#include "end_functions.h"
#include "game_structures.h"
#include "pawn_functions.h"
#include "print_functions.h"
#include "start_functions.h"
#include "tile_functions.h"
#include "title_screen.h"

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

GameStruct WaitingForAction(GameStruct game);

GameStruct game;

// afficher la grille en dessous (je vais dormir)
void PrintGameScreenDbg(GameStruct game) {
    game.turn.coordXMin = -5;
    game.turn.coordYMin = -5;
    do {
        game = InitTurn(game);
        game.turn.currentState = Tile;
        do {
            erase();
            PrintPlayers(game);
            if (game.turn.currentState == Tile) {
                game = CanBePlaced(game);
                game = ChangeNumbers(game);
            }
            PrintGrid(game, game.turn.coordXMin, game.turn.coordYMin);
            PrintTurnInfos(game.turn);
            PrintCommands(game.turn, game);

            game = WaitingForAction(game);
        } while (game.turn.turnEnd == 0);
        RemoveTile(game.pile);
        game.turn.currentState = End;
    } while (game.pile[0].tileType != 0);
}

int main(int argc, char* argv[]) {
    InitNcurses();

    TitleScreen();

    game = InitGame(game);

    erase();

    PrintGameScreenDbg(game);

    endwin();
    return 0;
}

// A commenter
void InitNcurses() {
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

GameStruct WaitingForAction(GameStruct game) {
    int ch;
    ch = getch();

    if (game.turn.currentState == Tile) {
        if (ch == 'c') {
            game.turn.currentMode = Camera;
        }

        if (ch == 'r') {
            game.turn.currentMode = Rotation;
        }

        if (ch == 'p') {
            game.turn.currentMode = Pose;
        }

        if (game.turn.currentMode == Rotation) {
            game.turn.currentTile = RotateTile(game.turn.currentTile, ch);
        }

        if (game.turn.currentMode == Camera) {
            game.turn = MoveCamera(game.turn, ch);
        }

        if (game.turn.currentMode == Pose) {
            game = PlaceTile(game, ch);
        }
    } else if (game.turn.currentState == Pawn) {
        if (game.turn.currentMode == Question) {
            if (ch == ' ') {
                game.turn.currentMode = Pion;
                game.turn.currentSide = 4;
                game = PlacePawn(game, (CoordStruct){game.turn.currentX, game.turn.currentY}, game.turn.currentSide);
            }

            if (ch == 'x') {
                game.turn.turnEnd = 1;
            }
        } else if (game.turn.currentMode == Pion) {
            if (ch == ' ') {
                game = AddPawn(game);
            }

            if (ch == 'x') {
                // remove last pawn (changement d'avis)
                game.turn.turnEnd = 1;
            }

            if (ch == KEY_LEFT || ch == KEY_RIGHT || ch == KEY_UP || ch == KEY_DOWN) {
                game = ChoosePawnPosition(game, game.turn.currentX, game.turn.currentY, ch);
            }
        }
    }

    return game;
}