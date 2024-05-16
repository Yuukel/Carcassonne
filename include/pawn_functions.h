#ifndef PAWN
#define PAWN

#include "game_structures.h"

GameStruct ChoosePawnPosition(GameStruct game, int x, int y, int ch);
GameStruct PlacePawn(GameStruct game, CoordStruct coords, int side);
GameStruct AddPawn(GameStruct game);
GameStruct RemovePawn(GameStruct game, PawnStruct pawn);
int CheckIfPlaceable(GameStruct game);
int CheckIfPlaceableOnRoadLoop(GameStruct game, CoordStruct coords);
int CheckIfPlaceableOnRoad(GameStruct game, CoordStruct coords);
int CheckIfPlaceableOnTown(GameStruct game, CoordStruct coords);

#endif  // PAWN