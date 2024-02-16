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
    int color;
	int score;
	int nbPions;
    int isHuman;
} PlayerStruct;

typedef struct{
    int nbPlayers;
    PlayerStruct playerList[5];
    TileStruct pile[72];
    TileStruct grid[143][143];
} GameStruct;

typedef struct{
    PlayerStruct currentPlayer;
    TileStruct currentTile;
    enum State currentState;
} TurnStruct;

#endif // GAME_STRUCTURES
