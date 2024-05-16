#include "end_functions.h"

#include <ncurses.h>

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
            if (PathAbbey(game, game.pawns[index].coords)) {
                game = AddScoreAbbey(game, game.pawns[index], 9);
                index++;
            } else {
                index++;
            }
        } else if (side == 'r') {
            int val = 0;
            val = PathRoadLoop(game, game.pawns[index].coords);
            if (val > 0) {
                game = AddScoreRoadLoop(game, game.pawns[index], val);
                index++;
            } else {
                val = PathRoad(game, game.pawns[index].coords);
                if (val == 0) {
                    index++;
                } else {
                    game = AddScoreRoad(game, game.pawns[index], val);
                }
            }
        } else if (side == 'v' || side == 'b') {
            int val = 0;
            val = 2 * PathScoreTown(game, game.pawns[index].coords, game.pawns[index].side);
            if (PathTown(game, game.pawns[index].coords, game.pawns[index].side)) {
                game = AddScoreTown(game, game.pawns[index], val);
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

GameStruct AddScoreAbbey(GameStruct game, PawnStruct pawn, int score) {
    game.playerList[pawn.idPlayers - 1].score += score;

    int index = 0;
    while (game.pawns[index].coords.x != pawn.coords.x || game.pawns[index].coords.y != pawn.coords.y) {
        index++;
    }
    game.playerList[pawn.idPlayers - 1].nbPions++;
    game = RemovePawn(game, game.pawns[index]);

    return game;
}

int PathAbbey(GameStruct game, CoordStruct coords) {
    int x = coords.x;
    int y = coords.y;

    if (game.grid[x - 1][y - 1].tileType != 1 || game.grid[x][y - 1].tileType != 1 || game.grid[x + 1][y - 1].tileType != 1 || game.grid[x - 1][y].tileType != 1 || game.grid[x + 1][y].tileType != 1 || game.grid[x - 1][y + 1].tileType != 1 || game.grid[x][y + 1].tileType != 1 || game.grid[x + 1][y + 1].tileType != 1) {
        return 0;
    }

    return 1;
}

GameStruct AddScoreRoad(GameStruct game, PawnStruct pawn, int score) {
    int x = pawn.coords.x;
    int y = pawn.coords.y;

    int previousDirection = 4;  // on place la direction au centre
    TileStruct tuile;

    previousDirection = pawn.side;

    PawnStruct pawnsList[25];
    int pawnIndex = 0;

    // PROBLEME EXTREMITES

    if (game.grid[x][y].centre != 'r') {
        for (int p = 0; p < 25; p++) {
            if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == previousDirection) {
                pawnsList[pawnIndex] = game.pawns[p];
                pawnIndex++;
            }
        }

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

    if (game.grid[x][y].centre != 'r') {
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
    }

    tuile = game.grid[x][y];

    while (tuile.centre == 'r') {
        for (int p = 0; p < 25; p++) {
            if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 4) {
                pawnsList[pawnIndex] = game.pawns[p];
                pawnIndex++;
            }
            if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == (previousDirection + 2) % 4) {
                pawnsList[pawnIndex] = game.pawns[p];
                pawnIndex++;
            }
        }

        if (tuile.cotes[0] == 'r' && previousDirection != 2) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 0) {
                    pawnsList[pawnIndex] = game.pawns[p];
                    pawnIndex++;
                }
            }
            previousDirection = 0;
            y = y - 1;
        } else if (tuile.cotes[1] == 'r' && previousDirection != 3) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 1) {
                    pawnsList[pawnIndex] = game.pawns[p];
                    pawnIndex++;
                }
            }
            previousDirection = 1;
            x = x + 1;
        } else if (tuile.cotes[2] == 'r' && previousDirection != 0) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 2) {
                    pawnsList[pawnIndex] = game.pawns[p];
                    pawnIndex++;
                }
            }
            previousDirection = 2;
            y = y + 1;
        } else if (tuile.cotes[3] == 'r' && previousDirection != 1) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 3) {
                    pawnsList[pawnIndex] = game.pawns[p];
                    pawnIndex++;
                }
            }
            previousDirection = 3;
            x = x - 1;
        }

        tuile = game.grid[x][y];
    }

    // PROBLEME EXTREMITES
    for (int p = 0; p < 25; p++) {
        if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == previousDirection) {
            pawnsList[pawnIndex] = game.pawns[p];
            pawnIndex++;
        }
    }

    int playersPawns[5];
    for (int i = 0; i < pawnIndex; i++) {
        playersPawns[pawnsList[i].idPlayers - 1] += 1;
    }

    int max = 0;
    for (int i = 0; i < 5; i++) {
        if (playersPawns[i] > max) max = playersPawns[i];
    }

    for (int i = 0; i < 5; i++) {
        if (playersPawns[i] == max) game.playerList[i].score += score;
    }

    for (int i = 0; i < pawnIndex; i++) {
        game.playerList[pawnsList[i].idPlayers - 1].nbPions++;
        game = RemovePawn(game, pawnsList[i]);
    }

    return game;
}

