#ifndef GAME_STRUCTURES
#define GAME_STRUCTURES

// définition des macros pour les couleurs
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define PINK 5
#define CYAN 6

#define ROUTE 7
#define PRE 8
#define VILLE 9
#define BLASON 10
#define ABBAYE 11
#define FIN 12
#define PLACEMENT 13

enum State {Draw, Tile, Pawn, End}; // Etats du jeu
// enum Type {Pre, Route, Ville, Blason, Abbaye, Fin};
enum Mode {Camera, Rotation, Pose}; // Mode dans le jeu (State.Tile)

typedef struct{
    char cotes[4];
    char centre;
    int tileType; // 0 = tuile vide // 1 = tuile pleine // 2 = emplacement pour poser
} TileStruct;

typedef struct{
    int id;
    int color;
	int score;
	int nbPions;
    int isHuman;
} PlayerStruct;

typedef struct{
    PlayerStruct currentPlayer;
    TileStruct currentTile;
    enum State currentState;
    enum Mode currentMode;
} TurnStruct;

typedef struct{
    int nbPlayers;
    PlayerStruct playerList[5];
    TileStruct pile[72];
    TileStruct grid[143][143];
    TurnStruct turn;
} GameStruct;

#endif // GAME_STRUCTURES
