#include "ChessProg.h"

int main(int argc, char *argv[]) {
	if (argc == 1){
		runningMode = CONSOLE;
		handleConsole();
	}
	else if (argc == 2 && strncmp(argv[1], "console", 7) == 0){
		runningMode = CONSOLE;
		handleConsole();
	}
	else if (argc == 2 && strncmp(argv[1], "gui", 3) == 0){
		runningMode = GUI;
		SDLMain();
	}
	return 0;
}
