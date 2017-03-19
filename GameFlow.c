#include "GameFlow.h"


/*
 * main function of console mode
 */
void handleConsole(){
	int i;
	char** board = NULL;
	int playingMode = 1; // 1- two players mode, 2- player vs. computer
	int difficulty = 1;
	int state = SETTINGS;
	int usersColor = WHITE;
	int nextTurn = WHITE;
	board = (char**)calloc(BOARD_SIZE, sizeof(char *));
	if (board == NULL){
		printf("%s", CALLOC_FAILED);
		exit(0);
	}
	for (i = 0; i < BOARD_SIZE; i++) {
		board[i] = (char*)calloc(BOARD_SIZE, sizeof(char));
		if (board[i] == NULL){
			printf("%s", CALLOC_FAILED);
			exit(0);
		}
	}
	initFullBoard(board);
	print_board(board);

	while (1){
		castle00 = 0;
		castle70 = 0;
		castle07 = 0;
		castle77 = 0;
		while (state == SETTINGS){
			printf("%s", ENTER_SETTINGS);
			settingsMode(board, &usersColor, &nextTurn, &difficulty, &state, &playingMode);
		}
		while (state == GAME){
			gameMode(board, &usersColor, &nextTurn, &difficulty, &state, &playingMode);
		}
	}
}

/*
 * manages all the settings commands
 */
void settingsMode(char** board, int* usersColor, int* nextTurn, int* difficulty, int* state, int* playingMode) {
	char command[51];
	int i, printfOutput, scanfOutput;
	char cmd[6], cmd2[6];
	location * loc;

	scanfOutput=scanf("%s", command);
	handleScanfFailure(scanfOutput);
	if (strncmp(command, "game_mode", 9) == 0){
		scanfOutput=scanf("%d", &i);
		handleScanfFailure(scanfOutput);
		setGameMode(i, playingMode);
		return;
	}
	else if (strncmp(command, "difficulty", 10) == 0){
		if (*playingMode == 1){
			printfOutput=printf("%s", ILLEGAL_COMMAND);
			handlePrintFailure(printfOutput);
			while (getchar() != '\n'){
			}
		}
		else{
			scanfOutput=scanf("%s", command);
			handleScanfFailure(scanfOutput);
			if (strncmp(command, "best", 4) == 0){
				*difficulty = -1;
			}
			if (strncmp(command, "depth", 5) == 0){
				scanfOutput=scanf("%d", &i);
				handleScanfFailure(scanfOutput);
				setDifficulty(difficulty, i);
			}
		}
		return;
	}
	else if (strncmp(command, "user_color", 10) == 0){
		if (*playingMode == 1){
			printfOutput=printf("%s", ILLEGAL_COMMAND);
			handlePrintFailure(printfOutput);
			while (getchar() != '\n'){
			}
		}
		else{
			scanfOutput=scanf("%s", command);
			handleScanfFailure(scanfOutput);
			setUsersColor(command, usersColor);
		}
		return;
	}
	else if (strncmp(command, "load", 4) == 0){
		scanfOutput=scanf("%s", command);
		handleScanfFailure(scanfOutput);
		loadFile(command, board, playingMode, nextTurn, difficulty, usersColor);
		return;
	}

	else if (strncmp(command, "clear", 5) == 0){
		clear(board);
	}

	else if (strncmp(command, "next_player", 11) == 0){
		scanfOutput=scanf("%s", cmd);	//cmd holds black or white
		handleScanfFailure(scanfOutput);
		setNextPlayer(cmd, nextTurn);
	}

	else if (strncmp(command, "rm", 2) == 0){
		scanfOutput=scanf("%s", cmd);	//cmd holds "<x,y>" (the location)
		handleScanfFailure(scanfOutput);
		loc = (location *)calloc(1, sizeof(location));
		if (loc == NULL){
			printf ("%s", CALLOC_FAILED);
			exit (0);
		}
		*loc = convertPositionStrToLocation(cmd);
		cleanSquare(board, *loc);
		free(loc);
	}
	else if (strncmp(command, "set", 3) == 0){
		scanfOutput=scanf("%s", cmd);		// <x,y>
		handleScanfFailure(scanfOutput);
		loc = (location *)calloc(1, sizeof(location));
		if (loc == NULL){
			printf ("%s", CALLOC_FAILED);
			exit (0);
		}
		*loc = convertPositionStrToLocation(cmd);
		scanfOutput=scanf("%s", cmd);		// color
		handleScanfFailure(scanfOutput);
		scanfOutput=scanf("%s", cmd2);		//type
		handleScanfFailure(scanfOutput);
		setSquare(board, *loc, cmd, cmd2);
		free(loc);
	}
	else if (strncmp(command, "print", 5) == 0){
		print_board(board);
		while (getchar() != '\n'){
		}
		return;
	}
	else if (strncmp(command, "quit", 4) == 0){
		freeBoard(board);
		exit(0);
	}
	else if (strncmp(command, "start", 5) == 0){
		if (countPiece(board, WHITE_K) == 0 || countPiece(board, BLACK_K) == 0 || isBoardEmpty(board)){
			printfOutput=printf("%s", WRONG_BOARD_INITIALIZATION);
			handlePrintFailure(printfOutput);
			return;
		}
		handleEndOfGameCases(board, *nextTurn);	//seek for tie, check or mate
		*state = GAME;
		fgets(command, 51, stdin);
		return;
	}
	else {
		printf("%s",ILLEGAL_COMMAND);
		while (getchar() != '\n') {
		}
	}
}

