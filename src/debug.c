#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "game_structures.h"

#define ROUTE 7
#define PRE 8
#define VILLE 9
#define BLASON 10
#define ABBAYE 11

void PrintTileDbg(TileStruct t){
    // attron(COLOR_PAIR(ROUTE));
    // printw("route\n");
    // attroff(COLOR_PAIR(ROUTE));

    // OUEST
    attron(COLOR_PAIR(ROUTE));
    wmove(stdscr, 1, 0);
    printw(" %c ",t.cotes[3]);
    attroff(COLOR_PAIR(ROUTE));

    // NORD
    attron(COLOR_PAIR(ROUTE));
    wmove(stdscr, 0, 3);
    printw(" %c ",t.cotes[0]);
    attroff(COLOR_PAIR(ROUTE));

    // CENTRE
    attron(COLOR_PAIR(ROUTE));
    wmove(stdscr, 1, 3);
    printw(" %c ",t.centre);
    attroff(COLOR_PAIR(ROUTE));

    // SUD
    attron(COLOR_PAIR(PRE));
    wmove(stdscr, 2, 3);
    printw(" %c ",t.cotes[2]);
    attroff(COLOR_PAIR(PRE));

    // EST
    attron(COLOR_PAIR(PRE));
    wmove(stdscr, 1, 6);
    printw(" %c ",t.cotes[1]);
    attroff(COLOR_PAIR(PRE));
}

// void SimplePrintTileDbg(TileStruct t){
//     printf(" %s %s \n",Couleur(t.cotes[0]),RESET);
//     printf("%s %s",Couleur(t.cotes[3]),RESET);
//     printf("%s %s",Couleur(t.centre),RESET);
//     printf("%s %s\n",Couleur(t.cotes[1]),RESET);
//     printf(" %s %s \n",Couleur(t.cotes[2]),RESET);
// }

// void PrintGridDbg(TileStruct grid[DIMENSION_MAX][DIMENSION_MAX]){
//     printf("\t");
//     for(int i = 0 ; i < DIMENSION_MAX ; i++){
//         printf("%d\t", i);
//     }
//     printf("\n");
//     for(int y = 0 ; y < DIMENSION_MAX ; y++){
//         printf("%d\t",y);
//         for(int x = 0 ; x < DIMENSION_MAX ; x++){
//             SimplePrintTileDbg(pile[0]);
//         }
//         printf("\n");
//     }
// }