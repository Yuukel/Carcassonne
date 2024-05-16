#ifndef PRINT
#define PRINT

#include "game_structures.h"

#define LIGNEMIN 6
#define LIGNEMINTUILE 8
#define COLONNEMINTUILE 4

// fonctions pour gérer la couleur dans l'affichage
void SetColor(char side);
void RemoveColor();

// fonctions d'affichage
void PrintPlayers(GameStruct game);                                              // affiche les joueurs et leurs infos
void PrintTile(TileStruct t, int i, int j, PawnStruct pawns[26], int x, int y);  // permet d'afficher une tuile
void PrintGrid(GameStruct game, int coordXMin, int coordYMin);                   // affichage la grille avec les tuiles
char* CurrentMode(TurnStruct turn);                                              // permet de retourner le mode actuel
void PrintTurnInfos(TurnStruct turn);                                            // affiche les infos actuelle du tour
void PrintCommands(TurnStruct turn, GameStruct game);                            // affichage des commandes
GameStruct ResetCanBePlaced(GameStruct game);                                    // reset des emplacements de tuiles posables
GameStruct CanBePlaced(GameStruct game);

#endif  // PRINT