/*
 * handles the game. called after "start" command
 */
void gameMode(char** board, int* usersColor, int* nextTurn, int* difficulty, int* state, int* playingMode){
	if (*playingMode == 2){ // player vs. computer
		if (*nextTurn != *usersColor){		//computer's turn
			handleComputersTurn(board, *difficulty, *nextTurn);
			*nextTurn = *usersColor;
			return;
		}
		if (*nextTurn == *usersColor){		//user's turn
			handleUsersTurn(board, usersColor, nextTurn, playingMode, difficulty, state);
			return;
		}
	}
	if (*playingMode == 1){ // 2 players mode
		handleUsersTurn(board, usersColor, nextTurn, playingMode, difficulty, state);
		return;
	}
}

/*
 *	manages user's turn
 */
void handleUsersTurn(char** board, int* usersColor, int* currentTurn, int* playingMode, int* difficulty, int* state){
	char cmd[51];
	int depth, score, printfOutput;
	char fileName[51];
	moveElement* move;
	if (*currentTurn == WHITE)
		printf("White player - enter your move:\n");
	else
		printf("Black player - enter your move:\n");
	fgets(cmd, 51, stdin);
	if (strncmp(cmd, "move", 4) == 0){
		move = convertStrToMove(cmd);
		executeMove(board, *move, currentTurn);
		free(move);
		return;
	}

	else if (strncmp(cmd, "get_moves", 9) == 0){
		getMovesForUser(board, *currentTurn, cmd);
		return;
	}

	else if (strncmp (cmd, "get_best_moves", 14) == 0){
		if (cmd[15] == 'b'){
			depth = -1;
		}
		else
			depth = strToInt(cmd+15);
		printBestMovesForUser(board, depth, *currentTurn);
		return;

	}
	else if (strncmp(cmd, "get_score", 9) == 0){
		if (cmd[10] == 'b'){
			depth = -1;
			move = convertStrToMove(cmd+15);
			if (isValidSquare(move->from.column, move->from.row) == 0 || isValidSquare(move->to.column, move->to.row) == 0){
				printfOutput=printf ("%s", WRONG_POSITION);
				handlePrintFailure(printfOutput);
				free (move);
				return;
			}
			score = getScore(board, depth, move, *currentTurn, 1);
			printfOutput=printf("%d\n", score);
			handlePrintFailure(printfOutput);
			return;
		}
		else{
			depth = strToInt(cmd+10);
			if (depth == 0){
				printfOutput= printf ("%s", WRONG_MINIMAX_DEPTH);
				handlePrintFailure(printfOutput);
				return;
			}
			move = convertStrToMove(cmd+12);
			if (isValidSquare(move->from.column, move->from.row) == 0 || isValidSquare(move->to.column, move->to.row) == 0){
				printfOutput=printf ("%s", WRONG_POSITION);
				handlePrintFailure(printfOutput);
				free (move);
				return;
			}
			score = getScore(board, depth, move, *currentTurn, 1);
			printfOutput=printf("%d\n", score);
			handlePrintFailure(printfOutput);
			free(move);
			return;
		}
	}

	else if (strncmp(cmd, "save", 4) == 0){
		memcpy(fileName, cmd + 5, strlen(cmd + 5) + 1);
		fileName[strlen(fileName) - 1] = '\0';
		saveGame(board, fileName, *playingMode, *difficulty, *usersColor, *currentTurn);
		return;
	}
	else if (strncmp(cmd, "quit", 4) == 0){
		freeBoard(board);
		exit(0);
		return;
	}
	else if (strncmp(cmd, "castle", 6) == 0){
		move = convertStrToCastleMove(cmd);
		executeMove(board, *move, currentTurn);
		free(move);
		return;
	}
	else {
		printfOutput=printf("%s", ILLEGAL_COMMAND);
		handlePrintFailure(printfOutput);
	}
}

