#include "start_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "draw_functions.h"
#include "end_functions.h"
#include "title_screen.h"

GameStruct InitGame(GameStruct game) {
    // Initialisation des joueurs
    game.playerList[0] = (PlayerStruct){1, RED, 0, 5, 1};
    game.playerList[1] = (PlayerStruct){2, GREEN, 0, 5, 0};
    game.playerList[2] = (PlayerStruct){0, 0, 0, 0, -1};
    game.playerList[3] = (PlayerStruct){0, 0, 0, 0, -1};
    game.playerList[4] = (PlayerStruct){0, 0, 0, 0, -1};
    game = SelectPlayers(game);

    // Initialisation du plateau de jeu
    for (int i = 0; i < 143; i++) {
        for (int j = 0; j < 143; j++) {
            game.grid[i][j] = (TileStruct){{'0', '0', '0', '0'}, '0', 0};
        }
    }

    // Initialisation de la pioche
    ParseurCSV("tuiles_base_simplifiees.csv", game.pile);
    game = PlaceStartingTile(game);
    ShufflePile(game.pile);

    // Initialisation des pions (vide)
    InitPawns(game.pawns);

    game.turn.nbTileInPile = 71;

    return game;
}

void ParseurCSV(char* fileName, TileStruct* pile) {
    FILE* fichier = fopen(fileName, "r");
    int nb, index = 0;
    char temp[1024];
    while (fgets(temp, 1024, fichier)) {
        TileStruct tile;
        nb = 0;
        char* value = strtok(temp, ", ");
        while (value != NULL) {
            if (value[4] == 'a') {  // village
                if (nb == 4) tile.centre = 'f';
            } else {
                if (nb < 4)
                    tile.cotes[nb] = value[0];
                else
                    tile.centre = value[0];
            }
            nb++;
            value = strtok(NULL, ", ");
        }
        tile.tileType = 1;
        pile[index] = tile;
        index++;
    }

    fclose(fichier);
}

// fonction pour placer la tuile de départ sur la grille
GameStruct PlaceStartingTile(GameStruct game) {
    game.grid[71][71] = game.pile[0];  // tuile de départ en (0 ; 0)
    RemoveTile(game.pile);

    return game;
}

// fonction pour mélanger la pile
void ShufflePile(TileStruct* pile) {
    // Mélange de Fisher-Yates
    int j;
    TileStruct temp;
    srand(time(NULL));

    for (int z = 0; z < 10; z++) {
        for (int i = 70; i > 0; i--) {
            j = rand() % (i + 1);
            temp = pile[j];
            pile[j] = pile[i];
            pile[i] = temp;
        }
    }
}

void InitPawns(PawnStruct* pawns) {
    PawnStruct pawn = (PawnStruct){(CoordStruct){-1, -1}, -1, -1};  // pion vide
    for (int i = 0; i < 26; i++) {
        pawns[i] = pawn;
    }
}

// void InitBuildings(BuildingStruct* buildings) {
//     for (int i = 0; i < 25; i++) {
//         buildings[i].coords = (CoordStruct){-1, -1};
//         buildings[i].buildingType = '0';
//         buildings[i].size = 0;
//         buildings[i].side = 0;
//     }
// }