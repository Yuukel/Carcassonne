#include "end_functions.h"

#include "draw_functions.h"
#include "pawn_functions.h"

GameStruct InitTurn(GameStruct game) {
    game = DrawTile(game);

    TurnStruct turn = game.turn;

    if (turn.currentPlayer.id == game.nbPlayers) turn.currentPlayer.id = 0;

    turn.currentPlayer = game.playerList[turn.currentPlayer.id];
    turn.currentState = Draw;
    turn.currentTile = game.pile[0];
    turn.currentMode = Camera;
    turn.turnEnd = 0;
    turn.tileIndex = 0;

    game.turn = turn;

    return game;
}

GameStruct EndTurn(GameStruct game) {
    int index = 0;
    while (game.pawns[index].coords.x != -1) {
        char side;
        if (game.pawns[index].side == 4) {
            side = game.grid[game.pawns[index].coords.x][game.pawns[index].coords.y].centre;
        } else {
            side = game.grid[game.pawns[index].coords.x][game.pawns[index].coords.y].cotes[game.pawns[index].side];
        }

        if (side == 'a') {
            if (RouteAbbey(game, game.pawns[index].coords)) {
                game = AddScore(game, game.pawns[index], 9);
                index++;
            } else {
                index++;
            }
        } else if (side == 'r') {
            int val = 0;
            val = RouteRoad(game, game.pawns[index].coords);
            if (val > 0) {
                game = AddScore(game, game.pawns[index], val);
                index++;
            } else {
                index++;
            }
        } else {
            index++;
        }
    }

    return game;
}

GameStruct AddScore(GameStruct game, PawnStruct pawn, int score) {
    game.playerList[pawn.idPlayers - 1].score += score;

    int index = 0;
    while (game.pawns[index].coords.x != pawn.coords.x || game.pawns[index].coords.y != pawn.coords.y) {
        index++;
    }
    game = RemovePawn(game, game.pawns[index]);
    game.playerList[pawn.idPlayers - 1].nbPions++;

    return game;
}

int RouteAbbey(GameStruct game, CoordStruct coords) {
    int x = coords.x;
    int y = coords.y;

    if (game.grid[x - 1][y - 1].tileType != 1 || game.grid[x][y - 1].tileType != 1 || game.grid[x + 1][y - 1].tileType != 1 || game.grid[x - 1][y].tileType != 1 || game.grid[x + 1][y].tileType != 1 || game.grid[x - 1][y + 1].tileType != 1 || game.grid[x][y + 1].tileType != 1 || game.grid[x + 1][y + 1].tileType != 1) {
        return 0;
    }

    return 1;
}

int RouteRoad(GameStruct game, CoordStruct coords) {
    int x = coords.x;
    int y = coords.y;
    int size = 0;

    int side = 4;
    TileStruct tuile;
    CoordStruct coordsList[72];
    int index = 0;
    int already = 0;
    while (game.grid[x][y].centre == 'r' && already == 0) {
        tuile = game.grid[x][y];

        coordsList[index] = (CoordStruct){x, y};
        index++;

        if (tuile.cotes[0] == 'r' && side != 2) {
            side = 0;
            y = y - 1;
        } else if (tuile.cotes[1] == 'r' && side != 3) {
            side = 1;
            x = x + 1;
        } else if (tuile.cotes[2] == 'r' && side != 0) {
            side = 2;
            y = y + 1;
        } else if (tuile.cotes[3] == 'r' && side != 1) {
            side = 3;
            x = x - 1;
        }

        for (int i = 0; i < index; i++) {
            if (coordsList[i].x == x && coordsList[i].y == y) {
                already = 1;
            }
        }
    }

    if (game.grid[x][y].tileType == 0) return 0;

    if (side == 2) {
        side = 0;
        y = y - 1;
    } else if (side == 3) {
        side = 1;
        x = x + 1;
    } else if (side == 0) {
        side = 2;
        y = y + 1;
    } else if (side == 1) {
        side = 3;
        x = x - 1;
    }

    tuile = game.grid[x][y];

    index = 0;
    already = 0;
    while (tuile.centre == 'r' && already == 0) {
        coordsList[index] = (CoordStruct){x, y};
        index++;

        if (tuile.cotes[0] == 'r' && side != 2) {
            side = 0;
            y = y - 1;
            size++;
        } else if (tuile.cotes[1] == 'r' && side != 3) {
            side = 1;
            x = x + 1;
            size++;
        } else if (tuile.cotes[2] == 'r' && side != 0) {
            side = 2;
            y = y + 1;
            size++;
        } else if (tuile.cotes[3] == 'r' && side != 1) {
            side = 3;
            x = x - 1;
            size++;
        }
        tuile = game.grid[x][y];

        for (int i = 0; i < index; i++) {
            if (coordsList[i].x == x && coordsList[i].y == y) {
                already = 1;
            }
        }
    }

    if (game.grid[x][y].tileType == 0) return 0;

    if (already == 0) size += 2;

    int val = CheckPawnAmount(game, coords, coordsList);
    if (val == 1)
        return size;
    else
        return 0;
}

int CheckPawnAmount(GameStruct game, CoordStruct coords, CoordStruct coordsList[72]) {
    PawnStruct pawn;
    for (int i = 0; i < 26; i++) {
        if (game.pawns[i].coords.x == coords.x && game.pawns[i].coords.y == coords.y) {
            pawn = game.pawns[i];
        }
    }

    int pawnsAmount[5];
    for (int i = 0; i < 72; i++) {
        for (int j = 0; j < 26; j++) {
            if (coordsList[i].x == game.pawns[j].coords.x && coordsList[i].y == game.pawns[j].coords.y) {
                pawnsAmount[game.pawns[j].idPlayers - 1]++;
            }
        }
    }

    int max = pawnsAmount[0];
    for (int i = 1; i < 5; i++) {
        if (pawnsAmount[i] > max) {
            max = pawnsAmount[i];
        }
    }

    if (pawnsAmount[pawn.idPlayers - 1] == max) {
        return 1;
    } else {
        return 0;
    }
}