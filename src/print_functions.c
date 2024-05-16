#include "print_functions.h"

#include <ncurses.h>
#include <string.h>

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

void PrintPlayers(GameStruct game) {
    for (int j = 0; j < game.nbPlayers; j++) {
        printw("+");
        for (int i = 0; i < 19; i++) {
            printw("-");
        }
        printw("+");

        printw("\t\t\t");
    }
    printw("\n");
    for (int j = 0; j < game.nbPlayers; j++) {
        printw("| ");
        attron(COLOR_PAIR(game.playerList[j].color));
        printw("Joueur : %d", j + 1);
        attroff(COLOR_PAIR(game.playerList[j].color));
        printw(" = %4d", game.playerList[j].score);
        printw(" |");

        printw("\t\t\t");
    }
    printw("\n");
    for (int j = 0; j < game.nbPlayers; j++) {
        printw("|   ");
        for (int i = 0; i < game.playerList[j].nbPions; i++) {
            attron(COLOR_PAIR(game.playerList[j].color));
            printw("o ");
            attroff(COLOR_PAIR(game.playerList[j].color));
        }
        wmove(stdscr, 2, 40 * j + 20);
        printw("|");
        printw("\t\t\t");
    }
    printw("\n");
    for (int j = 0; j < game.nbPlayers; j++) {
        printw("+");
        for (int i = 0; i < 19; i++) {
            printw("-");
        }
        printw("+");

        printw("\t\t\t");
    }
    printw("\n\n");
    refresh();
}

void PrintTile(TileStruct t, int i, int j, PawnStruct pawns[26], int x, int y) {
    if (t.tileType != 0) {
        char c[5] = {' ', ' ', ' ', ' ', ' '};
        if (t.tileType == 2 && t.centre != '0') {
            c[0] = t.cotes[0];
            c[1] = t.cotes[1];
            c[2] = t.cotes[2];
            c[3] = t.cotes[3];
            c[4] = t.centre;
        }

        for (int index = 0; index < 26; index++) {
            PawnStruct pawn = pawns[index];
            if (pawn.coords.x == x && pawn.coords.y == y) {
                char buffer[100];
                sprintf(buffer, "%d", pawn.idPlayers);
                c[pawn.side] = buffer[0];
                break;
            }
        }

        // OUEST
        SetColor(t.cotes[3]);
        wmove(stdscr, LIGNEMINTUILE + 1 + (j * 3), COLONNEMINTUILE + (i * 9));
        printw(" %c ", c[3]);
        RemoveColor();

        // NORD
        SetColor(t.cotes[0]);
        wmove(stdscr, LIGNEMINTUILE + (j * 3), COLONNEMINTUILE + 3 + (i * 9));
        printw(" %c ", c[0]);
        RemoveColor();

        // CENTRE
        SetColor(t.centre);
        wmove(stdscr, LIGNEMINTUILE + 1 + (j * 3), COLONNEMINTUILE + 3 + (i * 9));
        printw(" %c ", c[4]);
        RemoveColor();

        // SUD
        SetColor(t.cotes[2]);
        wmove(stdscr, LIGNEMINTUILE + 2 + (j * 3), COLONNEMINTUILE + 3 + (i * 9));
        printw(" %c ", c[2]);
        RemoveColor();

        // EST
        SetColor(t.cotes[1]);
        wmove(stdscr, LIGNEMINTUILE + 1 + (j * 3), COLONNEMINTUILE + 6 + (i * 9));
        printw(" %c ", c[1]);
        RemoveColor();
    }
}

void PrintGrid(GameStruct game, int coordXMin, int coordYMin) {
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
            PrintTile(game.grid[coordXMin + i + 71][coordYMin + j + 71], i, j, game.pawns, coordXMin + i + 71, coordYMin + j + 71);
        }
    }
}

char* CurrentMode(TurnStruct turn) {
    if (turn.currentMode == Rotation)
        return "Rotation";
    else if (turn.currentMode == Camera)
        return "Camera";
    else if (turn.currentMode == Pose)
        return "Pose";
    else if (turn.currentMode == Question)
        return "Question";
    else
        return "Pion";
}

