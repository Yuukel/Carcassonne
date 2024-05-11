#ifndef DEBUG
#define DEBUG

void PrintTileDbg(TileStruct t);
void SimplePrintTileDbg(TileStruct t);
void PrintGridDbg(GameStruct game, int coordXMin, int coordYMin);
void PrintCurrentTileDbg(GameStruct game);
GameStruct CanBePlaced(GameStruct game);

#endif  // DEBUG