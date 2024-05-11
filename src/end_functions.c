#include "end_functions.h"

#include "draw_functions.h"

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