void PrintTurnInfos(TurnStruct turn) {
    // Affichage du "panneau" joueur
    wmove(stdscr, LIGNEMIN, 130);
    printw("+");
    for (int i = 0; i < 21; i++) {
        printw("-");
    }
    printw("+");
    wmove(stdscr, LIGNEMIN + 1, 130);
    printw("|     ");
    attron(COLOR_PAIR(turn.currentPlayer.color));
    printw("Joueur : %d", turn.currentPlayer.id);
    attroff(COLOR_PAIR(turn.currentPlayer.color));

    printw("      |");
    wmove(stdscr, LIGNEMIN + 2, 130);
    printw("+");
    for (int i = 0; i < 21; i++) {
        printw("-");
    }
    printw("+");

    // Affichage de la tuile actuelle
    if (turn.currentTile.tileType != 0) {
        // OUEST
        SetColor(turn.currentTile.cotes[3]);
        wmove(stdscr, LIGNEMIN + 5, 136 + 1);
        printw("   ");
        RemoveColor();

        // NORD
        SetColor(turn.currentTile.cotes[0]);
        wmove(stdscr, LIGNEMIN + 4, 136 + 4);
        printw("   ");
        RemoveColor();

        // CENTRE
        SetColor(turn.currentTile.centre);
        wmove(stdscr, LIGNEMIN + 5, 136 + 4);
        printw("   ");
        RemoveColor();

        // SUD
        SetColor(turn.currentTile.cotes[2]);
        wmove(stdscr, LIGNEMIN + 6, 136 + 4);
        printw("   ");
        RemoveColor();

        // EST
        SetColor(turn.currentTile.cotes[1]);
        wmove(stdscr, LIGNEMIN + 5, 136 + 7);
        printw("   ");
        RemoveColor();
    }

    for (int i = 0; i < 5; i++) {
        wmove(stdscr, LIGNEMIN + 3 + i, 130);
        printw("|");
        wmove(stdscr, LIGNEMIN + 3 + i, 152);
        printw("|");
    }

    // Affichage du "panneau" mode
    wmove(stdscr, LIGNEMIN + 8, 130);
    printw("+");
    for (int i = 0; i < 21; i++) {
        printw("-");
    }
    printw("+");
    wmove(stdscr, LIGNEMIN + 9, 130);
    printw("|");
    char* currentMode = CurrentMode(turn);
    for (int i = 0; i < 7 - 0.5 * strlen(currentMode); i++) {
        printw(" ");
    }
    attron(COLOR_PAIR(turn.currentPlayer.color));
    printw("Mode : %s", currentMode);
    attroff(COLOR_PAIR(turn.currentPlayer.color));
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

    wmove(stdscr, LIGNEMIN + 11, 131);
    printw("Tuiles restantes : %d", turn.nbTileInPile);

    wmove(stdscr, LIGNEMIN + 12, 135);
    attron(COLOR_PAIR(7));
    printw("   ");
    attroff(COLOR_PAIR(7));
    printw(" = route");

    wmove(stdscr, LIGNEMIN + 13, 135);
    attron(COLOR_PAIR(8));
    printw("   ");
    attroff(COLOR_PAIR(8));
    printw(" = pre");

    wmove(stdscr, LIGNEMIN + 14, 135);
    attron(COLOR_PAIR(9));
    printw("   ");
    attroff(COLOR_PAIR(9));
    printw(" = ville");

    wmove(stdscr, LIGNEMIN + 15, 135);
    attron(COLOR_PAIR(10));
    printw("   ");
    attroff(COLOR_PAIR(10));
    printw(" = blason");

    wmove(stdscr, LIGNEMIN + 16, 135);
    attron(COLOR_PAIR(11));
    printw("   ");
    attroff(COLOR_PAIR(11));
    printw(" = abbaye");

    wmove(stdscr, LIGNEMIN + 17, 135);
    attron(COLOR_PAIR(12));
    printw("   ");
    attroff(COLOR_PAIR(12));
    printw(" = village");

    TileStruct tile = (TileStruct){{'0', '0', '0', '0'}, '0', 2};
    if (tile.tileType != 0) {
        // OUEST
        SetColor(tile.cotes[3]);
        wmove(stdscr, LIGNEMIN + 20, 132);
        printw("   ");
        RemoveColor();

        // NORD
        SetColor(tile.cotes[0]);
        wmove(stdscr, LIGNEMIN + 19, 135);
        printw("   ");
        RemoveColor();

        // CENTRE
        SetColor(tile.centre);
        wmove(stdscr, LIGNEMIN + 20, 135);
        printw("   ");
        RemoveColor();

        // SUD
        SetColor(tile.cotes[2]);
        wmove(stdscr, LIGNEMIN + 21, 135);
        printw("   ");
        RemoveColor();

        // EST
        SetColor(tile.cotes[1]);
        wmove(stdscr, LIGNEMIN + 20, 138);
        printw("   ");
        RemoveColor();
    }

    wmove(stdscr, LIGNEMIN + 20, 141);
    printw(" = emplacement");
}

