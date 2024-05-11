#include "tile_functions.h"

#include "draw_functions.h"
#include "print_functions.h"

TurnStruct MoveCamera(TurnStruct turn, int ch) {
    if (ch == KEY_DOWN && turn.coordYMin > -71) turn.coordYMin--;
    if (ch == KEY_UP && turn.coordYMin < 61) turn.coordYMin++;
    if (ch == KEY_RIGHT && turn.coordXMin > -71) turn.coordXMin--;
    if (ch == KEY_LEFT && turn.coordXMin < 61) turn.coordXMin++;

    return turn;
}

TileStruct RotateTile(TileStruct t, int ch) {
    char temp;
    int sens = 0;

    if (ch == KEY_LEFT) sens = -1;
    if (ch == KEY_RIGHT) sens = 1;

    if (sens > 0) {
        temp = t.cotes[3];
        t.cotes[3] = t.cotes[2];
        t.cotes[2] = t.cotes[1];
        t.cotes[1] = t.cotes[0];
        t.cotes[0] = temp;
    } else if (sens < 0) {
        temp = t.cotes[0];
        t.cotes[0] = t.cotes[1];
        t.cotes[1] = t.cotes[2];
        t.cotes[2] = t.cotes[3];
        t.cotes[3] = temp;
    }

    return t;
}

void ReturnCoordsList(GameStruct game, CoordStruct* coords) {
    for (int i = 0; i < 144; i++) {
        coords[i] = (CoordStruct){-1, -1};  // coordonnées vides;
    }

    int index = 0;
    for (int x = 0; x < 143; x++) {
        for (int y = 0; y < 143; y++) {
            if (game.grid[y][x].tileType == 2) {
                coords[index] = (CoordStruct){y, x};
                index++;
            }
        }
    }
}

GameStruct ChangeNumbers(GameStruct game) {
    CoordStruct coords[144];
    ReturnCoordsList(game, coords);

    int index = game.turn.tileIndex;
    for (int i = 0; i < 144; i++) {
        if (coords[i].x == -1) break;

        game.grid[coords[i].x][coords[i].y].cotes[0] = '0';
        game.grid[coords[i].x][coords[i].y].cotes[1] = '0';
        game.grid[coords[i].x][coords[i].y].cotes[2] = '0';
        game.grid[coords[i].x][coords[i].y].cotes[3] = '0';
        game.grid[coords[i].x][coords[i].y].centre = '0';
    }

    char playerNb[100];
    sprintf(playerNb, "%d", game.turn.currentPlayer.id);
    game.grid[coords[index].x][coords[index].y].cotes[0] = playerNb[0];
    game.grid[coords[index].x][coords[index].y].cotes[1] = playerNb[0];
    game.grid[coords[index].x][coords[index].y].cotes[2] = playerNb[0];
    game.grid[coords[index].x][coords[index].y].cotes[3] = playerNb[0];
    game.grid[coords[index].x][coords[index].y].centre = playerNb[0];

    return game;
}

GameStruct PlaceTile(GameStruct game, int ch) {
    CoordStruct coords[144];
    ReturnCoordsList(game, coords);
    int index = game.turn.tileIndex;

    if (ch == KEY_LEFT) {
        index--;
        if (index < 0) {
            index = 143;
        }

        while (coords[index].x == -1) {
            index--;
            if (index < 0) {
                index = 143;
            }
        }
    }

    if (ch == KEY_RIGHT) {
        index++;
        if (index > 143) {
            index = 0;
        }

        // necessaire ?
        while (coords[index].x == -1) {
            index++;
            if (index > 143) {
                index = 0;
            }
        }
    }

    if (ch == ' ') {
        int x = -1, y = -1;

        x = coords[index].x;
        y = coords[index].y;
        if (x > -1 && y > -1) {
            game = ResetCanBePlaced(game);
            if (game.turn.currentPlayer.nbPions > 0) {
                game.turn.currentState = Pawn;
                game.turn.currentMode = Question;
            }
            game.turn.currentX = x;
            game.turn.currentY = y;
            game.grid[x][y] = game.turn.currentTile;
        }
    }

    game.turn.tileIndex = index;
    return game;
}