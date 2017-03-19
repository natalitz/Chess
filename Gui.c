#include "Gui.h"

int castle_00, castle_70, castle_07, castle_77; // 0- hasn't moved, 1- moved
static int quit = 0;
static int gameMode = 1; // 1 = 2 players, 2 = player & computer
static int playerColor = WHITE;
static int difficulty = 1; // -1 is best
static int turn = WHITE;

static char** storedBoard = NULL; // used to restore the game
static int storedGameMode = 1; // 1 is player vs. player, 2 is player vs. computer
static int storedPlayerColor = WHITE;
static int storedDifficulty = 1; // -1 is best
static int storedTurn = WHITE;
static char** lastBoard = NULL; // this board will be used to restore the last board that appeared on window
static int chooseBoard = 0; // 0 means blank board will appear, 1 means last board (s_board) will appear, 2 means beginning of previous game (storedBoard) will appear, 3 means last board (s_board) with new promotion will appear
static int settingsAddColor = WHITE; // 1 is white, -1 is black

int SetDiffAsOne(window *s){
	difficulty = 1;
	storedDifficulty = 1;
	return 0;
}

static int isThereWin = 0;
static int isThereProblemToLoad = 0;
static int numberOfSavedGame = 0;

int SetPlayerAsBlack(window *s){
	playerColor = BLACK;
	storedPlayerColor = BLACK;
	return 0;
}

static int pressNumber = 0;
static int firstPressX = -1;
static int firstPressY = -1;
static int secondPressX = -1;
static int secondPressY = -1;
static moveElement* move;

// main window constants
static int buttonStartX = 250;
static int buttonWidth = 380;
static int buttonHight = 107;
static int redBackgroundcolor = 200;
static int blueBackgroundcolor = 160;
static int greenBackgroundcolor = 120;

static const char* sdlBufferError = "ERROR: failed to flip buffer: %s\n";
static const char* wrongFileError = "ERROR: Wrong file name\n";

int CreateMainWindow(window *s){
	control *main_window = CreatePanel(0, 0, SCREEN_W, SCREEN_H, redBackgroundcolor, blueBackgroundcolor, greenBackgroundcolor);
	control *chess = CreateLabel(150, 0, 572, 161, UploadPic("chessGameLable.bmp"));
	control *new_game = CreateButton(buttonStartX, 161, buttonWidth, buttonHight, UploadPic("newGameFinal.bmp"), &CreateGameModeWindow, 'x');
	control *load_game = CreateButton(buttonStartX, 300,  buttonWidth, buttonHight, UploadPic("LoadGameFinal.bmp"), &LoadGame, 'x');
	control *quit_b = CreateButton(buttonStartX, 440, buttonWidth, buttonHight, UploadPic("QuitGameFinal.bmp"), &QuitGame, 'x');
	gameMode = 1;
	playerColor = WHITE;
	difficulty = 1;
	turn = WHITE;
	storedGameMode = 1;
	storedPlayerColor = WHITE;
	storedDifficulty = 1;
	storedTurn = WHITE;
	isThereProblemToLoad = 0;
	pressNumber = 0;
	firstPressX = -1;
	firstPressY = -1;
	secondPressX = -1;
	secondPressY = -1;
	chooseBoard = 0;
	castle00 = 0;
	castle70 = 0;
	castle07 = 0;
	castle77 = 0;
	castle_00 = 0;
	castle_70 = 0;
	castle_07 = 0;
	castle_77 = 0;
	FreeBoardsAndTree(s);
	AddPanelToWindow(s, main_window);
	AddLabelToPanel(main_window, chess, s);
	AddButtonToPanel(main_window, new_game, s);
	AddButtonToPanel(main_window, load_game, s);
	AddButtonToPanel(main_window, quit_b, s);
	if (SDL_Flip(s->self) != 0){
		printf(sdlBufferError, SDL_GetError());
		exit(0);
	}
	while (quit == 0){
		SDL_Event e;
		while (SDL_PollEvent(&e) == 1 && quit == 0) {
			switch (e.type){
				case (SDL_QUIT) :
					FreeUITree(s->panel_child);
					s->panel_child = NULL;
					SDL_FreeSurface(s->self);
					free(s);
					quit = 1;
					break;
				case (SDL_MOUSEBUTTONUP) :
					HandlePress(s, main_window, e);
					break;
				default:
					break;
			}
		}
	}
	return 0;
}


int CreateGameModeWindow(window *s){
	// constants
	int buttonWidth1 = 721;
	int buttonHieght1 = 107;
	int buttonWidth2 = 380;
	int x1 = 90;

	control *gameModeWindow = CreatePanel(0, 0, SCREEN_W, SCREEN_H, redBackgroundcolor, blueBackgroundcolor, greenBackgroundcolor);
	control *playerVsPlayer = CreateButton(x1, 100, buttonWidth1, buttonHieght1, UploadPic("playerVsPlayer.bmp"), &OnPlayerVsPlayerPress, 'x');
	control *playerVsComputer = CreateButton(x1, 260, buttonWidth1, buttonHieght1, UploadPic("playerVsComp.bmp"), &OnPlayerVsComputerPress, 'x');
	control *cancel = CreateButton(260, 420, buttonWidth2, buttonHieght1, UploadPic("cancelFinal.bmp"), &CreateMainWindow, 'x');
	isThereProblemToLoad = 0;
	FreeUITree(s->panel_child);
	s->panel_child = NULL;
	AddPanelToWindow(s, gameModeWindow);
	AddButtonToPanel(gameModeWindow, playerVsPlayer, s);
	AddButtonToPanel(gameModeWindow, playerVsComputer, s);
	AddButtonToPanel(gameModeWindow, cancel, s);
	if (SDL_Flip(s->self) != 0){
		printf(sdlBufferError, SDL_GetError());
		exit(0);
	}
	while (quit == 0){
		SDL_Event e;
		while (SDL_PollEvent(&e) == 1 && quit == 0) {
			switch (e.type){
				case (SDL_QUIT) :
					FreeBoardsAndTree(s);
					SDL_FreeSurface(s->self);
					free(s);
					quit = 1;
					break;
				case (SDL_MOUSEBUTTONUP) :
					HandlePress(s, gameModeWindow, e);
					break;
				default:
					break;
			}
		}
	}
	return 0;
}

int OnPlayerVsComputerPress(window *s){
	gameMode = 2;
	storedGameMode = 2;
	CreateBoardSettingsWindow(s);
	return 0;
}

int OnPlayerVsPlayerPress(window *s){
	gameMode = 1;
	storedGameMode = 1;
		playerColor = WHITE;
		storedPlayerColor = WHITE;
		difficulty = 1;
		storedDifficulty = 1;
	CreateBoardSettingsWindow(s);
	return 0;
}

int CreateNextPlayerWindow(window *s){
	control *nextPlayerLabel = NULL; control *white = NULL; control *black = NULL;
	control *startGame = NULL; control *cancel = NULL;

	control *nextPlayerPanel = CreatePanel(0, 0, SCREEN_W, SCREEN_H, redBackgroundcolor, blueBackgroundcolor, greenBackgroundcolor);
	isThereProblemToLoad = 0;
	FreeUITree(s->panel_child);
	s->panel_child = NULL;
	AddPanelToWindow(s, nextPlayerPanel);
	RebuildNextPlayerPanel(s, &nextPlayerPanel, &nextPlayerLabel, &white, &black, &cancel, &startGame);
	if (SDL_Flip(s->self) != 0){
		printf(sdlBufferError, SDL_GetError());
		exit(0);
	}
	while (quit == 0){
		SDL_Event e;
		while (SDL_PollEvent(&e) == 1 && quit == 0) {
			switch (e.type){
				case (SDL_QUIT) :
					FreeBoardsAndTree(s);
					SDL_FreeSurface(s->self);
					free(s);
					quit = 1;
					break;
				case (SDL_MOUSEBUTTONUP) :
					HandlePress(s, nextPlayerPanel, e);
					if (quit!=1){
						FreeUITree(s->panel_child);
						s->panel_child = NULL;
						nextPlayerPanel = CreatePanel(0, 0, SCREEN_W, SCREEN_H, redBackgroundcolor, blueBackgroundcolor, greenBackgroundcolor);
						AddPanelToWindow(s, nextPlayerPanel);
						RebuildNextPlayerPanel(s, &nextPlayerPanel, &nextPlayerLabel, &white, &black, &cancel, &startGame);
						if (SDL_Flip(s->self) != 0){
							printf(sdlBufferError, SDL_GetError());
							exit(0);
						}
					}
					break;
				default:
					break;
			}
		}
	}
	return 0;
}