int PathRoad(GameStruct game, CoordStruct coords) {
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

    // Route incomplète
    if (game.grid[x][y].tileType == 0) return 0;

    extremites = (CoordStruct){x, y};

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
        if (tuile.cotes[0] == 'r' && previousDirection != 2) {
            previousDirection = 0;
            y = y - 1;
            size++;
        } else if (tuile.cotes[1] == 'r' && previousDirection != 3) {
            previousDirection = 1;
            x = x + 1;
            size++;
        } else if (tuile.cotes[2] == 'r' && previousDirection != 0) {
            previousDirection = 2;
            y = y + 1;
            size++;
        } else if (tuile.cotes[3] == 'r' && previousDirection != 1) {
            previousDirection = 3;
            x = x - 1;
            size++;
        }

        tuile = game.grid[x][y];
    }

    if (game.grid[x][y].tileType == 0) return 0;

    if (extremites.x == x && extremites.y == y)
        size++;
    else
        size += 2;

    return size;
}

GameStruct AddScoreRoadLoop(GameStruct game, PawnStruct pawn, int score) {
    int x = pawn.coords.x;
    int y = pawn.coords.y;

    int previousDirection = 4;  // on place la direction au centre
    TileStruct tuile;

    PawnStruct pawnsList[25];
    int pawnIndex = 1;

    pawnsList[0] = pawn;

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
            if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 4) {
                pawnsList[pawnIndex] = game.pawns[p];
                pawnIndex++;
            }
            if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == (previousDirection + 2) % 4) {
                pawnsList[pawnIndex] = game.pawns[p];
                pawnIndex++;
            }
        }

        if (tuile.cotes[0] == 'r' && previousDirection != 2) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 0) {
                    pawnsList[pawnIndex] = game.pawns[p];
                    pawnIndex++;
                }
            }
            previousDirection = 0;
            y = y - 1;
        } else if (tuile.cotes[1] == 'r' && previousDirection != 3) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 1) {
                    pawnsList[pawnIndex] = game.pawns[p];
                    pawnIndex++;
                }
            }
            previousDirection = 1;
            x = x + 1;
        } else if (tuile.cotes[2] == 'r' && previousDirection != 0) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 2) {
                    pawnsList[pawnIndex] = game.pawns[p];
                    pawnIndex++;
                }
            }
            previousDirection = 2;
            y = y + 1;
        } else if (tuile.cotes[3] == 'r' && previousDirection != 1) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 3) {
                    pawnsList[pawnIndex] = game.pawns[p];
                    pawnIndex++;
                }
            }
            previousDirection = 3;
            x = x - 1;
        }
    }

    int playersPawns[5];
    for (int i = 0; i < pawnIndex; i++) {
        playersPawns[pawnsList[i].idPlayers - 1] += 1;
    }

    int max = 0;
    for (int i = 0; i < 5; i++) {
        if (playersPawns[i] > max) max = playersPawns[i];
    }

    for (int i = 0; i < 5; i++) {
        if (playersPawns[i] == max) game.playerList[i].score += score;
    }

    for (int i = 0; i < pawnIndex; i++) {
        game.playerList[pawnsList[i].idPlayers - 1].nbPions++;
        game = RemovePawn(game, pawnsList[i]);
    }

    return game;
}

