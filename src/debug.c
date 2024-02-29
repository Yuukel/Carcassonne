#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "game_structures.h"

#define ROUTE 7
#define PRE 8
#define VILLE 9
#define BLASON 10
#define ABBAYE 11

#define LIGNEMIN 6
#define LIGNEMINTUILE 8
#define COLONNEMINTUILE 4

void SetColor(char side){
    if(side == 'r'){
        attron(COLOR_PAIR(ROUTE));
    } else if(side == 'a'){
        attron(COLOR_PAIR(ABBAYE));
    } else if(side == 'b'){
        attron(COLOR_PAIR(BLASON));
    } else if(side == 'v'){
        attron(COLOR_PAIR(VILLE));
    } else{
        attron(COLOR_PAIR(PRE));
    }
}

void RemoveColor(){
    attroff(COLOR_PAIR(ROUTE));
    attroff(COLOR_PAIR(ABBAYE));
    attroff(COLOR_PAIR(BLASON));
    attroff(COLOR_PAIR(VILLE));
    attroff(COLOR_PAIR(PRE));
}

void PrintTileDbg(TileStruct t, int i, int j){
    if(t.centre != '0'){
        // OUEST
        SetColor(t.cotes[3]);
        wmove(stdscr, LIGNEMINTUILE+1+(j*3), COLONNEMINTUILE+(i*9));
        printw(" %c ",t.cotes[3]);
        RemoveColor();

        // NORD
        SetColor(t.cotes[0]);
        wmove(stdscr, LIGNEMINTUILE+(j*3), COLONNEMINTUILE+3+(i*9));
        printw(" %c ",t.cotes[0]);
        RemoveColor();

        // CENTRE
        SetColor(t.centre);
        wmove(stdscr, LIGNEMINTUILE+1+(j*3), COLONNEMINTUILE+3+(i*9));
        printw(" %c ",t.centre);
        RemoveColor();

        // SUD
        SetColor(t.cotes[2]);
        wmove(stdscr, LIGNEMINTUILE+2+(j*3), COLONNEMINTUILE+3+(i*9));
        printw(" %c ",t.cotes[2]);
        RemoveColor();

        // EST
        SetColor(t.cotes[1]);
        wmove(stdscr, LIGNEMINTUILE+1+(j*3), COLONNEMINTUILE+6+(i*9));
        printw(" %c ",t.cotes[1]);
        RemoveColor();
    }
}

void PrintGridDbg(GameStruct game, int coordXMin, int coordYMin){
    int longueur = 11;
    int hauteur = 11;

    for(int i = 0 ; i < longueur ; i++){
        wmove(stdscr, LIGNEMIN, COLONNEMINTUILE+4+(i*9));
        printw("%d",coordXMin+i);
    }
    for(int j = 0 ; j < hauteur ; j++){
        wmove(stdscr, LIGNEMINTUILE+1+(j*3), 0);
        printw("%d",coordYMin+j);
    }

    for(int i = 0 ; i < longueur; i++){
        for(int j = 0; j < hauteur; j++){
            PrintTileDbg(game.grid[coordXMin+i+71][coordYMin+j+71], i, j);
        }
    }
}