void RebuildNextPlayerPanel(window *s, control **nextPlayerPanel, control **nextPlayerLabel, control **white,
		control **black, control **cancel, control **navigationButton){

	// constant
	int x1 = 520;
	int y1 = 150;
	int size1 = 160;

	*nextPlayerLabel = CreateLabel(50, y1+40, 475, 80, UploadPic("nextPlayer.bmp"));
	if (storedTurn == WHITE)
		*white = CreateButton(x1, y1, size1, size1, UploadPic("wb.bmp"), &SetNextTurnAsWhite, 'x');
	else
		*white = CreateButton(x1, y1, size1, size1, UploadPic("wn.bmp"), &SetNextTurnAsWhite, 'x');
	if (storedTurn == BLACK)
		*black = CreateButton(x1+160, y1, size1, size1, UploadPic("bb.bmp"), &SetNextTurnAsBlack, 'x');
	else
		*black = CreateButton(x1+160, y1, size1, size1, UploadPic("bn.bmp"), &SetNextTurnAsBlack, 'x');
	*cancel = CreateButton(150, 470, 380, 107, UploadPic("cancelS.bmp"), &CreateMainWindow, 'x');
	if(storedGameMode == 1){
		*navigationButton = CreateButton(500, 470, 380,107, UploadPic("startGameFromSettings.bmp"), &CreateGameWindow, 'x');
	}
	else{
		*navigationButton = CreateButton(500, 470, 380, 107, UploadPic("next.bmp"), &CreateAlSettingsWindow, 'x');
	}

	AddLabelToPanel(*nextPlayerPanel, *nextPlayerLabel, s);
	AddButtonToPanel(*nextPlayerPanel, *white, s);
	AddButtonToPanel(*nextPlayerPanel, *black, s);
	AddButtonToPanel(*nextPlayerPanel, *cancel, s);
	AddButtonToPanel(*nextPlayerPanel, *navigationButton, s);

}

int SetNextTurnAsWhite(window *s){
	storedTurn = WHITE;
	return 0;
}

int SetNextTurnAsBlack(window *s){
	storedTurn = BLACK;
	return 0;
}

int CreateAlSettingsWindow(window *s){
	control *chooseColor; control *white; control *black; control *chooseDifficulty;
	control *one; control *two; control *three; control *four; control *best; control *startGame;
	control *alSettingsWindow = CreatePanel(0, 0, SCREEN_W, SCREEN_H, redBackgroundcolor, blueBackgroundcolor, greenBackgroundcolor);
	FreeUITree(s->panel_child);
	s->panel_child = NULL;
	AddPanelToWindow(s, alSettingsWindow);
	RebuildSettingsMenu(s, &alSettingsWindow, &chooseColor, &white, &black, &chooseDifficulty, &one, &two, &three, &four, &best, &startGame);
	if (SDL_Flip(s->self) != 0){
		printf(sdlBufferError, SDL_GetError());
		exit(0);
	}
	while (quit == 0){
		SDL_Event e;
		while (SDL_PollEvent(&e) == 1 && quit == 0){
			switch (e.type){
				case (SDL_QUIT) :
					FreeBoardsAndTree(s);
					SDL_FreeSurface(s->self);
					free(s);
					quit = 1;
					break;
				case (SDL_MOUSEBUTTONUP) :
					HandlePress(s, alSettingsWindow, e);
					if (quit!=1){
						FreeUITree(s->panel_child);
						s->panel_child = NULL;
						alSettingsWindow = CreatePanel(0, 0, SCREEN_W, SCREEN_H, redBackgroundcolor, blueBackgroundcolor, greenBackgroundcolor);
						AddPanelToWindow(s, alSettingsWindow);
						RebuildSettingsMenu(s, &alSettingsWindow, &chooseColor, &white, &black, &chooseDifficulty, &one, &two, &three, &four, &best, &startGame);
						if (SDL_Flip(s->self) != 0){
							printf(sdlBufferError, SDL_GetError());
							exit(0);
						}
					}
					break;
				default:
					break;
			}
		}
	}
	return 0;
}


int RebuildSettingsMenu(window *s, control **second_settings_window, control **choose_color, control **white, control **black, control **choose_difficulty, control **one, control **two, control **three, control **four, control **best, control **startGame){
	// constants
	int size1 = 160;
	int size2 = 80;
	int x1 = 470;
	int y1 = 70;
	int x2 = 20;
	int x_1 = 400;
	int x_2 = 480;
	int x_3 = 560;
	int x_4 = 640;
	int x_best = 720;
	int yDifficulty = 330;

	*choose_color = CreateLabel(x2, 90, 400, 88, UploadPic("yourColor2.bmp"));
	if (storedPlayerColor == WHITE)
		*white = CreateButton(x1, y1, size1, size1, UploadPic("wb.bmp"), &SetPlayerAsWhite, 'x');
	else
		*white = CreateButton(x1, y1, size1, size1, UploadPic("wn.bmp"), &SetPlayerAsWhite, 'x');
	if (storedPlayerColor == BLACK)
		*black = CreateButton(630, y1, size1, size1, UploadPic("bb.bmp"), &SetPlayerAsBlack, 'x');
	else
		*black = CreateButton(630, y1, size1, size1, UploadPic("bn.bmp"), &SetPlayerAsBlack, 'x');
	*choose_difficulty = CreateLabel(x2, 320, 475, 81, UploadPic("difficulty.bmp"));
	if (storedDifficulty==1)
		*one = CreateButton(x_1, yDifficulty, size2, size2, UploadPic("1b.bmp"), &SetDiffAsOne, 'x');
	else
		*one = CreateButton(x_1, yDifficulty, size2, size2, UploadPic("1n.bmp"), &SetDiffAsOne, 'x');
	if (storedDifficulty==2)
		*two = CreateButton(x_2, yDifficulty, size2, size2, UploadPic("2b.bmp"), &SetDiffAsTwo, 'x');
	else
		*two = CreateButton(x_2, yDifficulty, size2, size2, UploadPic("2n.bmp"), &SetDiffAsTwo, 'x');
	if (storedDifficulty == 3)
		*three = CreateButton(x_3, yDifficulty, size2, size2, UploadPic("3b.bmp"), &SetDiffAsThree, 'x');
	else
		*three = CreateButton(x_3, yDifficulty, size2, size2, UploadPic("3n.bmp"), &SetDiffAsThree, 'x');
	if (storedDifficulty==4)
		*four = CreateButton(x_4, yDifficulty, size2, size2, UploadPic("4b.bmp"), &SetDiffAsFour, 'x');
	else
		*four = CreateButton(x_4, yDifficulty, size2, size2, UploadPic("4n.bmp"), &SetDiffAsFour, 'x');
	if (storedDifficulty==-1)
		*best = CreateButton(x_best, yDifficulty, size2, size2, UploadPic("bestB.bmp"), &SetDiffAsBest, 'x');
	else
		*best = CreateButton(x_best, yDifficulty, size2, size2, UploadPic("bestN.bmp"), &SetDiffAsBest, 'x');
	*startGame = CreateButton(350, 500, 380, 106, UploadPic("startGame.bmp"), &CreateGameWindow, 'x');
	AddLabelToPanel(*second_settings_window, *choose_color, s);
	AddButtonToPanel(*second_settings_window, *white, s);
	AddButtonToPanel(*second_settings_window, *black, s);
	AddLabelToPanel(*second_settings_window, *choose_difficulty, s);
	AddButtonToPanel(*second_settings_window, *one, s);
	AddButtonToPanel(*second_settings_window, *two, s);
	AddButtonToPanel(*second_settings_window, *three, s);
	AddButtonToPanel(*second_settings_window, *four, s);
	AddButtonToPanel(*second_settings_window, *best, s);
	AddButtonToPanel(*second_settings_window, *startGame, s);
	return 0;
}


int SetPlayerAsWhite(window *s){
	playerColor = WHITE;
	storedPlayerColor = WHITE;
	return 0;
}

int SetDiffAsTwo(window *s){
	difficulty = 2;
	storedDifficulty = 2;
	return 0;
}

int SetDiffAsThree(window *s){
	difficulty = 3;
	storedDifficulty = 3;
	return 0;
}

int SetDiffAsFour(window *s){
	difficulty = 4;
	storedDifficulty = 4;
	return 0;
}

int SetDiffAsBest(window *s){
	difficulty = -1;
	storedDifficulty = -1;
	return 0;
}