int PathRoadLoop(GameStruct game, CoordStruct coords) {
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

        if (tuile.cotes[0] == 'r' && previousDirection != 2) {
            previousDirection = 0;
            y = y - 1;
            size++;
        } else if (tuile.cotes[1] == 'r' && previousDirection != 3) {
            previousDirection = 1;
            x = x + 1;
            size++;
        } else if (tuile.cotes[2] == 'r' && previousDirection != 0) {
            previousDirection = 2;
            y = y + 1;
            size++;
        } else if (tuile.cotes[3] == 'r' && previousDirection != 1) {
            previousDirection = 3;
            x = x - 1;
            size++;
        }
    }

    if (game.grid[x][y].centre != 'r') return 0;
    size++;
    return size;
}

int PathTown(GameStruct game, CoordStruct coords, int side) {
    int x = coords.x;
    int y = coords.y;

    int isComplete = 1;
    game.grid[x][y].visited = 1;

    if (game.grid[x][y].centre != 'v' && game.grid[x][y].centre != 'b') {
        switch (side) {
            case 0:
                y--;
                break;
            case 1:
                x++;
                break;
            case 2:
                y++;
                break;
            case 3:
                x--;
                break;
            default:
                break;
        }
        if (game.grid[x][y].tileType == 0) return 0;
        game.grid[x][y].visited = 1;
    }

    CoordStruct coordsList[4] = {{x, y - 1}, {x + 1, y}, {x, y + 1}, {x - 1, y}};

    for (int i = 0; i < 4; i++) {
        if ((game.grid[x][y].cotes[i] == 'v' || game.grid[x][y].cotes[i] == 'b') && game.grid[coordsList[i].x][coordsList[i].y].visited == 0 && (game.grid[coordsList[i].x][coordsList[i].y].centre == 'v' || game.grid[coordsList[i].x][coordsList[i].y].centre == 'b')) {
            isComplete *= PathTown(game, coordsList[i], 4);
        } else if ((game.grid[x][y].cotes[i] == 'v' || game.grid[x][y].cotes[i] == 'b') && game.grid[coordsList[i].x][coordsList[i].y].tileType == 0)
            return 0;
    }

    return isComplete;
}