/*
 * convert string 1-4 to int
 */
int strToInt(char* ch){
	if (*ch == '1')
		return 1;
	else if (*ch == '2')
		return 2;
	else if (*ch == '3')
		return 3;
	else if (*ch == '4')
		return 4;
	return 0;
}
/*
 *  saves the game to the file in "fileName"
 */
void saveGame(char** board, char fileName[51], int gameMode, int minimaxDifficulty, int usersColor, int currentTurn){
	int i, j;
	FILE *f = fopen(fileName, "w");
	if (f == NULL){
		printf("%s", WRONG_FILE_NAME);
		return;
	}
	fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(f, "<game>\n");
	if (currentTurn == WHITE)								//set next turn
		fprintf(f, "\t<next_turn>White</next_turn>\n");
	else
		fprintf(f, "\t<next_turn>Black</next_turn>\n");

	fprintf(f, "\t<game_mode>%d</game_mode>\n", gameMode);		//set game mode

	if (gameMode == 2){					//player vs. computer
		if (minimaxDifficulty > 0)					//set difficulty
			fprintf(f, "\t<difficulty>%d</difficulty>\n", minimaxDifficulty);
		else
			fprintf(f, "\t<difficulty>best</difficulty>\n");

		if (usersColor == WHITE)		//set user's color
			fprintf(f, "\t<user_color>White</user_color>\n");
		else
			fprintf(f, "\t<user_color>Black</user_color>\n");

	}
	else {
		fprintf(f, "\t<difficulty></difficulty>\n");
		fprintf(f, "\t<user_color></user_color>\n");
	}

	fprintf(f, "\t<board>\n");
	for (j = BOARD_SIZE; j > 0; j--){
		fprintf(f, "\t\t<row_%d>", j);
		for (i = 0; i < BOARD_SIZE; i++){
			if (board[i][j - 1] == EMPTY)
				fprintf(f, "_");
			else
				fprintf(f, "%c", board[i][j-1]);
		}
		fprintf(f, "</row_%d>\n", j);
	}
	fprintf(f, "\t</board>\n");
	fprintf(f, "\t<general>%d,%d,%d,%d</general>\n", castle00, castle07, castle70, castle77);
	fprintf(f, "</game>\n");
	fclose(f);
}
/*
 *  prints all possible moves for usersColor from <x,y> given in cmd
 */
void getMovesForUser(char** board, int usersColor, char cmd[51]){
	char locationStr[6];
	location location;
	char piece;
	linkedListElement *head;
	memcpy(locationStr, cmd + 10, 5);
	location = convertPositionStrToLocation(locationStr);
	if (isValidSquare(location.column, location.row) == 0){
		printf("%s", WRONG_POSITION);
		return;
	}
	piece = board[location.column][location.row];
	if (isUsersColorPiece(piece, usersColor) == 0){
		return;
	}
	head = initLinkedList();
	moveAPiece(board, 0, 1, location, head, usersColor);
	head = getListsHead(head);
	printAllMoves(board, head);
	freeList(head);
}

/*
 * checks if piece belongs to the user or the opponent
 */
int isUsersColorPiece(char piece, int usersColor){

	if (((piece < 'a' || piece > 'z') && usersColor == WHITE) || ((piece < 'A' || piece > 'Z') && usersColor == BLACK)){
		printf("%s",NO_PIECE);
		return 0;
	}
	return 1;
}

