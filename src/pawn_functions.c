#include "pawn_functions.h"

#include <ncurses.h>

GameStruct ChoosePawnPosition(GameStruct game, int x, int y, int ch) {
    CoordStruct coords = {x, y};

    int side = game.turn.currentSide;

    if (ch == KEY_LEFT) {
        if (side == 1) {
            side = 4;
        } else {
            side = 3;
        }
    }

    if (ch == KEY_RIGHT) {
        if (side == 3) {
            side = 4;
        } else {
            side = 1;
        }
    }

    if (ch == KEY_UP) {
        if (side == 2) {
            side = 4;
        } else {
            side = 0;
        }
    }

    if (ch == KEY_DOWN) {
        if (side == 0) {
            side = 4;
        } else {
            side = 2;
        }
    }

    game.turn.currentSide = side;

    return PlacePawn(game, coords, side);
}

GameStruct PlacePawn(GameStruct game, CoordStruct coords, int side) {
    PawnStruct pawn = (PawnStruct){coords, game.turn.currentPlayer.id, side};
    game.pawns[25] = pawn;

    return game;
}

GameStruct AddPawn(GameStruct game) {
    int index = 0;
    while (game.pawns[index].idPlayers != -1) {
        index++;
    }
    game.pawns[index] = game.pawns[25];
    game.pawns[25] = (PawnStruct){(CoordStruct){-1, -1}, -1, -1};  // pion vide

    game.turn.turnEnd = 1;
    game.turn.currentState = Tile;

    game.playerList[game.turn.currentPlayer.id - 1].nbPions--;

    return game;
}