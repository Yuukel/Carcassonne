#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_structures.h"

#define ROUTE 7
#define PRE 8
#define VILLE 9
#define BLASON 10
#define ABBAYE 11
#define FIN 12
#define PLACEMENT 13

#define LIGNEMIN 6
#define LIGNEMINTUILE 8
#define COLONNEMINTUILE 4

void SetColor(char side) {
    if (side == 'r') {
        attron(COLOR_PAIR(ROUTE));
    } else if (side == 'a') {
        attron(COLOR_PAIR(ABBAYE));
    } else if (side == 'b') {
        attron(COLOR_PAIR(BLASON));
    } else if (side == 'v') {
        attron(COLOR_PAIR(VILLE));
    } else if (side == 'p') {
        attron(COLOR_PAIR(PRE));
    } else if (side == 'f') {
        attron(COLOR_PAIR(FIN));
    } else {
        attron(COLOR_PAIR(PLACEMENT));
    }
}

void RemoveColor() {
    attroff(COLOR_PAIR(ROUTE));
    attroff(COLOR_PAIR(ABBAYE));
    attroff(COLOR_PAIR(BLASON));
    attroff(COLOR_PAIR(VILLE));
    attroff(COLOR_PAIR(PRE));
    attroff(COLOR_PAIR(FIN));
    attroff(COLOR_PAIR(PLACEMENT));
}

void PrintTileDbg(TileStruct t, int i, int j) {
    if (t.tileType != 0) {
        // OUEST
        SetColor(t.cotes[3]);
        wmove(stdscr, LIGNEMINTUILE + 1 + (j * 3), COLONNEMINTUILE + (i * 9));
        printw(" %c ", t.cotes[3]);
        RemoveColor();

        // NORD
        SetColor(t.cotes[0]);
        wmove(stdscr, LIGNEMINTUILE + (j * 3), COLONNEMINTUILE + 3 + (i * 9));
        printw(" %c ", t.cotes[0]);
        RemoveColor();

        // CENTRE
        SetColor(t.centre);
        wmove(stdscr, LIGNEMINTUILE + 1 + (j * 3),
              COLONNEMINTUILE + 3 + (i * 9));
        printw(" %c ", t.centre);
        RemoveColor();

        // SUD
        SetColor(t.cotes[2]);
        wmove(stdscr, LIGNEMINTUILE + 2 + (j * 3),
              COLONNEMINTUILE + 3 + (i * 9));
        printw(" %c ", t.cotes[2]);
        RemoveColor();

        // EST
        SetColor(t.cotes[1]);
        wmove(stdscr, LIGNEMINTUILE + 1 + (j * 3),
              COLONNEMINTUILE + 6 + (i * 9));
        printw(" %c ", t.cotes[1]);
        RemoveColor();
    }
}

void PrintGridDbg(GameStruct game, int coordXMin, int coordYMin) {
    int longueur = 11;
    int hauteur = 11;

    for (int i = 0; i < longueur; i++) {
        wmove(stdscr, LIGNEMIN, COLONNEMINTUILE + 4 + (i * 9));
        printw("%d", coordXMin + i);
    }
    for (int j = 0; j < hauteur; j++) {
        wmove(stdscr, LIGNEMINTUILE + 1 + (j * 3), 0);
        printw("%d", coordYMin + j);
    }

    for (int i = 0; i < longueur; i++) {
        for (int j = 0; j < hauteur; j++) {
            PrintTileDbg(game.grid[coordXMin + i + 71][coordYMin + j + 71], i,
                         j);
        }
    }
}

char* CurrentMode(GameStruct game) {
    if (game.turn.currentMode == Rotation)
        return "Rotation";
    else if (game.turn.currentMode == Camera)
        return "Camera";
    else
        return "Pose";
}