int LoadGame(window *s){
	//constants
	int size1 = 80;
	int y1 = 250;

	control *loadAndSaveP; control *one; control *two; control *three; control *four; control *five;
	control *six; control *seven; control *eight; control *nine; control *ten; control *noGameHasBeenSaved;
	control *cancel = CreateButton(250, 470, 380, 106, UploadPic("cancelFinal.bmp"), &CreateMainWindow, 'x');
	control *loadGame = CreateLabel(150, 10, 572, 161, UploadPic("loadGameLabel.bmp"));
	FreeUITree(s->panel_child);
	s->panel_child = NULL;
	loadAndSaveP = CreatePanel(0, 0, SCREEN_W, SCREEN_H, redBackgroundcolor, blueBackgroundcolor, greenBackgroundcolor);
	AddPanelToWindow(s, loadAndSaveP);
	AddLabelToPanel(loadAndSaveP, loadGame, s);
	if (1 <= SLOTS){
		one = CreateButton(100, y1, size1, size1, UploadPic("1n.bmp"), &Load1, 'x');
		AddButtonToPanel(loadAndSaveP, one, s);
	}
	if (2 <= SLOTS){
		two = CreateButton(200, y1, size1, size1, UploadPic("2n.bmp"), &Load2, 'x');
		AddButtonToPanel(loadAndSaveP, two, s);
	}
	if (3 <= SLOTS){
		three = CreateButton(300, y1, size1, size1, UploadPic("3n.bmp"), &Load3, 'x');
		AddButtonToPanel(loadAndSaveP, three, s);
	}
	if (4 <= SLOTS){
		four = CreateButton(400, y1, size1, size1, UploadPic("4n.bmp"), &Load4, 'x');
		AddButtonToPanel(loadAndSaveP, four, s);
	}
	if (5 <= SLOTS){
		five = CreateButton(500, y1, size1, size1, UploadPic("5n.bmp"), &Load5, 'x');
		AddButtonToPanel(loadAndSaveP, five, s);
	}
	if (6 <= SLOTS){
		six = CreateButton(600, y1, size1, size1, UploadPic("6n.bmp"), &Load6, 'x');
		AddButtonToPanel(loadAndSaveP, six, s);
	}
	if (7 <= SLOTS){
		seven = CreateButton(700, y1, size1, size1, UploadPic("7n.bmp"), &Load7, 'x');
		AddButtonToPanel(loadAndSaveP, seven, s);
	}
	if (8 <= SLOTS){
		eight = CreateButton(200, y1+100, size1, size1, UploadPic("8n.bmp"), &Load8, 'x');
		AddButtonToPanel(loadAndSaveP, eight, s);
	}
	if (9 <= SLOTS){
		nine = CreateButton(400, y1+100, size1, size1, UploadPic("9n.bmp"), &Load9, 'x');
		AddButtonToPanel(loadAndSaveP, nine, s);
	}
	if (10 <= SLOTS){
		ten = CreateButton(600, y1+100, size1, size1, UploadPic("10n.bmp"), &Load10, 'x');
		AddButtonToPanel(loadAndSaveP, ten, s);
	}
	AddButtonToPanel(loadAndSaveP, cancel, s);
	if (SDL_Flip(s->self) != 0){
		printf(sdlBufferError, SDL_GetError());
		exit(0);
	}
	while (quit == 0){
		if (isThereProblemToLoad == 1 && GetlastChild(loadAndSaveP)->Label==NULL){
			noGameHasBeenSaved= CreateLabel(50, 130, 799, 107, UploadPic("NoGameHasBeenSaved.bmp"));
			AddLabelToPanel(loadAndSaveP, noGameHasBeenSaved, s);
			if (SDL_Flip(s->self) != 0){
				printf(sdlBufferError, SDL_GetError());
				exit(0);
			}
		}
		SDL_Event e;
		while (SDL_PollEvent(&e) == 1 && quit == 0) {
			switch (e.type){
				case (SDL_QUIT) :
					FreeBoardsAndTree(s);
					SDL_FreeSurface(s->self);
					free(s);
					quit = 1;
					break;
				case (SDL_MOUSEBUTTONUP) :
					HandlePress(s, loadAndSaveP, e);
					break;
				default:
					break;
			}
		}
	}
	return 0;
}

void Load(window *s, char* path){
	int i;
	storedBoard = (char**)calloc(BOARD_SIZE, sizeof(char *));
	if (storedBoard == NULL){
		printf("%s", CALLOC_FAILED);
		exit(0);
	}
	for (i = 0; i < BOARD_SIZE; i++){
		storedBoard[i] = (char*)calloc(BOARD_SIZE, sizeof(char));
		if (storedBoard[i] == NULL){
			printf("%s", CALLOC_FAILED);
			exit(0);
		}
	}
	SetFromFileGui(path);
	chooseBoard = 2;
	CreateGameModeWindow(s);
}

void SetFromFileGui(char* file_path){
	int i, j;
	char str[51];
	FILE *f = fopen(file_path, "r");
	if (f == NULL){
		printf("%s",wrongFileError);
		return;
	}
	for (i = 0; i < 5; i++)
		fscanf(f, "%s", str);
	if (*(str + 1) == 'n'){ // next_turn exist
			if (*(str + 11) == 'W')
				storedTurn = WHITE;
			else
				storedTurn = BLACK;
			fscanf(f, "%s", str);
		}
		else
			storedTurn = WHITE;
	if (*(str + 1) == 'g'){ // game_mode exist
		storedGameMode = *(str + 11) - '0';
		fscanf(f, "%s", str);
	}
	else
		storedGameMode = 1;
	if (*(str + 1) == 'd'){ // difficulty exist
		if (*(str + 12) == 'B'){
			storedDifficulty = -1;
		}
		else if(*str == '<'){
			storedDifficulty = 1;
		}
		else{
			storedDifficulty = *(str + 12) - '0';
		}
		fscanf(f, "%s", str);
	}
	else
		storedDifficulty = 1;
	if (*(str + 1) == 'u'){ // user_color exist
		if (*(str + 12) == 'B' || *(str + 12) == 'b')
			storedPlayerColor = BLACK;
		else
			storedPlayerColor = WHITE;
		fscanf(f, "%s", str);
	}
	else
		storedPlayerColor = WHITE;

	for (j = 7; j > -1; j--){
		fscanf(f, "%s", str);
		for (i = 0; i < BOARD_SIZE; i++){
			if (*(str + 7 + i) != '_')
				storedBoard[i][j] = *(str + 7 + i);
			else
				storedBoard[i][j] = EMPTY;
		}
	}
	fscanf(f, "%s", str);
	fscanf(f, "%s", str);
	if (*(str + 1) == 'g'){
		castle_00 = *(str + 9) - '0';
		castle_07 = *(str + 11) - '0';
		castle_70 = *(str + 13) - '0';
		castle_77 = *(str + 15) - '0';
	}
	else {
		if (storedBoard[0][0] != WHITE_R)
			castle_00 = 1;
		if (storedBoard[7][0] != WHITE_R)
			castle_70 = 1;
		if (storedBoard[0][7] != BLACK_R)
			castle_07 = 1;
		if (storedBoard[7][7] != BLACK_R)
			castle_77 = 1;
		if (storedBoard[4][0] != WHITE_K){
			castle_00 = 1;
			castle_70 = 1;
		}
		if (storedBoard[4][7] != BLACK_K){
			castle_07 = 1;
			castle_77 = 1;
		}
	}
	fclose(f);
}

int Load1(window *s){
	if (access("1.xml", 0) != -1)
		Load(s, "1.xml");
	else
		isThereProblemToLoad = 1;
	return 0;
}

int Load2(window *s){
	if (access("2.xml", 0) != -1)
		Load(s, "2.xml");
	else
		isThereProblemToLoad = 1;
	return 0;
}

int Load3(window *s){
	if (access("3.xml", 0) != -1)
		Load(s, "3.xml");
	else
		isThereProblemToLoad = 1;
	return 0;
}

int Load4(window *s){
	if (access("4.xml", 0) != -1)
		Load(s, "4.xml");
	else
		isThereProblemToLoad = 1;
	return 0;
}

int Load5(window *s){
	if (access("5.xml", 0) != -1)
		Load(s, "5.xml");
	else
		isThereProblemToLoad = 1;
	return 0;
}

int Load6(window *s){
	if (access("6.xml", 0) != -1)
		Load(s, "6.xml");
	else
		isThereProblemToLoad = 1;
	return 0;
}

int Load7(window *s){
	if (access("7.xml", 0) != -1)
		Load(s, "7.xml");
	else
		isThereProblemToLoad = 1;
	return 0;
}

int Load8(window *s){
	if (access("8.xml", 0) != -1)
		Load(s, "8.xml");
	else
		isThereProblemToLoad = 1;
	return 0;
}

int Load9(window *s){
	if (access("9.xml", 0) != -1)
		Load(s, "9.xml");
	else
		isThereProblemToLoad = 1;
	return 0;
}

int Load10(window *s){
	if (access("10.xml", 0) != -1)
		Load(s, "10.xml");
	else
		isThereProblemToLoad = 1;
	return 0;
}

