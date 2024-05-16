#ifndef START
#define START

#include "game_structures.h"

GameStruct InitGame(GameStruct game);
void ParseurCSV(char* fileName, TileStruct* pile);
GameStruct PlaceStartingTile(GameStruct game);
void ShufflePile(TileStruct* pile);
void InitPawns(PawnStruct* pawns);

#endif  // START