// affichage des commandes
void PrintCommands(TurnStruct turn, GameStruct game) {
    if (turn.currentState == Tile) {
        wmove(stdscr, 42, 0);
        printw("C : Changer le mode en \"Camera\"     R : Changer le mode en \"Rotation\"      P : Changer le mode en \"Pose\"");
        wmove(stdscr, 43, 0);
        if (turn.currentMode == Rotation)
            printw("< : Tourner la tuile vers la gauche     > : Tourner la tuile vers la droite");
        else if (turn.currentMode == Camera)
            printw("< ^ v > : Deplacer la camera dans le sens voulu");
        else if (turn.currentMode == Pose)
            printw("< > : Changer l'emplacement ou vous voulez poser la tuile     Espace : Poser la tuile");
    } else if (turn.currentState == Pawn) {
        wmove(stdscr, 42, 0);
        if (turn.currentMode == Question) {
            printw("Voulez-vous poser un pion sur cette tuile ?");
            wmove(stdscr, 43, 0);
            printw("Espace : Oui     X : Non");
        } else if (turn.currentMode == Pion)
            printw("< ^ v > : Deplacer le pion dans le sens voulu     Espace : Poser le pion     X : Annuler");
    }
    // wmove(stdscr, 44, 0);
    // printw("%d %d %d %d %d", game.pawnssss[0], game.pawnssss[1], game.pawnssss[2], game.pawnssss[3], game.pawnssss[4]);
}

GameStruct ResetCanBePlaced(GameStruct game) {
    for (int x = 0; x < 143; x++) {
        for (int y = 0; y < 143; y++) {
            if (game.grid[x][y].tileType == 2) {
                game.grid[x][y].tileType = 0;
            }
        }
    }

    return game;
}

GameStruct CanBePlaced(GameStruct game) {
    game = ResetCanBePlaced(game);
    for (int y = 0; y < 143; y++) {
        for (int x = 0; x < 143; x++) {
            if (game.grid[x][y].tileType == 0) {
                if ((game.grid[x][y - 1].tileType == 1) ||
                    (game.grid[x][y + 1].tileType == 1) ||
                    (game.grid[x - 1][y].tileType == 1) ||
                    (game.grid[x + 1][y].tileType == 1)) {
                    int northCond = 0, southCond = 0, eastCond = 0, westCond = 0;
                    if (x - 1 >= 0) {  // La tuile à l'ouest
                        westCond = (game.grid[x - 1][y].tileType == 0) || (game.grid[x - 1][y].cotes[1] == game.turn.currentTile.cotes[3] || (game.grid[x - 1][y].cotes[1] == 'b' && game.turn.currentTile.cotes[3] == 'v') || (game.grid[x - 1][y].cotes[1] == 'v' && game.turn.currentTile.cotes[3] == 'b'));
                        if (game.grid[x - 1][y].tileType == 2) westCond = 1;
                    }
                    if (x + 1 < 143) {  // La tuile à l'est
                        eastCond = (game.grid[x + 1][y].tileType == 0) || (game.grid[x + 1][y].cotes[3] == game.turn.currentTile.cotes[1] || (game.grid[x + 1][y].cotes[3] == 'b' && game.turn.currentTile.cotes[1] == 'v') || (game.grid[x + 1][y].cotes[3] == 'v' && game.turn.currentTile.cotes[1] == 'b'));
                        if (game.grid[x + 1][y].tileType == 2) eastCond = 1;
                    }
                    if (y - 1 >= 0) {  // La tuile au nord
                        northCond = (game.grid[x][y - 1].tileType == 0) || (game.grid[x][y - 1].cotes[2] == game.turn.currentTile.cotes[0] || (game.grid[x][y - 1].cotes[2] == 'b' && game.turn.currentTile.cotes[0] == 'v') || (game.grid[x][y - 1].cotes[2] == 'v' && game.turn.currentTile.cotes[0] == 'b'));
                        if (game.grid[x][y - 1].tileType == 2) northCond = 1;
                    }
                    if (y + 1 < 143) {  // La tuile au sud
                        southCond = (game.grid[x][y + 1].tileType == 0) || (game.grid[x][y + 1].cotes[0] == game.turn.currentTile.cotes[2] || (game.grid[x][y + 1].cotes[0] == 'b' && game.turn.currentTile.cotes[2] == 'v') || (game.grid[x][y + 1].cotes[0] == 'v' && game.turn.currentTile.cotes[2] == 'b'));
                        if (game.grid[x][y + 1].tileType == 2) southCond = 1;
                    }

                    if ((eastCond) && (westCond) && (southCond) && (northCond)) {
                        game.grid[x][y].tileType = 2;
                    }
                }
            }
        }
    }

    return game;
}