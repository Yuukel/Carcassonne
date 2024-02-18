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
#define CYAN 6

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
    init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);
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

// AFFICHAGE DE L'ECRAN TITRE
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

// SELECTION DES JOUEURS
void SelectPlayers(GameStruct game){
    int ch; // caractère lu au clavier
    int row = 0, column = 0;
    int nbPlayers = 2;

    int colors[5] = {1,2,0,0,0}; // liste des couleurs pour chaque joueur
    int roles[5] = {1,0,-1,-1,-1}; // liste des "roles" pour chaque joueur (humain / ia)

    // boucle while d'affichage et de gestion des touches lues au clavier
    do{
        // si on est sur le CONFIRMER et que la touche espace est appuyée : break
        if(ch == ' ' && row == 3) break;

        // permet de changer de ligne avec les flèches haut et bas du clavier
        if(ch == KEY_UP && row > 0) row--;
        if(ch == KEY_DOWN && row < 3) row++;

        // permet d'ajouter ou de retirer des joueurs avec les flèches gauches et droites du clavier lorsqu'on est sur la ligne 3
        if(ch == KEY_LEFT && row == 2 && nbPlayers > 2){
            colors[nbPlayers - 1] = 0;
            roles[nbPlayers - 1] = -1;
            nbPlayers--;
        }
        if(ch == KEY_RIGHT && row == 2 && nbPlayers < 5){
            nbPlayers++;
            colors[nbPlayers - 1]++;
            int i = -1;
            while(i < nbPlayers){
                i++;
                if(i != nbPlayers - 1){
                    if(colors[nbPlayers - 1] == colors[i]){
                        colors[nbPlayers - 1]++;
                        i = -1;
                        if(colors[nbPlayers - 1] > 6) colors[nbPlayers - 1] = 1;
                    }
                }
            }
        }

        // permet de changer de joueur (colonne) avec les flèches gauche et droite du clavier
        if(ch == KEY_LEFT && column > 0) column--;
        if(ch == KEY_RIGHT && column < nbPlayers - 1) column++;

        if(ch == ' '){
            if(row == 0){
                colors[column]++;
                int i = -1;
                while(i < nbPlayers){
                    i++;
                    if(i != column){
                        if(colors[column] == colors[i]){
                            colors[column]++;
                            i = -1;
                        }
                    }
                    if(colors[column] > 6){
                        colors[column] = 1;
                        i = -1;
                    }
                }
            } else if(row == 1){
                roles[column]--;
                if(roles[column] < 0) roles[column] = 1;
            }
        }

        erase();
        for(int i = 0 ; i < nbPlayers ; i++){
            attron(COLOR_PAIR(colors[i]));
            wmove(stdscr, 0, i*10*4);
            printw("  ( )");
            attroff(COLOR_PAIR(colors[i]));
        }

        for(int i = 0 ; i < nbPlayers ; i++){
            attron(COLOR_PAIR(colors[i]));
            wmove(stdscr, 1, i*10*4);
            printw("  ) (");
            attroff(COLOR_PAIR(colors[i]));
        }

        for(int i = 0 ; i < nbPlayers ; i++){
            attron(COLOR_PAIR(colors[i]));
            wmove(stdscr, 2, i*10*4);
            printw(" (   )");
            attroff(COLOR_PAIR(colors[i]));
        }

        for(int i = 0 ; i < nbPlayers ; i++){
            attron(COLOR_PAIR(colors[i]));
            wmove(stdscr, 3, i*10*4);
            printw("[_____]");
            attroff(COLOR_PAIR(colors[i]));
        }

        for(int i = 0 ; i < nbPlayers ; i++){
            wmove(stdscr, 4, i*10*4);
            printw("Joueur %d",i+1);
        }

        for(int i = 0 ; i < nbPlayers ; i++){
            wmove(stdscr, 5, i*10*4);
            if(row == 0 && column == i) printw("[ ");
            attron(COLOR_PAIR(colors[i]));
            if(colors[i] == RED) printw("ROUGE");
            else if(colors[i] == GREEN) printw("VERT");
            else if(colors[i] == YELLOW) printw("JAUNE");
            else if(colors[i] == BLUE) printw("BLEU");
            else if(colors[i] == PINK) printw("ROSE");
            else if(colors[i] == CYAN) printw("CYAN");
            attroff(COLOR_PAIR(colors[i]));
            if(row == 0 && column == i) printw(" ]");
        }

        for(int i = 0 ; i < nbPlayers ; i++){
            wmove(stdscr, 6, i*10*4);
            if(row == 1 && column == i) printw("[ ");
            if(roles[i] == 1) printw("Humain");
            else printw("IA");
            if(row == 1 && column == i) printw(" ]");
        }
        printw("\n\n");

        printw("Nombre de joueurs : ");
        if(row == 2) printw("< ");
        printw("%d",nbPlayers);
        if(row == 2) printw(" >");
        printw("\n\n");

        if(row == 3) printw("[ ");
        printw("CONFIRMER");
        if(row == 3) printw(" ]");

        printw("\n\n\n");
        printw("< > : Changer de joueur\t\t\t");
        printw("^ v : Changer de ligne\t\t\t");
        printw("ESPACE : Confirmer\n");

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