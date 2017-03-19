#include "SdlMain.h"

int SDLMain(){
	window *s;
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);
	s = CreateWindow(SCREEN_W, SCREEN_H);
	CreateMainWindow(s);
	return 0;
}
