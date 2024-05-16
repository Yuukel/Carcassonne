#include "title_screen.h"

#include <ncurses.h>

/********************/
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define PINK 5
#define CYAN 6
/********************/

// AFFICHAGE DE L'ECRAN TITRE
void TitleScreen() {
    int ch;
    do {
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
        if (ch == ' ') break;
    } while (1);
}

// SELECTION DES JOUEURS
GameStruct SelectPlayers(GameStruct game) {
    int ch;  // caractère lu au clavier
    int row = 0, column = 0;
    int nbPlayers = 2;

    int colors[5] = {1, 2, 0, 0, 0};    // liste des couleurs pour chaque joueur
    int roles[5] = {1, 0, -1, -1, -1};  // liste des "roles" pour chaque joueur (humain / ia)

    // boucle while d'affichage et de gestion des touches lues au clavier
    do {
        // si on est sur le CONFIRMER et que la touche espace est appuyée :
        // break
        if (ch == ' ' && row == 3) break;

        // permet de changer de ligne avec les flèches haut et bas du clavier
        if (ch == KEY_UP && row > 0) row--;
        if (ch == KEY_DOWN && row < 3) row++;

        // permet d'ajouter ou de retirer des joueurs avec les flèches gauches
        // et droites du clavier lorsqu'on est sur la ligne 3
        if (ch == KEY_LEFT && row == 2 && nbPlayers > 2) {
            colors[nbPlayers - 1] = 0;
            roles[nbPlayers - 1] = -1;
            nbPlayers--;
        }
        if (ch == KEY_RIGHT && row == 2 && nbPlayers < 5) {
            nbPlayers++;
            colors[nbPlayers - 1]++;
            int i = -1;
            while (i < nbPlayers) {
                i++;
                if (i != nbPlayers - 1) {
                    if (colors[nbPlayers - 1] == colors[i]) {
                        colors[nbPlayers - 1]++;
                        i = -1;
                        if (colors[nbPlayers - 1] > 6)
                            colors[nbPlayers - 1] = 1;
                    }
                }
            }
        }

        // permet de changer de joueur (colonne) avec les flèches gauche et
        // droite du clavier
        if (ch == KEY_LEFT && column > 0) column--;
        if (ch == KEY_RIGHT && column < nbPlayers - 1) column++;

        if (ch == ' ') {
            if (row == 0) {
                colors[column]++;
                int i = -1;
                while (i < nbPlayers) {
                    i++;
                    if (i != column) {
                        if (colors[column] == colors[i]) {
                            colors[column]++;
                            i = -1;
                        }
                    }
                    if (colors[column] > 6) {
                        colors[column] = 1;
                        i = -1;
                    }
                }
            } else if (row == 1) {
                roles[column]--;
                if (roles[column] < 0) roles[column] = 1;
            }
        }

        erase();
        for (int i = 0; i < nbPlayers; i++) {
            attron(COLOR_PAIR(colors[i]));
            wmove(stdscr, 0, i * 10 * 4);
            printw("  ( )");
            attroff(COLOR_PAIR(colors[i]));
        }

        for (int i = 0; i < nbPlayers; i++) {
            attron(COLOR_PAIR(colors[i]));
            wmove(stdscr, 1, i * 10 * 4);
            printw("  ) (");
            attroff(COLOR_PAIR(colors[i]));
        }

        for (int i = 0; i < nbPlayers; i++) {
            attron(COLOR_PAIR(colors[i]));
            wmove(stdscr, 2, i * 10 * 4);
            printw(" (   )");
            attroff(COLOR_PAIR(colors[i]));
        }

        for (int i = 0; i < nbPlayers; i++) {
            attron(COLOR_PAIR(colors[i]));
            wmove(stdscr, 3, i * 10 * 4);
            printw("[_____]");
            attroff(COLOR_PAIR(colors[i]));
        }

        for (int i = 0; i < nbPlayers; i++) {
            wmove(stdscr, 4, i * 10 * 4);
            printw("Joueur %d", i + 1);
        }

        for (int i = 0; i < nbPlayers; i++) {
            wmove(stdscr, 5, i * 10 * 4);
            if (row == 0 && column == i) printw("[ ");
            attron(COLOR_PAIR(colors[i]));
            if (colors[i] == RED)
                printw("ROUGE");
            else if (colors[i] == GREEN)
                printw("VERT");
            else if (colors[i] == YELLOW)
                printw("JAUNE");
            else if (colors[i] == BLUE)
                printw("BLEU");
            else if (colors[i] == PINK)
                printw("ROSE");
            else if (colors[i] == CYAN)
                printw("CYAN");
            attroff(COLOR_PAIR(colors[i]));
            if (row == 0 && column == i) printw(" ]");
        }

        for (int i = 0; i < nbPlayers; i++) {
            wmove(stdscr, 6, i * 10 * 4);
            if (row == 1 && column == i) printw("[ ");
            if (roles[i] == 1)
                printw("Humain");
            else
                printw("IA");
            if (row == 1 && column == i) printw(" ]");
        }
        printw("\n\n");

        printw("Nombre de joueurs : ");
        if (row == 2) printw("< ");
        printw("%d", nbPlayers);
        if (row == 2) printw(" >");
        printw("\n\n");

        if (row == 3) printw("[ ");
        printw("CONFIRMER");
        if (row == 3) printw(" ]");

        printw("\n\n\n");
        printw("< > : Changer de joueur\t\t\t");
        printw("^ v : Changer de ligne\t\t\t");
        printw("ESPACE : Confirmer\n");

        refresh();
        ch = getch();
    } while (1);

    game.nbPlayers = nbPlayers;
    for (int i = 0; i < nbPlayers; i++) {
        game.playerList[i].id = i + 1;
        game.playerList[i].color = colors[i];
        game.playerList[i].isHuman = roles[i];
        game.playerList[i].score = 0;
        game.playerList[i].nbPions = 5;
    }

    return game;
}