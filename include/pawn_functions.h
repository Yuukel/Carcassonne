#ifndef PAWN
#define PAWN

#include "game_structures.h"

GameStruct ChoosePawnPosition(GameStruct game, int x, int y, int ch);
GameStruct PlacePawn(GameStruct game, CoordStruct coords, int side);
GameStruct AddPawn(GameStruct game);
GameStruct RemovePawn(GameStruct game, PawnStruct pawn);
// GameStruct AddBuild(GameStruct game);
int CheckIfPlaceable(GameStruct game);

#endif  // PAWN