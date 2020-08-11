CC=g++
CFLAGS = -g -W -Wall 
LDFLAGS =jeu
OBJ = jeu.cpp

compile: $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -lncurses -o jeu




clean:
	rm -f jeu