/*
 * gets move as string and translates it to move struct
 */
moveElement* convertStrToMove(char cmd[51]){
	moveElement* move;
	char strLoc[6];
	location from, to;
	char promotion[6];
	move = (moveElement *)calloc(1, sizeof(moveElement));
	if (move == NULL){
		printf("%s",CALLOC_FAILED);
		exit(0);
	}
	memcpy(strLoc, cmd + 5, 5);
	from = convertPositionStrToLocation(strLoc);
	move->from.column=from.column;
	move->from.row = from.row;
	memcpy(strLoc, cmd + 14, 5);
	to = convertPositionStrToLocation(strLoc);
	move->to.column = to.column;
	move->to.row = to.row;
	memcpy(promotion, cmd + 20, 6);
	move->type = setMoveType(promotion);
	return move;
}

/*
 * gets castle command string and translates it to move element
 */
moveElement* convertStrToCastleMove(char cmd[51]){
	moveElement* move;
	char strLoc[6];
	location from;
	move = (moveElement*)calloc(1, sizeof(moveElement));
	if (move == NULL){
		printf("%s", CALLOC_FAILED);
		exit(0);
	}
	memcpy(strLoc, cmd + 7, 5);
	from = convertPositionStrToLocation(strLoc);
	move->from.column = from.column;
	move->from.row= from.row;
	move->to.column = -100;
	move->to.row = -100;
	move->type = 0;
	return move;
}

/*
 * checks if the move is legal  (if it appears on moves list) and execute it if so
 */
void executeMove(char** board, moveElement move, int* currentTurn){
	char piece;
	linkedListElement *head, *possibleMoves;
	int isLegalMove = 0;
	if (isValidSquare(move.from.column, move.from.row) == 0){
		printf("%s", WRONG_POSITION);
		return;
	}
	/* not castling */
	if (move.to.column != -100  && isValidSquare(move.to.column, move.to.row) == 0){
		printf("%s", WRONG_POSITION);
		return;
	}
	piece = board[move.from.column][move.from.row];
	if (isUsersColorPiece(piece, *currentTurn) == 0)
		return;
	if (move.to.column == -100 && piece != 'r' && piece != 'R'){
		printf("%s", WRONG_ROOK_POSITION);
		return;
	}
	head = getMoves(board, 0, 1, *currentTurn);
	possibleMoves = head;
	/*check whether "move" appears in possible moves list */
	while (possibleMoves->data != NULL && possibleMoves->next->data != NULL){
		if (possibleMoves->data->from.column == move.from.column && possibleMoves->data->from.row == move.from.row && possibleMoves->data->to.column == move.to.column && possibleMoves->data->to.row == move.to.row && possibleMoves->data->type == move.type){
			isLegalMove = 1;
			break;
		}
		possibleMoves = possibleMoves->next;
	}
	if (possibleMoves->data != NULL && possibleMoves->next->data == NULL && isLegalMove == 0)
		if (possibleMoves->data->from.column == move.from.column && possibleMoves->data->from.row == move.from.row && possibleMoves->data->to.column == move.to.column && possibleMoves->data->to.row == move.to.row && possibleMoves->data->type == move.type)
			isLegalMove = 1;
	if (isLegalMove == 0){
		freeList(head);
		if (move.to.column == -100)
			printf("%s", ILLEGAL_CASTLING_MOVE);
		else
			printf("%s", ILLEGAL_MOVE);
		return;
	}
	if (isLegalMove){
		changeCastlingGlobals(board, move);
		actuallyExecuteMove(board, move, *currentTurn);
		freeList(head);
	}
	print_board(board);
	handleEndOfGameCases(board, *currentTurn);
	*currentTurn = flipColor(*currentTurn);
}


/*
 *  translates promotion string to int
 */
int setMoveType(char* promotion){
	if (*promotion == '\0' || *promotion == '\n' || strncmp(promotion, "queen", 5) == 0) //non promotion or a queen
		return 0;
	else if (strncmp(promotion, "bishop", 6) == 0)
		return 1;
	else if (strncmp(promotion, "rook", 4) == 0)
		return 2;
	else if (strncmp(promotion, "knight", 6) == 0)
		return 3;
	return -1; //illegal value
}