int PathScoreTown(GameStruct game, CoordStruct coords, int side) {
    int x = coords.x;
    int y = coords.y;

    int val = 1;
    game.grid[x][y].visited = 1;

    for (int i = 0; i < 4; i++) {
        if (game.grid[x][y].cotes[i] == 'b') {
            val++;
            break;
        }
    }

    if (game.grid[x][y].centre != 'v' && game.grid[x][y].centre != 'b') {
        switch (side) {
            case 0:
                y--;
                break;
            case 1:
                x++;
                break;
            case 2:
                y++;
                break;
            case 3:
                x--;
                break;
            default:
                break;
        }
        game.grid[x][y].visited = 1;
        val++;
        for (int i = 0; i < 4; i++) {
            if (game.grid[x][y].cotes[i] == 'b') {
                val++;
                break;
            }
        }
    }

    CoordStruct coordsList[4] = {{x, y - 1}, {x + 1, y}, {x, y + 1}, {x - 1, y}};

    for (int i = 0; i < 4; i++) {
        if ((game.grid[x][y].cotes[i] == 'v' || game.grid[x][y].cotes[i] == 'b') && game.grid[coordsList[i].x][coordsList[i].y].tileType == 0) {
            return 0;
        } else if ((game.grid[x][y].cotes[i] == 'v' || game.grid[x][y].cotes[i] == 'b') && (game.grid[coordsList[i].x][coordsList[i].y].cotes[(i + 2) % 4] == 'v' || game.grid[coordsList[i].x][coordsList[i].y].cotes[(i + 2) % 4] == 'b') && (game.grid[coordsList[i].x][coordsList[i].y].visited == 0) && (game.grid[coordsList[i].x][coordsList[i].y].centre != game.grid[coordsList[i].x][coordsList[i].y].cotes[(i + 2) % 4])) {
            val++;
            game.grid[coordsList[i].x][coordsList[i].y].visited = 1;
        } else if ((game.grid[x][y].cotes[i] == 'v' || game.grid[x][y].cotes[i] == 'b') && (game.grid[coordsList[i].x][coordsList[i].y].cotes[(i + 2) % 4] == 'v' || game.grid[coordsList[i].x][coordsList[i].y].cotes[(i + 2) % 4] == 'b') && (game.grid[coordsList[i].x][coordsList[i].y].visited == 0)) {
            val += PathScoreTown(game, coordsList[i], 4);
        }
    }

    return val;
}

GameStruct AddScoreTown(GameStruct game, PawnStruct pawn, int score) {
    int nbPawnsPerPlayer[5];

    for (int i = 0; i < 5; i++) {
        nbPawnsPerPlayer[i] = HowManyPawnsInTheTown(&game, (CoordStruct){pawn.coords.x, pawn.coords.y}, pawn.side, i);
    }

    int max = 0;
    for (int i = 0; i < 5; i++) {
        game.pawnssss[i] = nbPawnsPerPlayer[i];
        if (nbPawnsPerPlayer[i] > max) max = nbPawnsPerPlayer[i];
    }

    for (int i = 0; i < 5; i++) {
        if (nbPawnsPerPlayer[i] == max) game.playerList[i].score += score;
    }

    return game;
}

int HowManyPawnsInTheTown(GameStruct* game, CoordStruct coords, int side, int playerId) {
    int x = coords.x;
    int y = coords.y;

    int nbPawns = 0;
    game->grid[x][y].visited = 1;

    for (int p = 0; p < 25; p++) {
        if (game->pawns[p].coords.x == x && game->pawns[p].coords.y == y && game->pawns[p].side == side && game->pawns[p].idPlayers - 1 == playerId) {
            nbPawns++;
            *game = RemovePawn(*game, game->pawns[p]);
            game->playerList[playerId].nbPions++;
        }
    }

    if (game->grid[x][y].centre != 'v' && game->grid[x][y].centre != 'b') {
        switch (side) {
            case 0:
                y--;
                break;
            case 1:
                x++;
                break;
            case 2:
                y++;
                break;
            case 3:
                x--;
                break;
            default:
                break;
        }
        game->grid[x][y].visited = 1;
    }

    CoordStruct coordsList[4] = {{x, y - 1}, {x + 1, y}, {x, y + 1}, {x - 1, y}};

    for (int i = 0; i < 4; i++) {
        for (int p = 0; p < 25; p++) {
            if (game->pawns[p].coords.x == x && game->pawns[p].coords.y == y && game->pawns[p].side == i && game->pawns[p].idPlayers - 1 == playerId) {
                nbPawns++;
                *game = RemovePawn(*game, game->pawns[p]);
                game->playerList[playerId].nbPions++;
            }
        }
        if ((game->grid[x][y].cotes[i] == 'v' || game->grid[x][y].cotes[i] == 'b') && game->grid[coordsList[i].x][coordsList[i].y].tileType == 0) {
            return 0;
        } else if ((game->grid[x][y].cotes[i] == 'v' || game->grid[x][y].cotes[i] == 'b') && (game->grid[coordsList[i].x][coordsList[i].y].cotes[(i + 2) % 4] == 'v' || game->grid[coordsList[i].x][coordsList[i].y].cotes[(i + 2) % 4] == 'b') && (game->grid[coordsList[i].x][coordsList[i].y].visited == 0) && (game->grid[coordsList[i].x][coordsList[i].y].centre != game->grid[coordsList[i].x][coordsList[i].y].cotes[(i + 2) % 4])) {
            for (int p = 0; p < 25; p++) {
                if (game->pawns[p].coords.x == x && game->pawns[p].coords.y == y && game->pawns[p].side == i && game->pawns[p].idPlayers - 1 == playerId) {
                    nbPawns++;
                    *game = RemovePawn(*game, game->pawns[p]);
                    game->playerList[playerId].nbPions++;
                }
            }
            game->grid[coordsList[i].x][coordsList[i].y].visited = 1;
        } else if ((game->grid[x][y].cotes[i] == 'v' || game->grid[x][y].cotes[i] == 'b') && (game->grid[coordsList[i].x][coordsList[i].y].cotes[(i + 2) % 4] == 'v' || game->grid[coordsList[i].x][coordsList[i].y].cotes[(i + 2) % 4] == 'b') && (game->grid[coordsList[i].x][coordsList[i].y].visited == 0)) {
            nbPawns += HowManyPawnsInTheTown(game, coordsList[i], 4, playerId);
        }
    }

    return nbPawns;
}

