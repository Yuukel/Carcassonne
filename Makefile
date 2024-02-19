SRC = ./src/
INC = ./include/
OBJ = ./obj/
BIN = ./bin/

CC = gcc
CFLAGS = -Wall -std=gnu11
EXEC = carcassonne

.PHONY: all run clean

all: $(EXEC)

run: $(EXEC)
	$(BIN)$<

$(EXEC): moteur_jeu.o debug.o title_screen.o lecteur_csv.o
	$(CC) $(OBJ)* -I $(INC) -o $(BIN)$@ -lncurses
	
moteur_jeu.o: $(SRC)moteur_jeu.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

debug.o: $(SRC)debug.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

title_screen.o: $(SRC)title_screen.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

lecteur_csv.o: $(SRC)lecteur_csv.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

clean:
	rm $(BIN)* $(OBJ)*