int SaveGame(window *s){
	control *one; control *two; control *three; control *four; control *five;
	control *six; control *seven; control *eight; control *nine; control *ten; control *child;
	control *load_and_save_p = CreatePanel(0, 0, SCREEN_W, SCREEN_H, redBackgroundcolor, blueBackgroundcolor, greenBackgroundcolor);
	control *save_game = CreateLabel(150, 10, 572, 161, UploadPic("saveGameLabel.bmp"));
	control *cancel = CreateButton(250, 470, 380, 106, UploadPic("cancelFinal.bmp"), &BackFromSave, 'x');
	FreeUITree(s->panel_child);
	s->panel_child = NULL;
	AddPanelToWindow(s, load_and_save_p);
	AddLabelToPanel(load_and_save_p, save_game, s);
	if (1 <= SLOTS){
		one = CreateButton(100, 250, 80, 80, UploadPic("1n.bmp"), &Save1, 'x');
		AddButtonToPanel(load_and_save_p, one, s);
	}
	if (2 <= SLOTS){
		two = CreateButton(200, 250, 80, 80, UploadPic("2n.bmp"), &Save2, 'x');
		AddButtonToPanel(load_and_save_p, two, s);
	}
	if (3 <= SLOTS){
		three = CreateButton(300, 250, 80, 80, UploadPic("3n.bmp"), &Save3, 'x');
		AddButtonToPanel(load_and_save_p, three, s);
	}
	if (4 <= SLOTS){
		four = CreateButton(400, 250, 80, 80, UploadPic("4n.bmp"), &Save4, 'x');
		AddButtonToPanel(load_and_save_p, four, s);
	}
	if (5 <= SLOTS){
		five = CreateButton(500, 250, 80, 80, UploadPic("5n.bmp"), &Save5, 'x');
		AddButtonToPanel(load_and_save_p, five, s);
	}
	if (6 <= SLOTS){
		six = CreateButton(600, 250, 80, 80, UploadPic("6n.bmp"), &Save6, 'x');
		AddButtonToPanel(load_and_save_p, six, s);
	}
	if (7 <= SLOTS){
		seven = CreateButton(700, 250, 80, 80, UploadPic("7n.bmp"), &Save7, 'x');
		AddButtonToPanel(load_and_save_p, seven, s);
	}
	if (8 <= SLOTS){
		eight = CreateButton(200, 350, 80, 80, UploadPic("8n.bmp"), &Save8, 'x');
		AddButtonToPanel(load_and_save_p, eight, s);
	}
	if (9 <= SLOTS){
		nine = CreateButton(400, 350, 80, 80, UploadPic("9n.bmp"), &Save9, 'x');
		AddButtonToPanel(load_and_save_p, nine, s);
	}
	if (10 <= SLOTS){
		ten = CreateButton(600, 350, 80, 80, UploadPic("10n.bmp"), &Save10, 'x');
		AddButtonToPanel(load_and_save_p, ten, s);
	}
	AddButtonToPanel(load_and_save_p, cancel, s);
	if (SDL_Flip(s->self) != 0){
		printf(sdlBufferError, SDL_GetError());
		exit(0);
	}
	while (quit == 0){
		child = load_and_save_p->Panel->children;
		while (child->next_bro != NULL){
			if (child->next_bro->Label!= NULL){
				SDL_FreeSurface(child->next_bro->Label->pic);
				free(child->next_bro->Label);
				free(child->next_bro);
				child->next_bro = NULL;
				break;
			}
			child = child->next_bro;
		}
		if (SDL_Flip(s->self) != 0){
			printf(sdlBufferError, SDL_GetError());
			exit(0);
		}
		SDL_Event e;
		while (SDL_PollEvent(&e) == 1 && quit == 0) {
			switch (e.type){
				case (SDL_QUIT) :
					FreeBoardsAndTree(s);
					SDL_FreeSurface(s->self);
					free(s);
					quit = 1;
					break;
				case (SDL_MOUSEBUTTONUP) :
					HandlePress(s, load_and_save_p, e);
					break;
				default:
					break;
			}
		}
	}
	return 0;
}

void SaveGui(char* file_name){
	int i, j;
	FILE *f = fopen(file_name, "w");
	if (f==NULL){
		printf("%s",wrongFileError);
		return;
	}
	fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(f, "<game>\n");
	if (turn == 1)
		fprintf(f, "\t<next_turn>White</next_turn>\n");
	else
		fprintf(f, "\t<next_turn>Black</next_turn>\n");
	fprintf(f, "\t<game_mode>%d</game_mode>\n", gameMode);
	if (gameMode == 2){
		if (difficulty>0)
			fprintf(f, "\t<difficulty>%d</difficulty>\n", difficulty);
		else
			fprintf(f, "\t<difficulty>Best/difficulty>\n");
		if (playerColor == WHITE)
			fprintf(f, "\t<user_color>White</user_color>\n");
		else
			fprintf(f, "\t<user_color>Black</user_color>\n");
	}
	else{
		fprintf(f, "\t<difficulty></difficulty>\n");
		fprintf(f, "\t<user_color></user_color>\n");
	}
	fprintf(f, "\t<board>\n");
	for (j = BOARD_SIZE; j > 0; j--){
		fprintf(f, "\t\t<row_%d>", j);
		for (i = 0; i < BOARD_SIZE; i++){
			if (lastBoard[i][j - 1] == EMPTY)
				fprintf(f, "_");
			else
				fprintf(f, "%c", lastBoard[i][j - 1]);
		}
		fprintf(f, "</row_%d>\n", j);
	}
	fprintf(f, "\t</board>\n");
	fprintf(f, "\t<general>%d,%d,%d,%d</general>\n", castle00, castle07, castle70, castle77);
	fprintf(f, "</game>\n");
	fclose(f);
}

int Save1(window *s){
	SaveGui("1.xml");
	numberOfSavedGame = 1;
	return 0;
}

int Save2(window *s){
	SaveGui("2.xml");
	numberOfSavedGame = 2;
	return 0;
}

int Save3(window *s){
	SaveGui("3.xml");
	numberOfSavedGame = 3;
	return 0;
}

int Save4(window *s){
	SaveGui("4.xml");
	numberOfSavedGame = 4;
	return 0;
}

int Save5(window *s){
	SaveGui("5.xml");
	numberOfSavedGame = 5;
	return 0;
}

int Save6(window *s){
	SaveGui("6.xml");
	numberOfSavedGame = 6;
	return 0;
}

int Save7(window *s){
	SaveGui("7.xml");
	numberOfSavedGame = 7;
	return 0;
}

int Save8(window *s){
	SaveGui("8.xml");
	numberOfSavedGame = 8;
	return 0;
}
int Save9(window *s){
	SaveGui("9.xml");
	numberOfSavedGame = 9;
	return 0;
}
int Save10(window *s){
	SaveGui("10.xml");
	numberOfSavedGame = 10;
	return 0;
}
int BackFromSave(window *s){
	chooseBoard = 1;
	CreateGameWindow(s);
	return 0;
}

int OnBoard(int x, int y){
	return !(x > 640 || x < 0 || y > 640 || y < 0);
}

int ColorByCoordinates(char** board, int x, int y){

	location loc;
	loc.column = x / 80;
	loc.row = 7 - y / 80;
	return getPieceColor(board, loc);
}