/*
 * translates the string position from command line to location
 */
location convertPositionStrToLocation(char loc[6]){
	location ret;
	if(loc[4] != '>')
		printf ("%s", WRONG_POSITION);
	ret.column = loc[1] - 'a';
	ret.row= loc[3] - '1';
	return ret;
}

/*
 * manages computer's turn
 */
void handleComputersTurn(char** board, int difficulty, int computersColor){
	moveElement nextMove;
	int isBest = 0;
	if (difficulty == -1){
		isBest = 1;
		difficulty = 4;
	}
	nextMove = calcNextMove(board, difficulty, computersColor, isBest);
	printf("Computer: move ");
	printSingleMove(board, nextMove);
	changeCastlingGlobals(board, nextMove);
	actuallyExecuteMove(board, nextMove, computersColor);
	print_board(board);
	handleEndOfGameCases(board, computersColor);
}

/*
 * handle the cases : check, mate and tie after a move has been executed , and after "start" command
 */
void handleEndOfGameCases(char** board, int color){
	if (isAWinner(color, board)){
		if (color == WHITE)
			printf("%s", MATE_WHITE);
		else
			printf("%s", MATE_BLACK);
		freeBoard(board);
		exit(0);
	}
	else if (isATie(board, color)){
		printf ("%s", TIE);
		freeBoard(board);
		exit(0);
	}
	else if (isACheck(board, color))
		printf("%s",CHECK);
}


/*
 * sets the game mode: 1= 2 players mode, 2= player vs. computer
 */
void setGameMode(int mode, int* constGameMode){
	if (mode != 1 && mode != 2){
		printf("%s", WRONG_GAME_MODE);
	}
	else{
		*constGameMode = mode;
		if (mode == 1)
			printf("%s", TWO_PLAYERS_GAME_MODE);
		else
			printf("%s", PLAYER_VS_AI_GAME_MODE);
	}
}

/*
 * sets the minimax depth
 */
void setDifficulty(int* minimaxDifficultyPtr, int minimaxDifficultyValue){
	if (minimaxDifficultyValue < 1 || minimaxDifficultyValue > 4)
		printf("%s" ,WRONG_MINIMAX_DEPTH);
	else
		*minimaxDifficultyPtr = minimaxDifficultyValue;
}

void setNextPlayer(char color[6], int* nextPlayer){
	if (color[0] == 'b'){
		*nextPlayer = BLACK;
	}
	else if (color[0] == 'w'){
		*nextPlayer = WHITE;
	}
}


int isBoardEmpty(char** board){
	int col, row;
	for (col = 0; col < BOARD_SIZE; col++){
		for (row = 0; row < BOARD_SIZE; row++){
			if(board[col][row] != EMPTY){
				return 0;
			}
		}
	}
	return 1;
}

