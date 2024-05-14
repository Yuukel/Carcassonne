#ifndef END
#define END

#include "game_structures.h"

GameStruct InitTurn(GameStruct game);
GameStruct EndTurn(GameStruct game);
GameStruct AddScore(GameStruct game, PawnStruct pawn, int score);
int RouteAbbey(GameStruct game, CoordStruct coords);
int RouteRoad(GameStruct game, CoordStruct coords);
int RouteRoadLoop(GameStruct game, CoordStruct coords);
void PrintEndScreen(GameStruct game);

#endif  // END