int CreateBoardSettingsWindow(window *s){
	char** current_board = NULL;
		control *menu_panel = NULL; control *game_panel = NULL; control *label= NULL; control *main_menu = NULL;
		control *white = NULL; control *black = NULL; control *pawn = NULL; control *bishop = NULL; control *rook = NULL;
		control *knight = NULL; control *queen = NULL; control *king = NULL; control *delete = NULL;
		control *startGame = NULL; control *child = NULL; control *full_panel = NULL;
		isThereWin = 0;
		settingsAddColor = WHITE;		/*****/
		int k;
		pressNumber = 0;
		numberOfSavedGame = 0;
		firstPressX = -1;
		firstPressY = -1;
		secondPressX = -1;
		secondPressY = -1;
		FreeUITree(s->panel_child);
		s->panel_child = NULL;
		full_panel = CreatePanel(0, 0, SCREEN_W, SCREEN_H, redBackgroundcolor, blueBackgroundcolor, greenBackgroundcolor);
		AddPanelToWindow(s, full_panel);
		RebuildSettingsBoardMenu(s, &full_panel, &menu_panel,&label, &main_menu, &white, &black, &pawn ,&bishop
				, &rook, &knight, &queen, &king, &delete, &startGame);
		if (chooseBoard == 0){
			current_board = (char**)calloc(BOARD_SIZE, sizeof(char*));
			if (current_board == NULL){
				printf("%s", CALLOC_FAILED);
				exit(0);
			}
			for (k = 0; k < BOARD_SIZE; k++){
				current_board[k] = (char*)calloc(BOARD_SIZE, sizeof(char));
				if (current_board == NULL){
					printf("%s", CALLOC_FAILED);
					exit(0);
				}
			}
			initFullBoard(current_board);
			storedBoard = copyBoard(current_board);
		}
		if (chooseBoard == 1){
			current_board = copyBoard(lastBoard);
			freeBoard(lastBoard);
			lastBoard = NULL;
		}
		if (chooseBoard == 2){
			current_board = copyBoard(storedBoard);
			if (lastBoard != NULL){
				freeBoard(lastBoard);
				lastBoard = NULL;
			}
			castle00 = castle_00;
			castle70 = castle_70;
			castle07 = castle_07;
			castle77 = castle_77;
			gameMode = storedGameMode;
			playerColor = storedPlayerColor;
			difficulty = storedDifficulty;
			turn = storedTurn;
		}
		if (chooseBoard == 3){
			current_board = copyBoard(lastBoard);
			freeBoard(lastBoard);
			lastBoard = NULL;
			actuallyExecuteMoveGui(current_board, *move, turn);
			free(move);
			turn = flipColor(turn);	/*****/
		}
		lastBoard = copyBoard(current_board);
		chooseBoard = 0;
		game_panel = CreatePanel(0, 0, 640, SCREEN_H, 255, 255, 255);
		AddPanelToPanel(full_panel, game_panel, s);
		createGame(s, game_panel, current_board);
		if (SDL_Flip(s->self) != 0){
			printf(sdlBufferError, SDL_GetError());
			exit(0);
		}
		while (quit == 0){
			if (isThereWin == 0){
				if (lastBoard != NULL){
					freeBoard(lastBoard);
					lastBoard = NULL;
				}
				lastBoard = copyBoard(current_board);
			}
			SDL_Event e;
			while (SDL_PollEvent(&e) == 1 && quit == 0) {
				switch (e.type){
					case (SDL_QUIT) :
						FreeBoardsAndTree(s);
						if (current_board != NULL){
							freeBoard(current_board);
							current_board = NULL;
						}
						SDL_FreeSurface(s->self);
						free(s);
						quit = 1;
						break;
					case (SDL_MOUSEBUTTONUP) :
						chooseBoard = 1;
						freeBoard(current_board);
						HandlePress(s, full_panel, e);
						if (lastBoard != NULL)
							current_board = copyBoard(lastBoard);
						if (quit != 1 && isThereWin==0){
							if (pressNumber == 0){
								if (OnBoard(firstPressX, firstPressY)){
									pressNumber++;
								}
								FreeUITree(s->panel_child);
								s->panel_child = NULL;
								full_panel = CreatePanel(0, 0, SCREEN_W, SCREEN_H, redBackgroundcolor, blueBackgroundcolor, greenBackgroundcolor);
								AddPanelToWindow(s, full_panel);
								RebuildSettingsBoardMenu(s, &full_panel, &menu_panel,&label, &main_menu, &white, &black, &pawn ,&bishop
										, &rook, &knight, &queen, &king, &delete, &startGame);
								current_board = copyBoard(lastBoard);
								freeBoard(lastBoard);
								lastBoard = copyBoard(current_board);
								chooseBoard = 0;
								game_panel = CreatePanel(0, 0, 640, SCREEN_H, 255, 255, 255);
								AddPanelToPanel(full_panel, game_panel, s);
									createGame(s, game_panel, current_board);
								if (SDL_Flip(s->self) != 0){
								printf(sdlBufferError, SDL_GetError());
									exit(0);
								}
							}
							else{
								pressNumber = 0;
								if (OnBoard(secondPressX, secondPressY)){
									move = (moveElement*)calloc(1, sizeof(moveElement));
									if (move == NULL){
										printf("%s", CALLOC_FAILED);
										exit(0);
									}
									move->type = 0;
									move->from.column = firstPressX / 80;
									move->from.row = 7 - firstPressY / 80;
									move->to.column = secondPressX / 80;
									move->to.row = 7 - secondPressY / 80;
										changeCastlingGlobals(current_board, *move);
										if (current_board[move->from.column][move->from.row] != EMPTY && current_board[move->to.column][move->to.row] == EMPTY){
											current_board[move->to.column][move->to.row] = current_board[move->from.column][move->from.row];
											current_board[move->from.column][move->from.row] = EMPTY;
										}
										if (lastBoard != NULL)
											freeBoard(lastBoard);
										lastBoard = copyBoard(current_board);
										free(move);
										FreeUITree(s->panel_child->Panel->children->next_bro);
										s->panel_child->Panel->children->next_bro = NULL;
										child = menu_panel->Panel->children;
										while (child->next_bro != NULL){
											if (child->next_bro->Label != NULL){
												full_panel->Panel->children = NULL;
												FreeUITree(menu_panel);
												RebuildSettingsBoardMenu(s, &full_panel, &menu_panel,&label, &main_menu, &white, &black, &pawn ,&bishop
														, &rook, &knight, &queen, &king, &delete, &startGame);
												break;
											}
											child = child->next_bro;
										}
										game_panel = CreatePanel(0, 0, 640, SCREEN_H, 255, 255, 255);
										AddPanelToPanel(full_panel, game_panel, s);
										createGame(s, game_panel, current_board);
										if (SDL_Flip(s->self) != 0){
											printf(sdlBufferError, SDL_GetError());
											exit(0);
										}

								}
								else{
									FreeUITree(s->panel_child->Panel->children->next_bro);
									s->panel_child->Panel->children->next_bro = NULL;
									child = menu_panel->Panel->children;
									while (child->next_bro != NULL){
										if (child->next_bro->Label != NULL){
											full_panel->Panel->children = NULL;
											FreeUITree(menu_panel);
											RebuildSettingsBoardMenu(s, &full_panel, &menu_panel,&label, &main_menu, &white, &black, &pawn ,&bishop
													, &rook, &knight, &queen, &king, &delete, &startGame);
											break;
										}
										child = child->next_bro;
									}
									game_panel = CreatePanel(0, 0, 640, SCREEN_H, 255, 255, 255);
									AddPanelToPanel(full_panel, game_panel, s);
									createGame(s, game_panel, current_board);
									if (SDL_Flip(s->self) != 0){
										printf(sdlBufferError, SDL_GetError());
										exit(0);
									}
								}
						}
					}
					break;
					default:
						break;
				}
			}
		}
		return 0;
}

void handleChooseBoard(char** current_board){
	int k;
	if (chooseBoard == 0){
				current_board = (char**)calloc(BOARD_SIZE, sizeof(char*));
				if (current_board == NULL){
					printf("%s", CALLOC_FAILED);
					exit(0);
				}
				for (k = 0; k < BOARD_SIZE; k++){
					current_board[k] = (char*)calloc(BOARD_SIZE, sizeof(char));
					if (current_board == NULL){
						printf("%s", CALLOC_FAILED);
						exit(0);
					}
				}
				initFullBoard(current_board);
				storedBoard = copyBoard(current_board);
			}
			if (chooseBoard == 1){
				current_board = copyBoard(lastBoard);
				freeBoard(lastBoard);
				lastBoard = NULL;
			}
			if (chooseBoard == 2){
				current_board = copyBoard(storedBoard);
				if (lastBoard != NULL){
					freeBoard(lastBoard);
					lastBoard = NULL;
				}
				castle00 = castle_00;
				castle70 = castle_70;
				castle07 = castle_07;
				castle77 = castle_77;
				gameMode = storedGameMode;
				playerColor = storedPlayerColor;
				difficulty = storedDifficulty;
				turn = storedTurn;
			}
			if (chooseBoard == 3){
				current_board = copyBoard(lastBoard);
				freeBoard(lastBoard);
				lastBoard = NULL;
				actuallyExecuteMoveGui(current_board, *move, turn);
				free(move);
				turn = flipColor(turn);
			}
}