void setSquare(char** board, location loc, char a[6], char b[6]){
	int printfOutput;
	if (isValidSquare(loc.column, loc.row) == 0){
		printfOutput=printf("%s", WRONG_POSITION);
		handlePrintFailure(printfOutput);
	}
	else{
		if (strncmp (a, "white",5) == 0 && strncmp(b,"p",1) == 0){	//white pawn
			if (countPiece(board, WHITE_P) == 8 && board[loc.column][loc.row] != WHITE_P)		//check if there are to many pieces of a pawn
				printfOutput=printf("%s", PIECE_CREATES_INVALID_BOARD);
			else
				board[loc.column][loc.row] = WHITE_P;
		}
		if (strncmp(a, "white",5) == 0 && strncmp(b,"ki",2) == 0){	//white king
			if (countPiece(board, WHITE_K) == 1 && board[loc.column][loc.row] != WHITE_K)
				printfOutput=printf("%s", PIECE_CREATES_INVALID_BOARD);
			else
				board[loc.column][loc.row] = WHITE_K;
		}
		if (strncmp (a, "white",5) == 0 && strncmp(b,"b",1) == 0){	//white bishop
			if (countPiece(board, WHITE_B) == 2 && board[loc.column][loc.row] != WHITE_B)
				printfOutput=printf("%s", PIECE_CREATES_INVALID_BOARD);
			else
				board[loc.column][loc.row] = WHITE_B;
		}
		if (strncmp(a, "white",5) == 0 && strncmp(b,"r",1) == 0){	//white rook
			if (countPiece(board, WHITE_R) == 2 && board[loc.column][loc.row] != WHITE_R)
				printfOutput=printf("%s", PIECE_CREATES_INVALID_BOARD);
			else
				board[loc.column][loc.row] = WHITE_R;
		}
		if (strncmp (a, "white",5) == 0 && strncmp(b,"kn",2) == 0){	//white knight
			if (countPiece(board, WHITE_N) == 2 && board[loc.column][loc.row] != WHITE_N)
				printfOutput=printf("%s", PIECE_CREATES_INVALID_BOARD);
			else
					board[loc.column][loc.row] = WHITE_N;
		}
		if (strncmp(a, "white",5) == 0 && strncmp(b,"q",1) == 0){	//white queen
			if (countPiece(board, WHITE_Q) == 1 && board[loc.column][loc.row] != WHITE_Q)
				printfOutput=printf("%s", PIECE_CREATES_INVALID_BOARD);
			else
				board[loc.column][loc.row] = WHITE_Q;
		}
		if (strncmp(a, "black",5) == 0 && strncmp(b,"p",1) == 0){	//black pawn
			if (countPiece(board, BLACK_P) == 8 && board[loc.column][loc.row] != BLACK_P)
				printfOutput=printf("%s", PIECE_CREATES_INVALID_BOARD);
			else
				board[loc.column][loc.row] = BLACK_P;
		}
		if (strncmp(a, "black",5) == 0  && strncmp(b,"ki",2) == 0){	//black king
			if (countPiece(board, BLACK_K) == 1 && board[loc.column][loc.row] != BLACK_K)
				printfOutput=printf("%s", PIECE_CREATES_INVALID_BOARD);
			else
				board[loc.column][loc.row] = BLACK_K;
		}
		if (strncmp(a, "black",5) == 0 && strncmp(b,"b",1) == 0){	//black bishop
			if (countPiece(board, BLACK_B) == 2 && board[loc.column][loc.row] != BLACK_B)
				printfOutput=printf("%s", PIECE_CREATES_INVALID_BOARD);
			else
				board[loc.column][loc.row] = BLACK_B;
		}
		if (strncmp(a, "black",5) == 0  && strncmp(b,"r",1) == 0){	//black rook
			if (countPiece(board, BLACK_R) == 2 && board[loc.column][loc.row] != BLACK_R)
				printfOutput=printf("%s", PIECE_CREATES_INVALID_BOARD);
			else
				board[loc.column][loc.row] = BLACK_R;
		}
		if (strncmp(a, "black",5) == 0 && strncmp(b,"kn",2) == 0){	//black knight
			if (countPiece(board, BLACK_N) == 2 && board[loc.column][loc.row] != BLACK_N)
				printfOutput=printf("%s", PIECE_CREATES_INVALID_BOARD);
			else
				board[loc.column][loc.row] = BLACK_N;
		}
		if (strncmp(a, "black",5) == 0  && strncmp(b,"q",1) == 0){	//black queen
			if (countPiece(board, BLACK_Q) == 1 && board[loc.column][loc.row] != BLACK_Q)
				printfOutput=printf("%s", PIECE_CREATES_INVALID_BOARD);
			else
				board[loc.column][loc.row] = BLACK_Q;
		}
	}
}
/*
 * gets the board and char representing piece
 * return the amount of this piece on board
 */
int countPiece(char** board,char ch){
	int col, row, res=0;
	for (col = 0; col < BOARD_SIZE; col++){
		for (row = 0; row < BOARD_SIZE; row++){
			if(board[col][row] == ch){
				res++;
			}
		}
	}
	return res;
}

/*
 *  sets the user's color
 */
void setUsersColor(char color[6], int *usersColor){
	if (color[0] == 'b'){
		*usersColor = BLACK;
	}
	else if (color[0] == 'w'){
		*usersColor = WHITE;
	}
}

/*
 * clears the board from discs
 */
void clear(char** board){
	int col, row;
	for (col = 0; col < BOARD_SIZE; col++){
		for (row = 0; row < BOARD_SIZE; row++){
			board[col][row] = EMPTY;
		}
	}
}

