C_FLAGS=  -std=c99 -pedantic-errors -Wall -g -lm `sdl-config --cflags` `sdl-config --libs`
O_FILES= ChessLogic.o Minimax.o GuiControls.o Gui.o SdlMain.o GameFlow.o ChessProg.o

all: chessprog

clean:
	rm chessprog $(O_FILES)

chessprog: $(O_FILES)
	gcc -o $@ $^ $(C_FLAGS)

ChessLogic.o: ChessLogic.c 
	gcc -c $(C_FLAGS) ChessLogic.c

Minimax.o: Minimax.c 
	gcc -c $(C_FLAGS) Minimax.c
	
GuiControls.o: GuiControls.c
	gcc -c $(C_FLAGS) GuiControls.c

Gui.o: Gui.c
	gcc -c $(C_FLAGS) Gui.c

SdlMain.o: SdlMain.c
	gcc -c $(C_FLAGS) SdlMain.c

GameFlow.o: GameFlow.c
	gcc -c $(C_FLAGS) GameFlow.c

ChessProg.o: ChessProg.c
	gcc -c $(C_FLAGS) ChessProg.c

