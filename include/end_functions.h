#ifndef END
#define END

#include "game_structures.h"

GameStruct InitTurn(GameStruct game);
GameStruct EndTurn(GameStruct game);
GameStruct AddScoreAbbey(GameStruct game, PawnStruct pawn, int score);
int RouteAbbey(GameStruct game, CoordStruct coords);
GameStruct AddScoreRoad(GameStruct game, PawnStruct pawn, int score);
int RouteRoad(GameStruct game, CoordStruct coords);
GameStruct AddScoreRoadLoop(GameStruct game, PawnStruct pawn, int score);
int RouteRoadLoop(GameStruct game, CoordStruct coords);
int RouteTown(GameStruct game, CoordStruct coords);
int RouteScore(GameStruct game, CoordStruct coords);
void PrintEndScreen(GameStruct game);

#endif  // END