#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "game_structures.h"
#include "lecteur_csv.h"

// A faire : dispatcher dans plusieurs fichiers etc
// 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

//*************************************

#define DIMENSION_MAX 10 // En attendant ?

#define RESET -1

#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define PINK 5

// A REFAIRE POUR NCURSES
#define ROUTE "\e[1;30m\e[103m"
#define PRE "\e[1;30m\e[42m"
#define VILLE "\e[1;30m\e[43m"
#define BLASON "\e[1;30m\e[46m"
#define ABBAYE "\e[1;30m\e[41m"

//*************************************

// Fonctions pour ncurses
void InitNcurses();

// Fonctions de debug
void PrintTileDbg(TileStruct t);
void SimplePrintTileDbg(TileStruct t);
void PrintGridDbg(TileStruct grid[DIMENSION_MAX][DIMENSION_MAX]);

// Fonctions du menu
void TitleScreen();
void SelectPlayers(GameStruct game);

// Fonctions d'affichage
void PrintPlayers(int nb);

// Fonctions de tuiles
TileStruct RotateTile(TileStruct t, int sens);
void Tour();

GameStruct game;
// TileStruct pile[72];
TileStruct grid[DIMENSION_MAX][DIMENSION_MAX];

int main(int argc, char * argv[])
{
    // parseur_csv("tuiles_base_simplifiees.csv", game.pile);

    InitNcurses();

    TitleScreen();

    game.playerList[0] = (PlayerStruct){1, RED, 0, 5, 1};
    game.playerList[1] = (PlayerStruct){2, GREEN, 0, 5, 0};
    game.playerList[2] = (PlayerStruct){0, 0, 0, 0, -1};
    game.playerList[3] = (PlayerStruct){0, 0, 0, 0, -1};
    game.playerList[4] = (PlayerStruct){0, 0, 0, 0, -1};
    SelectPlayers(game);

    endwin();
    return 0;
}

void InitNcurses(){
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    start_color();
    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(PINK, COLOR_MAGENTA, COLOR_BLACK);
}

// char * Couleur(char c){
//     if(c == 'r') return ROUTE;
//     if(c == 'v') return VILLE;
//     if(c == 'a') return ABBAYE;
//     if(c == 'b') return BLASON;
//     if(c == 'p') return PRE;

//     return "";
// }

// void PrintTileDbg(TileStruct t){
//     printf("   %s %c %s   \n",Couleur(t.cotes[0]),t.cotes[0],RESET);
//     printf("%s %c %s",Couleur(t.cotes[3]),t.cotes[3],RESET);
//     printf("%s %c %s",Couleur(t.centre),t.centre,RESET);
//     printf("%s %c %s\n",Couleur(t.cotes[1]),t.cotes[1],RESET);
//     printf("   %s %c %s   \n",Couleur(t.cotes[2]),t.cotes[2],RESET);
// }

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

void TitleScreen(){
    int ch;
    do{
        erase();
        printw("   _____              _____     _____               _____    _____    ____    _   _   _   _   ______ \n");
        printw("  / ____|     /\\     |  __ \\   / ____|     /\\      / ____|  / ____|  / __ \\  | \\ | | | \\ | | |  ____|\n");
        printw(" | |         /  \\    | |__) | | |         /  \\    | (___   | (___   | |  | | |  \\| | |  \\| | | |__   \n");
        printw(" | |        / /\\ \\   |  _  /  | |        / /\\ \\    \\___ \\   \\___ \\  | |  | | | . ` | | . ` | |  __|  \n");
        printw(" | |____   / ____ \\  | | \\ \\  | |____   / ____ \\   ____) |  ____) | | |__| | | |\\  | | |\\  | | |____ \n");
        printw("  \\_____| /_/    \\_\\ |_|  \\_\\  \\_____| /_/    \\_\\ |_____/  |_____/   \\____/  |_| \\_| |_| \\_| |______|\n");
        printw("\n\n\n");
        printw("Appuyez sur ESPACE pour commencer");

        ch = getch();
        if(ch == ' ') break;
    }while(1);
}

