#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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
GameStruct AIActions(GameStruct game);
int AIHaveToRotate(GameStruct game);

GameStruct game;

// afficher la grille en dessous (je vais dormir)
void PrintGameScreen(GameStruct game) {
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
            if (game.turn.currentPlayer.isHuman == 1)
                game = WaitingForAction(game);
            else
                game = AIActions(game);
        } while (game.turn.turnEnd == 0);
        RemoveTile(game.pile);
        game = EndTurn(game);
        game.turn.currentState = End;
    } while (game.pile[0].tileType != 0);
    game = EndGame(game);
    PrintEndScreen(game);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    InitNcurses();

    TitleScreen();

    game = InitGame(game);

    erase();

    PrintGameScreen(game);

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
    init_pair(FIN, COLOR_WHITE, COLOR_BLACK);
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
                game = RemovePawn(game, game.pawns[25]);
                game.turn.turnEnd = 1;
            }

            if (ch == KEY_LEFT || ch == KEY_RIGHT || ch == KEY_UP || ch == KEY_DOWN) {
                game = ChoosePawnPosition(game, game.turn.currentX, game.turn.currentY, ch);
            }
        }
    }

    return game;
}

GameStruct AIActions(GameStruct game) {
    int ch;
    if (game.turn.currentState == Tile) {
        game.turn.currentMode = Rotation;
        while (AIHaveToRotate(game)) {
            int nbRotation = rand() % 5;
            ch = KEY_RIGHT;
            for (int R = 0; R < nbRotation; R++) {
                if (game.turn.currentMode == Rotation) {
                    game.turn.currentTile = RotateTile(game.turn.currentTile, ch);
                }
            }
        }

        game.turn.currentMode = Pose;
        int nbDeplacement = rand() % 11;
        ch = KEY_RIGHT;
        for (int D = 0; D < nbDeplacement; D++) {
            if (game.turn.currentMode == Pose) {
                game = PlaceTile(game, ch);
            }
        }
        ch = ' ';
        if (game.turn.currentMode == Pose) {
            game = PlaceTile(game, ch);
        }
    } else if (game.turn.currentState == Pawn) {
        if (game.turn.currentMode == Question) {
            int randomChoice = rand() % 3;

            if (randomChoice == 1) {
                game.turn.currentMode = Pion;
                game.turn.currentSide = 4;
                game = PlacePawn(game, (CoordStruct){game.turn.currentX, game.turn.currentY}, game.turn.currentSide);
            } else {
                game.turn.turnEnd = 1;
            }
        } else if (game.turn.currentMode == Pion) {
            int nbDeplacementPion = rand() % 10;
            for (int P = 0; P < nbDeplacementPion; P++) {
                int randomDirection = rand() % 4;
                switch (randomDirection) {
                    case 0:
                        ch = KEY_UP;
                        break;
                    case 1:
                        ch = KEY_RIGHT;
                        break;
                    case 2:
                        ch = KEY_DOWN;
                        break;
                    case 3:
                        ch = KEY_LEFT;
                        break;
                }
                game = ChoosePawnPosition(game, game.turn.currentX, game.turn.currentY, ch);
            }
            game = AddPawn(game);
        }
    }

    return game;
}

int AIHaveToRotate(GameStruct game) {
    for (int i = 0; i < 143; i++) {
        for (int j = 0; j < 143; j++) {
            if (game.grid[i][j].tileType == 2) return 0;
        }
    }

    return 1;
}