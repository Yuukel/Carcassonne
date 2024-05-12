#ifndef END
#define END

#include "game_structures.h"

GameStruct InitTurn(GameStruct game);
GameStruct EndTurn(GameStruct game);
GameStruct AddScore(GameStruct game, PawnStruct pawn, int score);
// void RemoveBuilding(GameStruct game, BuildingStruct building);
int RouteAbbey(GameStruct game, CoordStruct coords);
int RouteRoad(GameStruct game, CoordStruct coords);
int CheckPawnAmount(GameStruct game, CoordStruct coords, CoordStruct coordsList[72]);

#endif  // END