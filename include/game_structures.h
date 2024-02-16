#ifndef GAME_STRUCTURES
#define GAME_STRUCTURES

enum State {Draw, Tile, Pawn, End}; // Etats du jeu
enum Type {Pre, Route, Ville, Blason, Abbaye, Fin}; //Check les traductions

typedef struct{
    char cotes[4];
    char centre;
} TileStruct;

typedef struct{
    int id;
} PlayerStruct;

typedef struct{
    PlayerStruct playerList[5];
    TileStruct pile[72];
    TileStruct grid[143][143];
} Game;

typedef struct{
    PlayerStruct currentPlayer;
    TileStruct currentTile;
    enum State currentState;
} Turn;

#endif // GAME_STRUCTURES
