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

enum State { Draw,
             Tile,
             Pawn,
             End };  // Etats du jeu

enum Mode { Camera,
            Rotation,
            Pose,
            Question,
            Pion };  // Modes dans le jeu

typedef struct {
    char cotes[4];
    char centre;
    int tileType;  // 0 = tuile vide // 1 = tuile pleine // 2 = emplacement pour poser
} TileStruct;

typedef struct {
    int x, y;
} CoordStruct;

typedef struct {
    CoordStruct coords;
    int idPlayers;
    int side;  // N 0 - E 1 - S 2 - O 4 - C 5
} PawnStruct;

// typedef struct {
//     CoordStruct coords;
//     char buildingType;
//     int size;
//     int side;
// } BuildingStruct;

typedef struct {
    int id;
    int color;
    int score;
    int nbPions;
    int isHuman;
} PlayerStruct;

typedef struct {
    PlayerStruct currentPlayer;
    TileStruct currentTile;
    enum State currentState;
    enum Mode currentMode;

    int coordYMin;
    int coordXMin;
    int tileIndex;

    int currentX;
    int currentY;
    int currentSide;

    int turnEnd;

    CoordStruct turnX;
} TurnStruct;

typedef struct {
    int nbPlayers;
    PlayerStruct playerList[5];
    TileStruct pile[72];
    TileStruct grid[143][143];
    TurnStruct turn;
    PawnStruct pawns[26];
    // BuildingStruct buildings[25];
} GameStruct;

#endif  // GAME_STRUCTURES
