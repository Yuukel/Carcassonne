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
            if (RouteAbbey(game, game.pawns[index].coords)) {
                game = AddScore(game, game.pawns[index], 9);
                index++;
            } else {
                index++;
            }
        } else if (side == 'r') {
            int val = 0;
            val = RouteRoadLoop(game, game.pawns[index].coords);
            // if (val == 0) game.turn.roadComplete = RouteRoad(game, game.pawns[index].coords);
            if (val == 0) val = RouteRoad(game, game.pawns[index].coords);
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
    // if (game.grid[x][y].tileType == 0) return (CoordStruct){100 + x, 100 + y};
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

    // if (game.grid[x][y].tileType == 0) return (CoordStruct){x, y};
    if (game.grid[x][y].tileType == 0) return 0;

    if (extremites.x == x && extremites.y == y)
        size++;
    else
        size += 2;

    // return (CoordStruct){1000 + size + 71, 1000 + size + 71};
    return size;
}

int RouteRoadLoop(GameStruct game, CoordStruct coords) {
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

    // Route incomplète
    // if (game.grid[x][y].tileType != 1) return 0; // marche pas ?
    // if (game.grid[x][y].centre == 'r') return 1;

    if (game.grid[x][y].centre != 'r') return 0;
    size++;
    return size;
}

void selectionSort(int t[], int n, GameStruct game) {
    for (int i = 0; i < n - 1; i++) {
        int maxIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (game.playerList[j].score > game.playerList[maxIndex].score) {
                int temp = t[i];
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