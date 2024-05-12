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
    if (CheckIfPlaceable(game)) {
        // game = AddBuild(game);

        int index = 0;
        while (game.pawns[index].idPlayers != -1) {
            index++;
        }
        game.pawns[index] = game.pawns[25];
        game.pawns[25] = (PawnStruct){(CoordStruct){-1, -1}, -1, -1};  // pion vide

        game.turn.turnEnd = 1;

        game.playerList[game.turn.currentPlayer.id - 1].nbPions--;
    }

    return game;
}

GameStruct RemovePawn(GameStruct game, PawnStruct pawn) {
    int index = 0;
    while (game.pawns[index].coords.x != pawn.coords.x || game.pawns[index].coords.y != pawn.coords.y) {
        index++;
    }

    for (int i = index; i < 25; i++) {
        game.pawns[i] = game.pawns[i + 1];
    }

    game.pawns[25] = (PawnStruct){(CoordStruct){-1, -1}, -1, -1};  // pion vide

    return game;
}

// GameStruct AddBuild(GameStruct game) {
//     PawnStruct pawn = game.pawns[25];
//     TileStruct tile = game.grid[game.turn.currentX][game.turn.currentY];
//     char sideType;
//     if (pawn.side == 4) {
//         sideType = tile.centre;
//     } else {
//         sideType = tile.cotes[pawn.side];
//     }
//     BuildingStruct building = {pawn.coords, sideType, 1, pawn.side};

//     int index = 0;
//     while (game.buildings[index].coords.x != -1) {
//         index++;
//     }

//     game.buildings[index] = building;

//     return game;
// }

int CheckIfPlaceable(GameStruct game) {
    if (game.pawns[25].side == 4) {
        if (game.grid[game.turn.currentX][game.turn.currentY].centre == 'p' || game.grid[game.turn.currentX][game.turn.currentY].centre == 'f') {
            return 0;
        }
    } else {
        if (game.grid[game.turn.currentX][game.turn.currentY].cotes[game.pawns[25].side] == 'p' || game.grid[game.turn.currentX][game.turn.currentY].cotes[game.pawns[25].side] == 'f') {
            return 0;
        }
    }

    return 1;
}