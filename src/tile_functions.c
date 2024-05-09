#include <stdio.h>
#include <stdlib.h>
#include "tile_functions.h"
#include "print_functions.h"

GameStruct WaitingForAction(GameStruct game){
    int ch;
    ch = getch();

    if(ch == 'c'){
        game.turn.currentMode = Camera;
    }

    if(ch == 'r'){
        game.turn.currentMode = Rotation;
    }

    if(ch == 'p'){
        game.turn.currentMode = Pose;
    }

    if(game.turn.currentMode == Rotation){
        game.turn.currentTile = RotateTile(game.turn.currentTile, ch);
    }

    if(game.turn.currentMode == Camera){
        game.turn = MoveCamera(game.turn, ch);
    }

    if(game.turn.currentMode == Pose){
        game = PlaceTile(game, ch);
    }

    return game;
}

TurnStruct MoveCamera(TurnStruct turn, int ch){
    if(ch == KEY_DOWN && turn.coordYMin > -71) turn.coordYMin--;
    if(ch == KEY_UP && turn.coordYMin < 61) turn.coordYMin++;
    if(ch == KEY_RIGHT && turn.coordXMin > -71) turn.coordXMin--;
    if(ch == KEY_LEFT && turn.coordXMin < 61) turn.coordXMin++;

    return turn;
}

TileStruct RotateTile(TileStruct t, int ch){
    char temp;
    int sens = 0;

    if(ch == KEY_LEFT) sens = -1;
    if(ch == KEY_RIGHT) sens = 1;

    if(sens > 0){
        temp = t.cotes[3];
        t.cotes[3] = t.cotes[2];
        t.cotes[2] = t.cotes[1];
        t.cotes[1] = t.cotes[0];
        t.cotes[0] = temp;
    }
    else if(sens < 0){
        temp = t.cotes[0];
        t.cotes[0] = t.cotes[1];
        t.cotes[1] = t.cotes[2];
        t.cotes[2] = t.cotes[3];
        t.cotes[3] = temp;
    }

    return t;
}

void ReturnCoordsList(GameStruct game, CoordStruct * coords){
    for(int i = 0 ; i < 144 ; i++){
        coords[i] = (CoordStruct){-1, -1}; // coordonnées vides;
    }

    int index = 0;
    for(int x = 0 ; x < 143 ; x++){
        for(int y = 0 ; y < 143 ; y++){
            if(game.grid[x][y].tileType == 2){
                coords[index++] = (CoordStruct){x,y};
            }
        }
    }
}

GameStruct ChangeNumbers(GameStruct game){
    CoordStruct coords[144];
    ReturnCoordsList(game, coords);

    int index = game.turn.tileIndex;
    for (int i = 0; i < 144; i++)
    {
        if (coords[i].x == -1)
            break;

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

GameStruct PlaceTile(GameStruct game, int ch){
    CoordStruct coords[144];
    ReturnCoordsList(game, coords);
    int x = -1, y = -1, index = game.turn.tileIndex;

    if(ch == KEY_LEFT){
        index--;
        if(index < 0){
            index = 143;
        }

        while(coords[index].x == -1){
            index--;
        }
    }

    if(ch == KEY_RIGHT){
        index++;
        if(index > 143){
            index = 0;
        }

        // necessaire ?
        while(coords[index].x == -1){
            index++;
        }
    }

    if(ch == 10){
        x = coords[index].x;
        y = coords[index].y;
        if(x > -1 && y > -1){
            game = ResetCanBePlaced(game);
            game.grid[x][y] = game.turn.currentTile;
        }
    }

    game.turn.tileIndex = index;
    return game;
}