GameStruct EndGame(GameStruct game) {
    while (game.pawns[0].coords.x != -1) {
        char side;
        if (game.pawns[0].side == 4) {
            side = game.grid[game.pawns[0].coords.x][game.pawns[0].coords.y].centre;
        } else {
            side = game.grid[game.pawns[0].coords.x][game.pawns[0].coords.y].cotes[game.pawns[0].side];
        }

        if (side == 'a') {
            game = AddScoreAbbeyIncomplete(game, game.pawns[0]);
        } else if (side == 'r') {
            game = AddScoreRoadIncomplete(game, game.pawns[0]);
        } else
            break;
    }

    return game;
}

GameStruct AddScoreAbbeyIncomplete(GameStruct game, PawnStruct pawn) {
    int x = pawn.coords.x;
    int y = pawn.coords.y;

    int score = 1;

    if (game.grid[x - 1][y - 1].tileType == 1) {
        score++;
    }
    if (game.grid[x][y - 1].tileType == 1) {
        score++;
    }
    if (game.grid[x + 1][y - 1].tileType == 1) {
        score++;
    }
    if (game.grid[x - 1][y].tileType == 1) {
        score++;
    }
    if (game.grid[x + 1][y].tileType == 1) {
        score++;
    }
    if (game.grid[x - 1][y + 1].tileType == 1) {
        score++;
    }
    if (game.grid[x][y + 1].tileType == 1) {
        score++;
    }
    if (game.grid[x + 1][y + 1].tileType == 1) {
        score++;
    }

    game.playerList[pawn.idPlayers - 1].score += score;
    game = RemovePawn(game, pawn);

    return game;
}