int CreateGameWindow(window *s){
	char** current_board = NULL;
	control *menu_panel = NULL; control *game_panel = NULL; control *save_game = NULL; control *main_menu = NULL;
	control *quit_b = NULL; control *check = NULL; control *tie = NULL; control *win = NULL; control *child = NULL; control *full_panel = NULL;
	moveElement computer_move;
	isThereWin = 0;
	int i, j, a, b, k, isBest = 0; /*****/
	pressNumber = 0;
	numberOfSavedGame = 0;
	firstPressX = -1;
	firstPressY = -1;
	secondPressX = -1;
	secondPressY = -1;
	FreeUITree(s->panel_child);
	s->panel_child = NULL;
	full_panel = CreatePanel(0, 0, SCREEN_W, SCREEN_H, redBackgroundcolor, blueBackgroundcolor, greenBackgroundcolor);
	AddPanelToWindow(s, full_panel);
	RebuildGameMenu(s, &full_panel, &menu_panel, &save_game, &main_menu, &quit_b);
	turn = storedTurn;
	if (chooseBoard == 0){
		current_board = (char**)calloc(BOARD_SIZE, sizeof(char*));
		if (current_board == NULL){
			printf("%s", CALLOC_FAILED);
			exit(0);
		}
		for (k = 0; k < BOARD_SIZE; k++){
			current_board[k] = (char*)calloc(BOARD_SIZE, sizeof(char));
			if (current_board == NULL){
				printf("%s", CALLOC_FAILED);
				exit(0);
			}
		}
		initFullBoard(current_board);
		storedBoard = copyBoard(current_board);
	}
	if (chooseBoard == 1){
		current_board = copyBoard(lastBoard);
		freeBoard(lastBoard);
		lastBoard = NULL;
	}
	if (chooseBoard == 2){
		current_board = copyBoard(storedBoard);
		if (lastBoard != NULL){
			freeBoard(lastBoard);
			lastBoard = NULL;
		}
		castle00 = castle_00;
		castle70 = castle_70;
		castle07 = castle_07;
		castle77 = castle_77;
		gameMode = storedGameMode;
		playerColor = storedPlayerColor;
		difficulty = storedDifficulty;
	}
	if (chooseBoard == 3){
		current_board = copyBoard(lastBoard);
		freeBoard(lastBoard);
		lastBoard = NULL;
		actuallyExecuteMoveGui(current_board, *move, turn);
		free(move);
		turn = flipColor(turn);
	}
	lastBoard = copyBoard(current_board);
	if (chooseBoard != 0){
		if (isAWinner(turn, current_board)){
			if (turn == BLACK)
				win = CreateLabel(5, 250, 250, 70, UploadPic("blackWins.bmp"));
			else
				win = CreateLabel(5, 250, 250, 70, UploadPic("whiteWins.bmp"));
			AddLabelToPanel(menu_panel, win, s);
			isThereWin = 1;
		}
		else if (isACheck(current_board, turn)){
			check = CreateLabel(5, 350, 250, 70, UploadPic("smallCheck.bmp"));
			AddLabelToPanel(menu_panel, check, s);
		}
		else if (isATie(current_board, turn)){
			tie = CreateLabel(5, 350, 250, 70, UploadPic("smallTie.bmp"));
			AddLabelToPanel(menu_panel, tie, s);
		}
	}
	chooseBoard = 0;
	game_panel = CreatePanel(0, 0, 640, SCREEN_H, 255, 255, 255);
	AddPanelToPanel(full_panel, game_panel, s);
	createGame(s, game_panel, current_board);
	if (SDL_Flip(s->self) != 0){
		printf(sdlBufferError, SDL_GetError());
		exit(0);
	}
	while (quit == 0){
		if (isThereWin == 0){
			if (lastBoard != NULL){
				freeBoard(lastBoard);
				lastBoard = NULL;
			}
			lastBoard = copyBoard(current_board);
			if (gameMode == 2 && turn != playerColor){
				if (difficulty == -1) {
					isBest = 1;
					difficulty = 4;
				}
				computer_move = calcNextMove(current_board, difficulty, flipColor(playerColor), isBest);
				changeCastlingGlobals(current_board, computer_move);
				actuallyExecuteMoveGui(current_board, computer_move, turn);
				if (lastBoard != NULL)
					freeBoard(lastBoard);
				lastBoard = copyBoard(current_board);
				turn = flipColor(turn);
				FreeUITree(s->panel_child->Panel->children->next_bro);
				s->panel_child->Panel->children->next_bro = NULL;
				child = menu_panel->Panel->children;
				while (child->next_bro != NULL){
					if (child->next_bro->Label != NULL){
						full_panel->Panel->children = NULL;
						FreeUITree(menu_panel);
						RebuildGameMenu(s, &full_panel, &menu_panel, &save_game, &main_menu, &quit_b);
						break;
					}
					child = child->next_bro;
				}
				if (isAWinner(flipColor(turn),current_board)){
					if (turn == BLACK)
						win = CreateLabel(5, 250, 250, 70, UploadPic("blackWins.bmp"));
					else
						win = CreateLabel(5, 250, 250, 70, UploadPic("whiteWins.bmp"));
					AddLabelToPanel(menu_panel, win, s);
					isThereWin = 1;
				}
				else if (isACheck(current_board, turn)){
					check = CreateLabel(5, 350, 250, 70, UploadPic("smallCheck.bmp"));
					AddLabelToPanel(menu_panel, check, s);
				}
				else if (isATie(current_board, turn)){
					tie = CreateLabel(5, 350, 250, 70, UploadPic("smallTie.bmp"));
					AddLabelToPanel(menu_panel, tie, s);
				}
				game_panel = CreatePanel(0, 0, 640, SCREEN_H, 255, 255, 255);
				AddPanelToPanel(full_panel, game_panel, s);
				createGame(s, game_panel, current_board);
				if (SDL_Flip(s->self) != 0){
					printf(sdlBufferError, SDL_GetError());
					exit(0);
				}
			}
		}
		SDL_Event e;
		while (SDL_PollEvent(&e) == 1 && quit == 0) {
			switch (e.type){
				case (SDL_QUIT) :
					FreeBoardsAndTree(s);
					if (current_board != NULL){
						freeBoard(current_board);
						current_board = NULL;
					}
					SDL_FreeSurface(s->self);
					free(s);
					quit = 1;
					break;
				case (SDL_MOUSEBUTTONUP) :
					freeBoard(current_board);
					HandlePress(s, full_panel, e);
					if (lastBoard != NULL)
						current_board = copyBoard(lastBoard);
					if (quit != 1 && isThereWin==0){
						if (pressNumber == 0){
							if (OnBoard(firstPressX, firstPressY)){
								if (ColorByCoordinates(current_board, firstPressX, firstPressY) == turn)
									pressNumber++;
								else{
									firstPressX = -1;
									firstPressY = -1;
								}
							}
						}
						else{
							if (OnBoard(secondPressX, secondPressY)){
								move = (moveElement*)calloc(1, sizeof(moveElement));
								if (move == NULL){
									printf("%s", CALLOC_FAILED);
									exit(0);
								}
								i = firstPressX / 80;
								j = 7 - firstPressY / 80;
								a = secondPressX / 80;
								b = 7 - secondPressY / 80;
								if (turn == WHITE && i == 0 && j == 0 && a == 4 && b == 0 && current_board[0][0] == WHITE_R && current_board[4][0] == WHITE_K){
									move->type = 0;
									move->from.column = i;
									move->from.row = j;
									move->to.column = -100;
									move->to.row = -100;
								}
								else if (turn == WHITE && i == 7 && j == 0 && a == 4 && b == 0 && current_board[7][0] == WHITE_R && current_board[4][0] == WHITE_K){
									move->type = 0;
									move->from.column = i;
									move->from.row = j;
									move->to.column = -100;
									move->to.row = -100;
								}
								else if (turn == BLACK && i == 0 && j == 7 && a == 4 && b == 7 && current_board[0][7] == BLACK_R && current_board[4][7] == BLACK_K){
									move->type = 0;
									move->from.column = i;
									move->from.row = j;
									move->to.column = -100;
									move->to.row = -100;
								}
								else if (turn == BLACK && i == 7 && j == 7 && a == 4 && b == 7 && current_board[7][7] == BLACK_R && current_board[4][7] == BLACK_K){
									move->type = 0;
									move->from.column = i;
									move->from.row = j;
									move->to.column = -100;
									move->to.row = -100;
								}
								else{
									move->type = 0;
									move->from.column = firstPressX / 80;
									move->from.row = 7 - firstPressY / 80;
									move->to.column = secondPressX / 80;
									move->to.row = 7 - secondPressY / 80;
								}
								if (!IsMovePossible(current_board, *move, turn)){
									free(move);
									freeBoard(current_board);
									RevertIllegalMove(s);
									quit = 1;
								}
								else{
									pressNumber = 0;
									if (isPawnPromotionNeeded(current_board, move) == 0){
										changeCastlingGlobals(current_board, *move);
										actuallyExecuteMoveGui(current_board, *move, turn);
										if (lastBoard != NULL)
											freeBoard(lastBoard);
										lastBoard = copyBoard(current_board);
										turn = flipColor(turn);
										free(move);
										FreeUITree(s->panel_child->Panel->children->next_bro);
										s->panel_child->Panel->children->next_bro = NULL;
										child = menu_panel->Panel->children;
										while (child->next_bro != NULL){
											if (child->next_bro->Label != NULL){
												full_panel->Panel->children = NULL;
												FreeUITree(menu_panel);
												RebuildGameMenu(s, &full_panel, &menu_panel, &save_game, &main_menu, &quit_b);
												break;
											}
											child = child->next_bro;
										}
										if (isAWinner(turn,current_board)){
											if (turn == BLACK)
												win = CreateLabel(5, 250, 250, 70, UploadPic("blackWins.bmp"));
											else
												win = CreateLabel(5, 250, 250, 70, UploadPic("whiteWins.bmp"));
											AddLabelToPanel(menu_panel, win, s);
											isThereWin = 1;
										}
										else if (isACheck(current_board, turn)){
											check = CreateLabel(5, 350, 250, 70, UploadPic("smallCheck.bmp"));
											AddLabelToPanel(menu_panel, check, s);
										}
										else if (isATie(current_board,flipColor(turn))){
											tie = CreateLabel(5, 350, 250, 70, UploadPic("smallTie.bmp"));
											AddLabelToPanel(menu_panel, tie, s);
										}
										game_panel = CreatePanel(0, 0, 640, SCREEN_H, 255, 255, 255);
										AddPanelToPanel(full_panel, game_panel, s);
										createGame(s, game_panel, current_board);
										if (SDL_Flip(s->self) != 0){
											printf(sdlBufferError, SDL_GetError());
											exit(0);
										}
									}
									else{
										freeBoard(current_board);
										ExecutePawnPromotion(s);
									}
								}
							}
						}
					}
					break;
					default:
						break;
			}
		}
	}
	return 0;
}


