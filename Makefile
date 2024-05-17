SRC = ./src/
INC = ./include/
OBJ = ./obj/
BIN = ./bin/
NCR = ./ncurses/include/

CC = gcc
CFLAGS = -Wall -std=gnu11
EXEC = carcassonne

.PHONY: all run clean

all: $(EXEC)

run: $(EXEC)
	$(BIN)$<

$(EXEC): moteur_jeu.o title_screen.o start_functions.o print_functions.o draw_functions.o tile_functions.o pawn_functions.o end_functions.o
	$(CC) $(OBJ)* -I $(INC) -o $(BIN)$@ -I $(NCR) -lncursesw
	
moteur_jeu.o: $(SRC)moteur_jeu.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

title_screen.o: $(SRC)title_screen.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

start_functions.o: $(SRC)start_functions.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

print_functions.o: $(SRC)print_functions.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

draw_functions.o: $(SRC)draw_functions.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

tile_functions.o: $(SRC)tile_functions.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

pawn_functions.o: $(SRC)pawn_functions.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

end_functions.o: $(SRC)end_functions.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

clean:
	rm $(BIN)* $(OBJ)*