GameStruct AddScoreRoadIncomplete(GameStruct game, PawnStruct pawn) {
    int x = pawn.coords.x;
    int y = pawn.coords.y;
    int size = 0;

    PawnStruct pawnsList[25];
    int pawnIndex = 1;
    pawnsList[0] = pawn;

    TileStruct tuile;

    int previousDirection = pawn.side;

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
            if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 4) {
                pawnsList[pawnIndex] = game.pawns[p];
                pawnIndex++;
            }
            if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == (previousDirection + 2) % 4) {
                pawnsList[pawnIndex] = game.pawns[p];
                pawnIndex++;
            }
        }

        if (tuile.cotes[0] == 'r' && previousDirection != 2) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 0) {
                    pawnsList[pawnIndex] = game.pawns[p];
                    pawnIndex++;
                }
            }
            previousDirection = 0;
            y = y - 1;
            size++;
        } else if (tuile.cotes[1] == 'r' && previousDirection != 3) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 1) {
                    pawnsList[pawnIndex] = game.pawns[p];
                    pawnIndex++;
                }
            }
            previousDirection = 1;
            x = x + 1;
            size++;
        } else if (tuile.cotes[2] == 'r' && previousDirection != 0) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 2) {
                    pawnsList[pawnIndex] = game.pawns[p];
                    pawnIndex++;
                }
            }
            previousDirection = 2;
            y = y + 1;
            size++;
        } else if (tuile.cotes[3] == 'r' && previousDirection != 1) {
            for (int p = 0; p < 25; p++) {
                if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == 3) {
                    pawnsList[pawnIndex] = game.pawns[p];
                    pawnIndex++;
                }
            }
            previousDirection = 3;
            x = x - 1;
            size++;
        }

        tuile = game.grid[x][y];
    }

    // PROBLEME EXTREMITES
    for (int p = 0; p < 25; p++) {
        if (game.pawns[p].coords.x == x && game.pawns[p].coords.y == y && game.pawns[p].side == previousDirection) {
            pawnsList[pawnIndex] = game.pawns[p];
            pawnIndex++;
        }
    }

    int playersPawns[5];
    for (int i = 0; i < pawnIndex; i++) {
        playersPawns[pawnsList[i].idPlayers - 1] += 1;
    }

    int max = 0;
    for (int i = 0; i < 5; i++) {
        if (playersPawns[i] > max) max = playersPawns[i];
    }

    for (int i = 0; i < 5; i++) {
        if (playersPawns[i] == max) game.playerList[i].score += size;
    }

    for (int i = 0; i < pawnIndex; i++) {
        game.playerList[pawnsList[i].idPlayers - 1].nbPions++;
        game = RemovePawn(game, pawnsList[i]);
    }

    return game;
}

// A FAIRE
GameStruct AddScoreTownIncomplete(GameStruct game, PawnStruct pawn) {
    return game;
}

void selectionSort(int t[], int n, GameStruct game) {
    int temp;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (game.playerList[t[i] - 1].score < game.playerList[t[j] - 1].score) {
                temp = t[i];
                t[i] = t[j];
                t[j] = temp;
            }
        }
    }
}

void PrintEndScreen(GameStruct game) {
    int t[5] = {1, 2, 3, 4, 5};
    selectionSort(t, game.nbPlayers, game);
    int ch;
    do {
        erase();
        printw("   _____ _                _____ _____ ______ __  __ ______ _   _ _______ \n");
        printw("  / ____| |        /\\    / ____/ ____|  ____|  \\/  |  ____| \\ | |__   __| \n");
        printw(" | |    | |       /  \\  | (___| (___ | |__  | \\  / | |__  |  \\| |  | |    \n");
        printw(" | |    | |      / /\\ \\  \\___ \\\\___ \\|  __| | |\\/| |  __| | . ` |  | |    \n");
        printw(" | |____| |____ / ____ \\ ____) |___) | |____| |  | | |____| |\\  |  | |    \n");
        printw("  \\_____|______/_/    \\_\\_____/_____/|______|_|  |_|______|_| \\_|  |_|    \n");
        printw("\n\n\n");

        for (int i = 0; i < game.nbPlayers; i++) {
            attron(COLOR_PAIR(game.playerList[t[i] - 1].color));
            printw("Joueur %d ", t[i]);
            attroff(COLOR_PAIR(game.playerList[t[i] - 1].color));
            printw(": %d points\n", game.playerList[t[i] - 1].score);
        }

        printw("\n\n");
        printw("Appuyez sur espace pour terminer le programme.");

        ch = getch();
        if (ch == ' ') break;
    } while (1);
}