void PrintCurrentTileDbg(GameStruct game) {
    // Affichage du "panneau" joueur
    wmove(stdscr, LIGNEMIN, 130);
    printw("+");
    for (int i = 0; i < 21; i++) {
        printw("-");
    }
    printw("+");
    wmove(stdscr, LIGNEMIN + 1, 130);
    printw("|     ");
    attron(COLOR_PAIR(game.turn.currentPlayer.color));
    printw("Joueur : %d", game.turn.currentPlayer.id);
    attroff(COLOR_PAIR(game.turn.currentPlayer.color));

    printw("      |");
    wmove(stdscr, LIGNEMIN + 2, 130);
    printw("+");
    for (int i = 0; i < 21; i++) {
        printw("-");
    }
    printw("+");

    // Affichage de la tuile actuelle
    if (game.turn.currentTile.tileType != 0) {
        // OUEST
        SetColor(game.turn.currentTile.cotes[3]);
        wmove(stdscr, LIGNEMIN + 5, 136 + 1);
        printw(" %c ", game.turn.currentTile.cotes[3]);
        RemoveColor();

        // NORD
        SetColor(game.turn.currentTile.cotes[0]);
        wmove(stdscr, LIGNEMIN + 4, 136 + 4);
        printw(" %c ", game.turn.currentTile.cotes[0]);
        RemoveColor();

        // CENTRE
        SetColor(game.turn.currentTile.centre);
        wmove(stdscr, LIGNEMIN + 5, 136 + 4);
        printw(" %c ", game.turn.currentTile.centre);
        RemoveColor();

        // SUD
        SetColor(game.turn.currentTile.cotes[2]);
        wmove(stdscr, LIGNEMIN + 6, 136 + 4);
        printw(" %c ", game.turn.currentTile.cotes[2]);
        RemoveColor();

        // EST
        SetColor(game.turn.currentTile.cotes[1]);
        wmove(stdscr, LIGNEMIN + 5, 136 + 7);
        printw(" %c ", game.turn.currentTile.cotes[1]);
        RemoveColor();
    }

    for (int i = 0; i < 5; i++) {
        wmove(stdscr, LIGNEMIN + 3 + i, 130);
        printw("|");
        wmove(stdscr, LIGNEMIN + 3 + i, 152);
        printw("|");
    }

    // Affichage du "panneau" mode
    game.turn.currentMode = Rotation;
    wmove(stdscr, LIGNEMIN + 8, 130);
    printw("+");
    for (int i = 0; i < 21; i++) {
        printw("-");
    }
    printw("+");
    wmove(stdscr, LIGNEMIN + 9, 130);
    printw("|");
    char* currentMode = CurrentMode(game);
    for (int i = 0; i < 7 - 0.5 * strlen(currentMode); i++) {
        printw(" ");
    }
    attron(COLOR_PAIR(game.turn.currentPlayer.color));
    printw("Mode : %s", currentMode);
    attroff(COLOR_PAIR(game.turn.currentPlayer.color));
    for (int i = 0; i < 7 - 0.5 * strlen(currentMode); i++) {
        printw(" ");
    }
    printw("|");
    wmove(stdscr, LIGNEMIN + 10, 130);
    printw("+");
    for (int i = 0; i < 21; i++) {
        printw("-");
    }
    printw("+");
}

// DEBUG CETTE FONCTION
GameStruct CanBePlaced(GameStruct game) {
    for (int x = 0; x < 143; x++) {
        for (int y = 0; y < 143; y++) {
            if (game.grid[x][y].tileType == 1) continue;
            // faire les coins

            // NORD
            if (game.grid[x][y - 1].tileType == 0) {
                if (game.grid[x][y].cotes[0] ==
                    game.turn.currentTile.cotes[2]) {
                    game.grid[x][y - 1].tileType = 2;
                }
                // else{
                //     // game.grid[i][j-1].tileType = 0;
                // }
            }

            // SUD
            if (game.grid[x][y + 1].tileType == 0) {
                if (game.grid[x][y].cotes[2] ==
                    game.turn.currentTile.cotes[0]) {
                    game.grid[x][y + 1].tileType = 2;
                }
                // else{
                //     // game.grid[i][j+1].tileType = 0;
                // }
            }

            // EST
            if (game.grid[x + 1][y].tileType == 0) {
                if (game.grid[x][y].cotes[1] ==
                    game.turn.currentTile.cotes[3]) {
                    game.grid[x + 1][y].tileType = 2;
                }
                // else{
                //     // game.grid[i+1][j].tileType = 0;
                // }
            }

            // OUEST
            if (game.grid[x - 1][y].tileType == 0) {
                if (game.grid[x][y].cotes[3] ==
                    game.turn.currentTile.cotes[1]) {
                    game.grid[x - 1][y].tileType = 2;
                }
                // else{
                //     // game.grid[i-1][j].tileType = 0;
                // }
            }
        }
    }

    return game;
}