/*
 * gets a square and empties it
 */
void cleanSquare(char ** board, location loc){
	int printfOutput;
	if (isValidSquare(loc.column, loc.row) == 0){
		printfOutput=printf("%s", WRONG_POSITION);
		handlePrintFailure(printfOutput);
	}
	else{
		board[loc.column][loc.row] = EMPTY;
	}
}

/*
 *  sets the game to the game saved in path if exists
 */
void loadFile(char path[51], char** board, int* gameMode, int* nextTurn, int* difficulty, int* usersColor){
	int i, j;
	char str[51];
	FILE *f = fopen(path, "r");
	if (f == NULL)
		printf("%s" ,WRONG_FILE_NAME);

	else {
		for (i = 0; i < 5; i++)
			fscanf(f, "%s", str);

		if (*(str + 1) == 'n'){ // next_turn exists
			if (*(str + 11) == 'W')
				*nextTurn = WHITE;
			else
				*nextTurn = BLACK;
			fscanf(f, "%s", str);
		}
		else
			*nextTurn = WHITE;


		if (*(str + 1) == 'g'){ 		//game mode exists
			*gameMode = *(str + 11) - '0';
			fscanf(f, "%s", str);
		}
		else
			*gameMode = 1;		//default - 2 players mode

		if (*gameMode == 2) {		//if its player vs. computer mode
			if (*(str + 1) == 'd'){ 		//difficulty exists
				if (*(str + 12) == 'b')
					*difficulty = -1;
				else
					*difficulty = *(str + 12) - '0';
				fscanf(f, "%s", str);
			}
			else
				*difficulty = 1;

			if (*(str + 1) == 'u'){ 		//user_color exists
				if (*(str + 12) == 'W')
					*usersColor = WHITE;
				else
					*usersColor = BLACK;
				fscanf(f, "%s", str);
			}
			else
				*usersColor = WHITE;
		}
		else {
			fscanf(f, "%s", str);
			fscanf(f, "%s", str);
		}

		for (j = 7; j >= 0; j--){
			fscanf(f, "%s", str);
			for (i = 0; i < BOARD_SIZE; i++){
				if (*(str + 7 + i) != '_')
					board[i][j] = *(str + 7 + i);
				else
					board[i][j] = EMPTY;
			}
		}
		fscanf(f, "%s", str);
		fscanf(f, "%s", str);
		if (*(str + 1) == 'g'){		//general exists
			castle00 = *(str + 9) - '0';
			castle07 = *(str + 11) - '0';
			castle70 = *(str + 13) - '0';
			castle77 = *(str + 15) - '0';
		}
		else {
			if (board[0][0] != WHITE_R)
				castle00 = 1;
			if (board[7][0] != WHITE_R)
				castle70 = 1;
			if (board[0][7] != BLACK_R)
				castle07 = 1;
			if (board[7][7] != BLACK_R)
				castle77 = 1;
			if (board[4][0] != WHITE_K){
				castle00 = 1;
				castle70 = 1;
			}
			if (board[4][7] != BLACK_K){
				castle07 = 1;
				castle77 = 1;
			}
		}
		fclose(f);
		print_board(board);
	}
}

void handlePrintFailure(int printfOutput){
	if (printfOutput<0){
		perror("Error: standard function printf has failed\n");
		exit(0);
	}
}

void handleScanfFailure(int scanfOutput){
	if (scanfOutput<0){
		perror("Error: standard function scanf has failed\n");
		exit(0);
	}
}

void print_board(char** board){
	int i, j;
	print_line();
	for (j = BOARD_SIZE-1; j >= 0; j--){
		printf("%d", j + 1);
		for (i = 0; i < BOARD_SIZE; i++)
			printf("| %c ", board[i][j]);
		printf("|\n");
		print_line();
	}
	for (j = 0; j < BOARD_SIZE; j++)
		printf("   %c", (char)('a' + j));
	printf("  \n");
}

/*
 *  prints a single line of the board
 */
void print_line(){
	int i;
	printf(" |");
	for (i = 1; i < BOARD_SIZE * 4; i++)
		printf("-");
	printf("|\n");
}