// A COMMENTER
void SelectPlayers(GameStruct game){
    int ch;
    int row = 0, column = 0;

    int colors[5] = {1,2,0,0,0};
    int roles[5] = {1,0,-1,-1,-1};
    do{
        if(ch == ' ' && row == 2) break;

        if(ch == KEY_UP && row > 0) row--;
        if(ch == KEY_DOWN && row < 2) row++;
        if(ch == KEY_LEFT && column > 0) column--;
        if(ch == KEY_RIGHT && column < 4) column++;

        if(ch == ' '){
            if(row == 0){
                colors[column]++;
                int i = -1;
                while(i < 5){
                    i++;
                    if(i != column){
                        if(colors[column] == colors[i]){
                            colors[column]++;
                            i = -1;
                        }
                    }
                }
                if(colors[column] > 5) colors[column] = 0;
            }
            else{
                roles[column]--;
                if(roles[column] < -1) roles[column] = 1;
            }
        }

        erase();
        for(int i = 0 ; i < 5 ; i++){
            attron(COLOR_PAIR(colors[i]));
            printw("  ( )");
            printw("\t\t\t");
            attroff(COLOR_PAIR(colors[i]));
        }
        printw("\n");
        for(int i = 0 ; i < 5 ; i++){
            attron(COLOR_PAIR(colors[i]));
            printw("  ) (");
            printw("\t\t\t");
            attroff(COLOR_PAIR(colors[i]));
        }
        printw("\n");
        for(int i = 0 ; i < 5 ; i++){
            attron(COLOR_PAIR(colors[i]));
            printw(" (   )");
            printw("\t\t\t");
            attroff(COLOR_PAIR(colors[i]));
        }
        printw("\n");
        for(int i = 0 ; i < 5 ; i++){
            attron(COLOR_PAIR(colors[i]));
            printw("[_____]");
            printw("\t\t\t");
            attroff(COLOR_PAIR(colors[i]));
        }
        printw("\n");
        for(int i = 0 ; i < 5 ; i++){
            printw("Joueur %d",i+1);
            printw("\t\t");
        }
        printw("\n");
        for(int i = 0 ; i < 5 ; i++){
            if(row == 0 && column == i) printw("[ ");
            attron(COLOR_PAIR(colors[i]));
            if(colors[i] == RED) printw("ROUGE");
            else if(colors[i] == GREEN) printw("VERT");
            else if(colors[i] == YELLOW) printw("JAUNE");
            else if(colors[i] == BLUE) printw("BLEU");
            else if(colors[i] == PINK) printw("ROSE");
            else printw("- - -");
            attroff(COLOR_PAIR(colors[i]));
            if(row == 0 && column == i) printw(" ]");

            printw("\t\t\t");
        }
        printw("\n");
        for(int i = 0 ; i < 5 ; i++){
            if(row == 1 && column == i) printw("[");
            if(roles[i] == 1) printw("Humain");
            else if(roles[i] == 0) printw("IA");
            else printw("- - -");
            if(row == 1 && column == i) printw("]");
            printw("\t\t\t");
        }
        printw("\n\n");
        if(row == 2) printw("[ ");
        printw("CONFIRMER");
        if(row == 2) printw(" ]");

        printw("\n\n\n");
        printw("< > : Changer de joueur\t\t");
        printw("^ v : Changer de ligne\t\t");
        printw("ESPACE : Confirmer : %d", row);

        refresh();
        ch = getch();
    } while(1);
}

void PrintPlayers(int nb){
    for(int j = 0 ; j < nb ; j++){
        printw("+");
        for(int i = 0 ; i < 18 ; i++){
            printw("-");
        }
        printw("+");

        printw("\t\t\t");
    }
    printw("\n");
    for(int j = 0 ; j < nb ; j++){
        printw("| ");
        printw("Joueur : %d",j+1);
        printw(" = %d", 999);
        printw(" |");

        printw("\t\t\t");
    }
    printw("\n");
    for(int j = 0 ; j < nb ; j++){
        printw("+");
        for(int i = 0 ; i < 18 ; i++){
            printw("-");
        }
        printw("+");

        printw("\t\t\t");
    }
    printw("\n\n");
    refresh();
}

TileStruct RotateTile(TileStruct t, int sens){
    char temp; 
    if(sens > 0){
        temp = t.cotes[3];
        t.cotes[3] = t.cotes[2];
        t.cotes[2] = t.cotes[1];
        t.cotes[1] = t.cotes[0];
        t.cotes[0] = temp;
    }
    else if(sens < 0){
        temp = t.cotes[0];
        t.cotes[0] = t.cotes[1];
        t.cotes[1] = t.cotes[2];
        t.cotes[2] = t.cotes[3];
        t.cotes[3] = temp;
    }

    return t;
}

// void Tour(){
//     int choice;
//     PrintTileDbg(pile[1]);
//     while(scanf("%d",&choice) != 0){
//         pile[1] = RotateTile(pile[1], choice);
//         system("clear");
//         PrintTileDbg(pile[1]);
//     }
// }