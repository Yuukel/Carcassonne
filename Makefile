SRC = ./src/
INC = ./include/
OBJ = ./obj/
BIN = ./bin/

CC = gcc
CFLAGS = -Wall -std=c11
EXEC = carcassonne

.PHONY: all run clean

all: $(EXEC)

run: $(EXEC)
	$(BIN)$<

$(EXEC): moteur_jeu.o lecteur_csv.o
	$(CC) $(OBJ)* -I $(INC) -o $(BIN)$@
	
moteur_jeu.o: $(SRC)moteur_jeu.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

lecteur_csv.o: $(SRC)lecteur_csv.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

clean:
	rm $(BIN)* $(OBJ)*
