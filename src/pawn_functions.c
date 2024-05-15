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

int CheckIfPlaceable(GameStruct game) {
    if (game.pawns[25].side == 4) {
        if (game.grid[game.turn.currentX][game.turn.currentY].centre == 'p' || game.grid[game.turn.currentX][game.turn.currentY].centre == 'f') {
            return 0;
        } else if (game.grid[game.turn.currentX][game.turn.currentY].centre == 'r') {
            if (CheckIfPlaceableOnRoadLoop(game, (CoordStruct){game.turn.currentX, game.turn.currentY}) == 0) return 0;
            if (CheckIfPlaceableOnRoad(game, (CoordStruct){game.turn.currentX, game.turn.currentY}) == 0) return 0;
        }
    } else {
        if (game.grid[game.turn.currentX][game.turn.currentY].cotes[game.pawns[25].side] == 'p' || game.grid[game.turn.currentX][game.turn.currentY].cotes[game.pawns[25].side] == 'f') {
            return 0;
        } else if (game.grid[game.turn.currentX][game.turn.currentY].cotes[game.pawns[25].side] == 'r') {
            if (CheckIfPlaceableOnRoadLoop(game, (CoordStruct){game.turn.currentX, game.turn.currentY}) == 0) return 0;
            if (CheckIfPlaceableOnRoad(game, (CoordStruct){game.turn.currentX, game.turn.currentY}) == 0) return 0;
        }
    }

    return 1;
}

int CheckIfPlaceableOnRoadLoop(GameStruct game, CoordStruct coords) {
    int x = coords.x;
    int y = coords.y;
    int size = 0;

    int previousDirection = 4;  // on place la direction au centre
    TileStruct tuile;

    // Choisir vers où on va partir
    for (int i = 0; i < 4; i++) {
        if (game.grid[x][y].cotes[i] == 'r') {
            previousDirection = i;
            break;
        }
    }

    game.grid[x][y].visited = 1;  // Première tuile visitée

    // Commencer le parcours
    if (previousDirection == 0) {
        y--;
    } else if (previousDirection == 1) {
        x++;
    } else if (previousDirection == 2) {
        y++;
    } else if (previousDirection == 3) {
        x--;
    }

    // Boucle de parcours
    while (game.grid[x][y].centre == 'r' && game.grid[x][y].visited == 0) {
        tuile = game.grid[x][y];
        game.grid[x][y].visited = 1;  // tuile visitée

        for (int p = 0; p < 25; p++) {
            if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 4) return 0;
            if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == (previousDirection + 2) % 4 && tuile.cotes[(previousDirection + 2) % 4] == 'r') return 0;
        }

        if (tuile.cotes[0] == 'r' && previousDirection != 2) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 0) return 0;
            }
            previousDirection = 0;
            y = y - 1;
            size++;
        } else if (tuile.cotes[1] == 'r' && previousDirection != 3) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 1) return 0;
            }
            previousDirection = 1;
            x = x + 1;
            size++;
        } else if (tuile.cotes[2] == 'r' && previousDirection != 0) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 2) return 0;
            }
            previousDirection = 2;
            y = y + 1;
            size++;
        } else if (tuile.cotes[3] == 'r' && previousDirection != 1) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 3) return 0;
            }
            previousDirection = 3;
            x = x - 1;
            size++;
        }
    }

    if (size == 0 || game.grid[x][y].centre != 'r') return 1;

    size++;

    return 1;
}

int CheckIfPlaceableOnRoad(GameStruct game, CoordStruct coords) {
    int x = coords.x;
    int y = coords.y;
    int size = 0;

    int previousDirection = 4;  // on place la direction au centre
    TileStruct tuile;

    CoordStruct extremites = (CoordStruct){-1, -1};

    PawnStruct pawn;
    for (int i = 0; i < 26; i++) {
        if (game.pawns[i].coords.x == coords.x && game.pawns[i].coords.y == coords.y) {
            pawn = game.pawns[i];
        }
    }

    previousDirection = pawn.side;

    if (game.grid[x][y].centre != 'r') {
        if (previousDirection == 0) {
            y--;
        } else if (previousDirection == 1) {
            x++;
        } else if (previousDirection == 2) {
            y++;
        } else if (previousDirection == 3) {
            x--;
        }
    }

    // Boucle de parcours, si c'est pas une extrémité
    while (game.grid[x][y].centre == 'r') {
        tuile = game.grid[x][y];

        if (tuile.cotes[0] == 'r' && previousDirection != 2) {
            previousDirection = 0;
            y = y - 1;
        } else if (tuile.cotes[1] == 'r' && previousDirection != 3) {
            previousDirection = 1;
            x = x + 1;
        } else if (tuile.cotes[2] == 'r' && previousDirection != 0) {
            previousDirection = 2;
            y = y + 1;
        } else if (tuile.cotes[3] == 'r' && previousDirection != 1) {
            previousDirection = 3;
            x = x - 1;
        }
    }

    extremites = (CoordStruct){x, y};
    int extremitesSide = previousDirection;

    // Demi tour
    if (previousDirection == 0) {
        y++;
        previousDirection = 2;
    } else if (previousDirection == 1) {
        x--;
        previousDirection = 3;
    } else if (previousDirection == 2) {
        y--;
        previousDirection = 0;
    } else if (previousDirection == 3) {
        x++;
        previousDirection = 1;
    }

    tuile = game.grid[x][y];

    while (tuile.centre == 'r') {
        for (int p = 0; p < 25; p++) {
            if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 4) return 0;
            if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == (previousDirection + 2) % 4) return 0;
        }

        if (tuile.cotes[0] == 'r' && previousDirection != 2) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 0) return 0;
            }
            previousDirection = 0;
            y = y - 1;
            size++;
        } else if (tuile.cotes[1] == 'r' && previousDirection != 3) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 1) return 0;
            }
            previousDirection = 1;
            x = x + 1;
            size++;
        } else if (tuile.cotes[2] == 'r' && previousDirection != 0) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 2) return 0;
            }
            previousDirection = 2;
            y = y + 1;
            size++;
        } else if (tuile.cotes[3] == 'r' && previousDirection != 1) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 3) return 0;
            }
            previousDirection = 3;
            x = x - 1;
            size++;
        }

        tuile = game.grid[x][y];
    }

    if (extremites.x == x && extremites.y == y) {
        size++;

    } else {
        size += 2;
        for (int p = 0; p < 25; p++) {
            if (game.pawns[p].coords.x == extremites.x && game.pawns[p].coords.y == extremites.y && game.pawns[p].side == extremitesSide) return 0;
            if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == (previousDirection + 2) % 4) return 0;
        }
    }

    return 1;
}