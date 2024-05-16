#ifndef END
#define END

#include "game_structures.h"

GameStruct InitTurn(GameStruct game);
GameStruct EndTurn(GameStruct game);
GameStruct AddScoreAbbey(GameStruct game, PawnStruct pawn, int score);
int PathAbbey(GameStruct game, CoordStruct coords);
GameStruct AddScoreRoad(GameStruct game, PawnStruct pawn, int score);
int PathRoad(GameStruct game, CoordStruct coords);
GameStruct AddScoreRoadLoop(GameStruct game, PawnStruct pawn, int score);
int PathRoadLoop(GameStruct game, CoordStruct coords);
int PathTown(GameStruct game, CoordStruct coords, int side);
int PathScoreTown(GameStruct game, CoordStruct coords, int side);

GameStruct EndGame(GameStruct game);
GameStruct AddScoreAbbeyIncomplete(GameStruct game, PawnStruct pawn);
void PrintEndScreen(GameStruct game);

#endif  // END