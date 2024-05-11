#include "draw_functions.h"

#include "tile_functions.h"

void RemoveTile(TileStruct* pile) {
    for (int i = 0; i < 71; i++) {
        pile[i] = pile[i + 1];
    }
    pile[71] = (TileStruct){{'0', '0', '0', '0'}, '0', 0};  // tuile vide
}

GameStruct DrawTile(GameStruct game) {
    int canBePlaced = 0;
    TileStruct t;
    do {
        t = game.pile[0];
        for (int i = 0; i < 4; i++) {
            t = RotateTile(t, 1);
            canBePlaced += CheckCanBePlaced(game, t);
        }

        if (canBePlaced == 0) {
            ReorderPile(game.pile);
        }
    } while (canBePlaced == 0);

    return game;
}

void ReorderPile(TileStruct* pile) {
    TileStruct temp = pile[0];
    RemoveTile(pile);
    int index = 0;
    while (pile[index].tileType != 0) {
        index++;
    }
    pile[index] = temp;
}

int CheckCanBePlaced(GameStruct game, TileStruct t) {
    for (int y = 0; y < 143; y++) {
        for (int x = 0; x < 143; x++) {
            if (game.grid[x][y].tileType == 0) {
                if ((game.grid[x][y - 1].tileType == 1) ||
                    (game.grid[x][y + 1].tileType == 1) ||
                    (game.grid[x - 1][y].tileType == 1) ||
                    (game.grid[x + 1][y].tileType == 1)) {
                    int northCond = 0, southCond = 0, eastCond = 0,
                        westCond = 0;
                    if (x - 1 >= 0) {  // La tuile à l'ouest
                        westCond = (game.grid[x - 1][y].tileType == 0) ||
                                   (game.grid[x - 1][y].cotes[1] == t.cotes[3]);
                    }
                    if (x + 1 < 143) {  // La tuile à l'est
                        eastCond = (game.grid[x + 1][y].tileType == 0) ||
                                   (game.grid[x + 1][y].cotes[3] == t.cotes[1]);
                    }
                    if (y - 1 >= 0) {  // La tuile au nord
                        northCond =
                            (game.grid[x][y - 1].tileType == 0) ||
                            (game.grid[x][y - 1].cotes[2] == t.cotes[0]);
                    }
                    if (y + 1 < 143) {  // La tuile au sud
                        southCond =
                            (game.grid[x][y + 1].tileType == 0) ||
                            (game.grid[x][y + 1].cotes[0] == t.cotes[2]);
                    }

                    if ((eastCond) && (westCond) && (southCond) &&
                        (northCond)) {
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}