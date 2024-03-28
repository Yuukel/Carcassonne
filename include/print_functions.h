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
void PrintPlayers(GameStruct game); // affiche les joueurs et leurs infos
void PrintTile(TileStruct t, int i, int j); // permet d'afficher une tuile
void PrintGrid(GameStruct game, int coordXMin, int coordYMin); // affichage la grille avec les tuiles
char * CurrentMode(GameStruct game); // permet de retourner le mode actuel
void PrintTurnInfos(GameStruct game); // affiche les infos actuelle du tour
// affichage des commandes

#endif // PRINT