void RebuildGameMenu(window *s, control **full_panel, control **menu_panel, control **save_game, control **main_menu, control **quit_b){
	*menu_panel = CreatePanel(640, 0, 460, SCREEN_H, redBackgroundcolor, blueBackgroundcolor, greenBackgroundcolor);
	*save_game = CreateButton(5, 50, 250, 70, UploadPic("saveGame.bmp"), &SaveGame, 'x');
	*main_menu = CreateButton(5, 150, 250, 70, UploadPic("mainMenu.bmp"), &CreateMainWindow, 'x');
	*quit_b = CreateButton(5, 550, 250, 70, UploadPic("quitSmall.bmp"), &QuitGame, 'x');
	AddPanelToPanel(*full_panel, *menu_panel, s);
	AddButtonToPanel(*menu_panel, *save_game, s);
	AddButtonToPanel(*menu_panel, *main_menu, s);
	AddButtonToPanel(*menu_panel, *quit_b, s);
}

void RebuildSettingsBoardMenu(window *s, control **full_panel, control **menu_panel, control **main_menu, control **label
		, control **white, control **black, control **pawn ,control **bishop
		, control **rook, control **knight, control **queen, control **king, control **delete
		, control **next){
	*menu_panel = CreatePanel(640, 0, 460, SCREEN_H, redBackgroundcolor, blueBackgroundcolor, greenBackgroundcolor);
	*label = CreateLabel(5, 20, 250, 55, UploadPic("settingsWindowLabel.bmp"));
	*main_menu = CreateButton(5, 100, 250, 70, UploadPic("mainMenu.bmp"), &CreateMainWindow, 'x');
	if (settingsAddColor == WHITE)
		*white = CreateButton(70, 250, 80, 80, UploadPic("wbS.bmp"), &setAddColorAsWhite, 'x');
	else
		*white = CreateButton(70, 250, 80, 80, UploadPic("wnS.bmp"), &setAddColorAsWhite, 'x');
	if (settingsAddColor == BLACK)
		*black = CreateButton(130, 250, 80, 80, UploadPic("bbS.bmp"), &setAddColorAsBlack, 'x');
	else
		*black = CreateButton(130, 250, 80, 80, UploadPic("bnS.bmp"), &setAddColorAsBlack, 'x');
	*pawn = CreateButton(2, 350, 125, 34, UploadPic("pawnF.bmp"), &addPawnToTheBoard, 'x');
	*bishop = CreateButton(133, 350, 125, 34, UploadPic("bishopF.bmp"), &addBishopToTheBoard, 'x');
	*rook = CreateButton(2, 400, 125, 34, UploadPic("rookF.bmp"), &addRookToTheBoard, 'x');
	*knight = CreateButton(133, 400, 125, 34, UploadPic("knightF.bmp"), &addKnightToTheBoard, 'x');
	*queen = CreateButton(2, 450, 125, 34, UploadPic("queenF.bmp"), &addQweenToTheBoard, 'x');
	*king = CreateButton(133, 450, 125, 34, UploadPic("KingF.bmp"), &addKingToTheBoard, 'x');
	*delete = CreateButton(70, 500, 130, 36, UploadPic("deleteS.bmp"), &deletePawn, 'x');
	*next = CreateButton(5, 550, 250, 70, UploadPic("next.bmp"), &CreateNextPlayerWindow, 'x');
	AddPanelToPanel(*full_panel, *menu_panel, s);
	AddLabelToPanel(*menu_panel, *label, s);
	AddButtonToPanel(*menu_panel, *main_menu, s);
	AddButtonToPanel(*menu_panel, *white, s);
	AddButtonToPanel(*menu_panel, *black, s);

	AddButtonToPanel(*menu_panel, *pawn, s);
	AddButtonToPanel(*menu_panel, *bishop, s);
	AddButtonToPanel(*menu_panel, *rook, s);
	AddButtonToPanel(*menu_panel, *knight, s);
	AddButtonToPanel(*menu_panel, *queen, s);
	AddButtonToPanel(*menu_panel, *king, s);
	AddButtonToPanel(*menu_panel, *delete, s);

	AddButtonToPanel(*menu_panel, *next, s);
}

int addPawnToTheBoard(window *s){
	if(settingsAddColor == 1){
		if(countPiece(lastBoard, WHITE_P) < 8 && firstPressX != -1 && firstPressY != -1 && OnBoard(firstPressX,firstPressY)){
			lastBoard[firstPressX/80][7 - firstPressY/80] = WHITE_P;
		}
	}
	else{
		if(countPiece(lastBoard, BLACK_P) < 8 && firstPressX != -1 && firstPressY != -1 && OnBoard(firstPressX,firstPressY)){
			lastBoard[firstPressX/80][7 - firstPressY/80] = BLACK_P;
				}
	}
	return 0;
}

int addBishopToTheBoard(window *s){
	if(settingsAddColor == WHITE){
		if(countPiece(lastBoard, WHITE_B) < 2 && firstPressX != -1 && firstPressY != -1 && OnBoard(firstPressX,firstPressY)){
			lastBoard[firstPressX/80][7 - firstPressY/80] = WHITE_B;
		}
	}
	else{
		if(countPiece(lastBoard, BLACK_B) < 2 && firstPressX != -1 && firstPressY != -1 && OnBoard(firstPressX,firstPressY)){
			lastBoard[firstPressX/80][7 - firstPressY/80] = BLACK_B;
				}
	}
	return 0;
}

int addRookToTheBoard(window *s){
	if(settingsAddColor == WHITE){
		if(countPiece(lastBoard, WHITE_R) < 2 && firstPressX != -1 && firstPressY != -1 && OnBoard(firstPressX,firstPressY)){
			lastBoard[firstPressX/80][7 - firstPressY/80] = WHITE_R;
		}
	}
	else{
		if(countPiece(lastBoard, BLACK_R) < 2 && firstPressX != -1 && firstPressY != -1 && OnBoard(firstPressX,firstPressY)){
			lastBoard[firstPressX/80][7 - firstPressY/80] = BLACK_R;
				}
	}
	return 0;
}

int addKnightToTheBoard(window *s){
	if(settingsAddColor == WHITE){
		if(countPiece(lastBoard, WHITE_N) < 2 && firstPressX != -1 && firstPressY != -1 && OnBoard(firstPressX,firstPressY)){
			lastBoard[firstPressX/80][7 - firstPressY/80] = WHITE_N;
		}
	}
	else{
		if(countPiece(lastBoard, BLACK_N) < 2 && firstPressX != -1 && firstPressY != -1 && OnBoard(firstPressX,firstPressY)){
			lastBoard[firstPressX/80][7 - firstPressY/80] = BLACK_N;
				}
	}
	return 0;
}

int addQweenToTheBoard(window *s){
	if(settingsAddColor == WHITE){
		if(countPiece(lastBoard, WHITE_Q) < 1 && firstPressX != -1 && firstPressY != -1 && OnBoard(firstPressX,firstPressY)){
			lastBoard[firstPressX/80][7 - firstPressY/80] = WHITE_Q;
		}
	}
	else{
		if(countPiece(lastBoard, BLACK_Q) < 1 && firstPressX != -1 && firstPressY != -1 && OnBoard(firstPressX,firstPressY)){
			lastBoard[firstPressX/80][7 - firstPressY/80] = BLACK_Q;
				}
	}
	return 0;
}

int addKingToTheBoard(window *s){
	if(settingsAddColor == WHITE){
		if(countPiece(lastBoard, WHITE_K) < 1 && firstPressX != -1 && firstPressY != -1 && OnBoard(firstPressX,firstPressY)){
			lastBoard[firstPressX/80][7 - firstPressY/80] = WHITE_K;
		}
	}
	else{
		if(countPiece(lastBoard, BLACK_K) < 1 && firstPressX != -1 && firstPressY != -1 && OnBoard(firstPressX,firstPressY)){
			lastBoard[firstPressX/80][7 - firstPressY/80] = BLACK_K;
				}
	}
	return 0;
}
int deletePawn(window* s){
	if(firstPressX != -1 && firstPressY != -1 && OnBoard(firstPressX,firstPressY) &&
			lastBoard[firstPressX/80][7- firstPressY/80] != WHITE_K && lastBoard[firstPressX/80][7- firstPressY/80] != BLACK_K){
	  lastBoard[firstPressX/80][7- firstPressY/80] = EMPTY;
	}
	return 0;
}
int setAddColorAsWhite(window* s){
	settingsAddColor = WHITE;
	return 0;
}

int setAddColorAsBlack(window* s){
	settingsAddColor = BLACK;
	return 0;
}

void createGame(window *s, control *panel_node, char** current_board){
	int col, row;
	for (col = 0; col < BOARD_SIZE; col++){
		for (row = 0; row < BOARD_SIZE; row++){
			if (current_board[col][row] == WHITE_P)
				Createm(s, panel_node, col, row);
			else if (current_board[col][row] == BLACK_P)
				CreateM(s, panel_node, col, row);
			else if (current_board[col][row] == WHITE_B)
				Createb(s, panel_node, col, row);
			else if (current_board[col][row] == BLACK_B)
				CreateB(s, panel_node, col, row);
			else if (current_board[col][row] == WHITE_R)
				Creater(s, panel_node, col, row);
			else if (current_board[col][row] == BLACK_R)
				CreateR(s, panel_node, col, row);
			else if (current_board[col][row] == WHITE_N)
				Createn(s, panel_node, col, row);
			else if (current_board[col][row] == BLACK_N)
				CreateN(s, panel_node, col, row);
			else if (current_board[col][row] == WHITE_Q)
				Createq(s, panel_node, col, row);
			else if (current_board[col][row] == BLACK_Q)
				CreateQ(s, panel_node, col, row);
			else if (current_board[col][row] == WHITE_K)
				Createk(s, panel_node, col, row);
			else if (current_board[col][row] == BLACK_K)
				CreateK(s, panel_node, col, row);
			else if (current_board[col][row] == EMPTY)
				CreateBlank(s, panel_node, col, row);
		}
	}
}

int IsMovePossible(char** current_board, moveElement move, int curTurn){
	linkedListElement *head, *possibleMoves;
	int isLegalMove = 0; // 1 yes, 0 no 
	head = getMoves(current_board, 0, 1, curTurn);
	possibleMoves = head;
	while (possibleMoves->data != NULL && possibleMoves->next->data != NULL){
		if (possibleMoves->data->from.column == move.from.column && possibleMoves->data->from.row == move.from.row && possibleMoves->data->to.column == move.to.column && possibleMoves->data->to.row == move.to.row){
			isLegalMove = 1;
			break;
		}
		possibleMoves = possibleMoves->next;
	}
	if (possibleMoves->data != NULL && possibleMoves->next->data == NULL && isLegalMove == 0)
		if (possibleMoves->data->from.column == move.from.column && possibleMoves->data->from.row == move.from.row && possibleMoves->data->to.column == move.to.column && possibleMoves->data->to.row == move.to.row)
			isLegalMove = 1;
	freeList(head);
	return isLegalMove;
}

int RevertIllegalMove(window *s){
	chooseBoard = 1;
	FreeUITree(s->panel_child);
	s->panel_child = NULL;
	if (SDL_Flip(s->self) != 0){
		printf(sdlBufferError, SDL_GetError());
		exit(0);
	}
	CreateGameWindow(s);
	return 0;
}

int ExecutePawnPromotion(window *s){
	int y = 150;
	int initX = 10;
	int w = 125;
	int h = 35;

	control *pawnPromotionPanel = CreatePanel(20, 220, 600, 250, redBackgroundcolor, blueBackgroundcolor, greenBackgroundcolor);
	control *choosePro = CreateLabel(50, 5, 458, 140, UploadPic("choosePromotion.bmp"));
	control *bishop = CreateButton(initX, y, w, h, UploadPic("bishopF.bmp"), &SetPromotionAsBishop, 'x');
	control *rook = CreateButton(initX + 150, y, w, h, UploadPic("rookF.bmp"), &SetPromotionAsRook, 'x');
	control *knight = CreateButton(initX+300, y, w, h, UploadPic("knightF.bmp"), &SetPromotionAsKnight, 'x');
	control *queen = CreateButton(initX+450, y, w, h, UploadPic("queenF.bmp"), &SetPromotionAsQueen, 'x');
	FreeUITree(s->panel_child);
	s->panel_child = NULL;
	AddPanelToWindow(s, pawnPromotionPanel);
	AddLabelToPanel(pawnPromotionPanel, choosePro, s);
	AddButtonToPanel(pawnPromotionPanel, bishop, s);
	AddButtonToPanel(pawnPromotionPanel, rook, s);
	AddButtonToPanel(pawnPromotionPanel, knight, s);
	AddButtonToPanel(pawnPromotionPanel, queen, s);
	if (SDL_Flip(s->self) != 0){
		printf(sdlBufferError, SDL_GetError());
		exit(0);
	}
	while (quit == 0){
		SDL_Event e;
		while (SDL_PollEvent(&e) == 1 && quit == 0) {
			switch (e.type){
				case (SDL_QUIT) :
					free(move);
					FreeBoardsAndTree(s);
					SDL_FreeSurface(s->self);
					free(s);
					quit = 1;
					break;
				case (SDL_MOUSEBUTTONUP) :
					HandlePress(s, pawnPromotionPanel, e);
					break;
				default:
					break;
			}
		}
	}
	return 0;
}

int SetPromotionAsBishop(window* s){
	move->type = 1;
	chooseBoard = 3;
	CreateGameWindow(s);
	return 0;
}

int SetPromotionAsKnight(window* s){
	move->type = 3;
	chooseBoard = 3;
	CreateGameWindow(s);
	return 0;
}

int SetPromotionAsRook(window* s){
	move->type = 2;
	chooseBoard = 3;
	CreateGameWindow(s);
	return 0;
}

int SetPromotionAsQueen(window* s){
	move->type = 0;
	chooseBoard = 3;
	CreateGameWindow(s);
	return 0;
}

void HandlePress(window *s, control *panel_node, SDL_Event e){
	control * child = panel_node->Panel->children;
	control * child_of_panel;
	int con = 1;
	while (child != NULL){
		if (child->Panel != NULL){
			child_of_panel = child->Panel->children;
			while (child_of_panel != NULL){
				if (child_of_panel->Button != NULL){
					con = ButtonPress(s, child_of_panel, e);
					if (con == 0)
						return;
				}
				child_of_panel = child_of_panel->next_bro;
			}
		}
		else if (child->Button != NULL){
			con = ButtonPress(s, child, e);
			if (con == 0)
				return;
		}
		child = child->next_bro;
	}
}

int ButtonPress(window *s, control *button_node, SDL_Event e){
	if ((e.button.x > button_node->rect.x) && (e.button.x < button_node->rect.x + button_node->rect.w) && (e.button.y > button_node->rect.y) && (e.button.y < button_node->rect.y + button_node->rect.h)) {
		button_node->Button->f(s);
		if (quit != 1){
			if (isThereWin == 0 && button_node->Button->game_control != 'x' && pressNumber == 0){
				firstPressX = button_node->rect.x;
				firstPressY = button_node->rect.y;
			}
			if (isThereWin == 0 && button_node->Button->game_control != 'x' && pressNumber == 1){
				secondPressX = button_node->rect.x;
				secondPressY = button_node->rect.y;
			}
		}
		return 0;
	}
	return 1;
}

void FreeBoardsAndTree(window *s){
	if (storedBoard != NULL){
		freeBoard(storedBoard);
		storedBoard = NULL;
	}
	if (lastBoard != NULL){
		freeBoard(lastBoard);
		lastBoard = NULL;
	}
	FreeUITree(s->panel_child);
	s->panel_child = NULL;
}

int QuitGame(window *s){
	FreeBoardsAndTree(s);
	SDL_FreeSurface(s->self);
	free(s);
	quit = 1;
	return 0;
}

