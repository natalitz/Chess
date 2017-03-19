#include "ChessLogic.h"


/*
 * sets a blank board
 */
void initFullBoard(char** board){
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++){
		board[i][1] = WHITE_P;
		board[i][6] = BLACK_P;
		for (j = 2; j < 6; j++)
			board[i][j] = EMPTY;
	}
	board[0][0] = WHITE_R;
	board[1][0] = WHITE_N;
	board[2][0] = WHITE_B;
	board[3][0] = WHITE_Q;
	board[4][0] = WHITE_K;
	board[5][0] = WHITE_B;
	board[6][0] = WHITE_N;
	board[7][0] = WHITE_R;
	board[0][7] = BLACK_R;
	board[1][7] = BLACK_N;
	board[2][7] = BLACK_B;
	board[3][7] = BLACK_Q;
	board[4][7] = BLACK_K;
	board[5][7] = BLACK_B;
	board[6][7] = BLACK_N;
	board[7][7] = BLACK_R;
}


/*
 * copies a board from source to a new board and returns new board
 */
char** copyBoard(char** source){
	char** newBoard;
	int i, j;
	newBoard = (char**)calloc(BOARD_SIZE, sizeof(char*));
	if (newBoard == NULL){
		printf("%s", CALLOC_FAILED);
		exit(0);
	}
	for (i = 0; i < BOARD_SIZE; i++){
		newBoard[i] = (char*)calloc(BOARD_SIZE, sizeof(char));
		if (newBoard[i] == NULL){
			printf("%s", CALLOC_FAILED);
			exit(0);
		}
	}
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			newBoard[i][j] = source[i][j];
		}
	}
	return newBoard;
}

/*
 *  returns 1 if game control in current_board[i][j] is white, 0 if black, -1 if EMPTY
 */
int getPieceColor(char** board, location loc ){
	int col, row;
	col=loc.column;
	row=loc.row;
	if (board[col][row] == WHITE_P || board[col][row] == WHITE_B || board[col][row] == WHITE_R || board[col][row] == WHITE_N || board[col][row] == WHITE_Q || board[col][row] == WHITE_K)
		return WHITE;
	else if (board[col][row] == BLACK_P || board[col][row] == BLACK_B || board[col][row] == BLACK_R || board[col][row] == BLACK_N || board[col][row] == BLACK_Q || board[col][row] == BLACK_K)
		return BLACK;
	return -1;
}

/*
 * checks if the location is valid. returns 1 if it is , 0 elsewhere
 */
int isValidSquare(int col, int row){
	if (col > 7 || row > 7 || col < 0 || row < 0)
		return 0;
	return 1;
}

/*
 * frees all the memory that was allocated for board
 */
void freeBoard(char** board){
	int i;
	for (i = 0; i<BOARD_SIZE; i++)
		free(board[i]);
	free(board);
}
/*
 * returns list of moves current color can execute
 */
linkedListElement* getMoves(char** board, int getMovesCall, int isCastle, int color){
	location loc;
	int col, row;
	linkedListElement* head;
	head = initLinkedList();
	for (col = 0; col < BOARD_SIZE; col++){
		loc.column=col;
		for (row = 0; row < BOARD_SIZE; row++){
			loc.row = row;
			if (color == getPieceColor(board, loc))
				moveAPiece(board, getMovesCall, isCastle, loc, head, color);
		}
	}
	return getListsHead(head);
}


/*
 * calls relevant piece function according to piece located in board at "from" location
 */
void moveAPiece(char ** board, int getMovesCall, int isCastle, location from, linkedListElement * head, int color){
	int row, col;
	col=from.column;
	row=from.row;

	if ((board[col][row] == WHITE_P && color == WHITE) || (board[col][row] == BLACK_P && color == BLACK))
		pawnMoves(board, getMovesCall, from, head, color);
	if ((board[col][row] == WHITE_B && color == WHITE) || (board[col][row] == BLACK_B && color == BLACK))
		bishopMoves(board, getMovesCall, from, head, color);
	if ((board[col][row] == WHITE_R && color == WHITE) || (board[col][row] == BLACK_R && color == BLACK))
		rookMoves(board, getMovesCall, from, head, color);
	if ((board[col][row] == WHITE_N && color == WHITE) || (board[col][row] == BLACK_N && color == BLACK))
		knightMoves(board, getMovesCall, from, head, color);
	if ((board[col][row] == WHITE_Q && color == WHITE) || (board[col][row] == BLACK_Q && color == BLACK)){
		rookMoves(board, getMovesCall, from, head, color);
		bishopMoves(board, getMovesCall, from, head, color);
	}
	if ((board[col][row] == WHITE_K && color == WHITE) || (board[col][row] == BLACK_K && color == BLACK))
		kingMoves(board, getMovesCall, from, head, color);
	if (isCastle == 1 && ((board[col][row] == WHITE_R && color == WHITE) || (board[col][row] == BLACK_R && color == BLACK)))
		castleMoves(board, getMovesCall, from, head, color);
}

/*
 * adds pawn moves from board at "from" location to list "head"
 */
void pawnMoves(char** board, int getMovesCall, location from, linkedListElement *head, int color){
	int row, col, offset;
	location to;
	col=from.column;
	row=from.row;
	offset = (color==WHITE ? 1 : -1);
	to.column = col;
	to.row = row + offset;

	if (isValidSquare(col, row + offset)  && board[col][row + offset] == EMPTY){	//single move forward
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
		if ((row + offset == 7 && color == WHITE) || (row + offset == 0 && color == BLACK)){	//promoting pawn
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 1);
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 2);
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 3);
		}
	}
	to.column = col - offset;
	/* capturing forward & left */
	if (isValidSquare(col - offset, row + offset) && getPieceColor(board, to) == flipColor(color)){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
		if ((row + offset == 7 && color == WHITE) || (row + offset == 0 && color == BLACK)){
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 1);
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 2);
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 3);
		}
	}
	to.column= col + offset;
	/* capturing forward & right */
	if (isValidSquare(col + offset , row + offset)  && getPieceColor(board, to) == flipColor(color)){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
		if ((row + offset == 7 && color == WHITE) || (row + offset == 0 && color == BLACK)){
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 1);
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 2);
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 3);
		}
	}
}
/*
 *adds rook moves from board at "from" location to list "head"
 */
void rookMoves(char** board, int getMovesCall, location from, linkedListElement *head, int color){
	int row, col;
	col = from.column;
	row = from.row;
	location to;
	to.column = col;
	to.row = row;
	int offset = 1;
	/*going right on row */
	while (isValidSquare(col, row + offset)) {
		to.row = row + offset;
		if (getPieceColor(board, to) == flipColor(color)){
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
			break;
		}
		if (board[col][row + offset] == EMPTY){
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
			offset++;
		}
		else
			break;
	}

	offset = 1;
	/*going left on row*/
	while (isValidSquare(col, row - offset)) {
		to.row = row - offset;
		if (getPieceColor(board, to) == flipColor(color)){
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
			break;
		}
		if (board[col][row - offset] == EMPTY){
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
			offset++;
		}
		else
			break;
	}

	offset = 1;
	to.row=row;
	/*going up on column*/
	while (isValidSquare(col + offset, row)) {
		to.column= col + offset;
		if (getPieceColor(board, to) == flipColor(color)){
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
			break;
		}
		if (board[col + offset][row] == EMPTY){
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
			offset++;
		}
		else
			break;
	}

	offset = 1;
	/*going down on column*/
	while (isValidSquare(col - offset, row)) {
		to.column = col - offset;
		if (getPieceColor(board, to) == flipColor(color)){
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
			break;
		}
		if (board[col - offset][row] == EMPTY){
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
			offset++;
		}
		else
			break;
	}
}


/*
 *adds king moves from board at "from" location to list "head"
 */
void kingMoves(char** board, int getMovesCall, location from, linkedListElement *head, int color){
	int row, col;
	col=from.column;
	row=from.row;
	location to;

	to.column=col;
	to.row=row + 1;
	if (isValidSquare(col, row + 1) && (board[col][row + 1] == EMPTY || getPieceColor(board, to) == flipColor(color))){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
	}
	to.row= row - 1;
	if (isValidSquare(col, row - 1) && (board[col][row - 1] == EMPTY || getPieceColor(board, to) == flipColor(color))){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
	}
	to.row = row;
	to.column = col + 1;
	if (isValidSquare(col + 1, row) && (board[col + 1][row] == EMPTY || getPieceColor(board, to) == flipColor(color))){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
	}
	to.column= col - 1;
	if (isValidSquare(col - 1, row) && (board[col - 1][row] == EMPTY || getPieceColor(board, to) == flipColor(color))){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
	}
	to.column = col + 1;
	to.row = row + 1;
	if (isValidSquare(col + 1, row + 1) && (board[col + 1][row + 1] == EMPTY || getPieceColor(board, to) == flipColor(color))){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
	}
	to.row = row - 1;
	if (isValidSquare(col + 1, row - 1) && (board[col + 1][row - 1] == EMPTY || getPieceColor(board, to) == flipColor(color))){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
	}
	to.row = row + 1;
	to.column = col - 1;
	if (isValidSquare(col - 1, row + 1) && (board[col - 1][row + 1] == EMPTY || getPieceColor(board, to) == flipColor(color))){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
	}
	to.row = row - 1;
	if (isValidSquare(col - 1, row - 1) && (board[col - 1][row - 1] == EMPTY || getPieceColor(board, to) == flipColor(color))){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
	}
}

/*
 *adds knight moves from board at "from" location to  list "head"
 */
void knightMoves(char** board, int getMovesCall, location from, linkedListElement *head, int color){
	int row, col;
	location to;
	col=from.column;
	row=from.row;

	to.column=col + 2;
	to.row=row + 1;
	/* 2 right 1 forward */
	if ((isValidSquare(col + 2, row + 1)) && ((board[col + 2][row + 1] == EMPTY) || (getPieceColor(board, to) == flipColor(color)))){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
	}
	to.row=row - 1;
	/* 2 right one down  */
	if ((isValidSquare(col + 2, row - 1)) && ((board[col + 2][row - 1] == EMPTY) || (getPieceColor(board, to) == flipColor(color)))){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
	}
	to.column=col + 1;
	to.row=row + 2;
	/* 1 right 2 forward */
	if ((isValidSquare(col + 1, row + 2)) && ((board[col + 1][row + 2] == EMPTY) || (getPieceColor(board, to) == flipColor(color)))){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
	}
	to.column=col - 1;
	/* 1 left 2 forward */
	if ((isValidSquare(col - 1, row + 2)) && ((board[col - 1][row + 2] == EMPTY) || (getPieceColor(board, to) == flipColor(color)))){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
	}
	to.column=col - 2;
	to.row=row + 1;
	/* 2 left 1 forward */
	if ((isValidSquare(col - 2, row + 1)) && ((board[col - 2][row + 1] == EMPTY) || (getPieceColor(board, to) == flipColor(color)))){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
	}
	to.row=row - 1;
	/* 2 left 1 down */
	if ((isValidSquare(col - 2, row - 1)) && ((board[col - 2][row - 1] == EMPTY) || (getPieceColor(board, to) == flipColor(color)))){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
	}
	to.column=col + 1;
	to.row= row - 2;
	/* 1 right 2 down */
	if ((isValidSquare(col + 1, row - 2)) && ((board[col + 1][row - 2] == EMPTY) || (getPieceColor(board, to) == flipColor(color)))){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
	}
	to.column= col - 1;
	/*1 left 2 down */
	if ((isValidSquare(col - 1, row - 2)) && ((board[col - 1][row - 2] == EMPTY) || (getPieceColor(board, to) == flipColor(color)))){
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
	}
}

/*
 * adds bishop moves from board at "from" location to list "head"
 */
void bishopMoves(char** board, int getMovesCall, location from, linkedListElement *head, int color){
	int row, col;
	int offset = 1;
	location to;
	col=from.column;
	row=from.row;

	/*up right*/
	while (isValidSquare(col + offset, row + offset)) {
		to.column=col + offset;
		to.row=row + offset;
		if (getPieceColor(board, to) == flipColor(color)){		//capturing
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
			break;
		}
		if (board[col + offset][row + offset] == EMPTY){		//moving
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
			offset++;
		}
		else
			break;
	}

	offset = 1;
	/*up left*/
	while (isValidSquare(col - offset, row + offset)) {
		to.column = col - offset;
		to.row = row + offset;
		if (getPieceColor(board, to) == flipColor(color)){
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
			break;
		} if (board[col - offset][row + offset] == EMPTY){
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
			offset++;
		}
		else
			break;
	}

	offset = 1;
	/*down right*/
	while (isValidSquare(col + offset, row - offset)) {
		to.column = col + offset;
		to.row = row - offset;
		if (getPieceColor(board, to) == flipColor(color)){
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
			break;
		} if (board[col + offset][row - offset] == EMPTY){
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
			offset++;
		}
		else
			break;
	}

	offset = 1;
	/*down left*/
	while (isValidSquare(col - offset, row - offset)) {
		to.column = col - offset;
		to.row = row - offset;
		if (getPieceColor(board, to) == flipColor(color)){
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
			break;
		} if (board[col - offset][row - offset] == EMPTY){
			addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
			offset++;
		}
		else
			break;
	}
}




/*
 * adds castling moves from board at "from" location to list "head"
 */
void castleMoves(char** board, int getMovesCall, location from, linkedListElement *head, int color){
	int row, col, i;
	location to;
	char** tmpBoard;
	col=from.column;
	row=from.row;
	to.row = -100;
	to.column = -100;
	if ((col != 0 && col != 7) || (row != 0 && row != 7))
		return;
	if (isACheck(board, flipColor(color)))
		return;
	if (col == 0 && row == 0){
		if (castle00 == 1)
			return;
		for (i = 1; i < 4; i++){
			if (board[i][0] != EMPTY)
				return;
		}
		tmpBoard = copyBoard(board);
		tmpBoard[4][0] = EMPTY;
		tmpBoard[3][0] = WHITE_K;
		if (isACheck(tmpBoard, flipColor(color))){
			freeBoard(tmpBoard);
			return;
		}
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
		freeBoard(tmpBoard);
	}
	if (col == 0 && row == 7){
		if (castle07 == 1)
			return;
		for (i = 1; i < 4; i++){
			if (board[i][7] != EMPTY)
				return;
		}
		tmpBoard = copyBoard(board);
		tmpBoard[4][7] = EMPTY;
		tmpBoard[3][7] = BLACK_K;
		if (isACheck(tmpBoard, flipColor(color))){
			freeBoard(tmpBoard);
			return;
		}
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
		freeBoard(tmpBoard);
	}

	if (col == 7 && row == 0){
		if (castle70 == 1)
			return;
		for (i = 5; i < 7; i++){
			if (board[i][0] != EMPTY)
				return;
		}
		tmpBoard = copyBoard(board);
		tmpBoard[4][0] = EMPTY;
		tmpBoard[5][0] = WHITE_K;
		if (isACheck(tmpBoard, flipColor(color))){
			freeBoard(tmpBoard);
			return;
		}
		addMoveToMovesList(board, color, getMovesCall, from, to, head, 0);
		freeBoard(tmpBoard);
	}

	if (col == 7 && row == 7){
		if (castle77 == 1)
			return;
		for (i = 5; i < 7; i++){
			if (board[i][7] != EMPTY)
				return;
		}
		tmpBoard = copyBoard(board);
		tmpBoard[4][7] = EMPTY;
		tmpBoard[5][7] = BLACK_K;
		if (isACheck(tmpBoard, flipColor(color))){
			freeBoard(tmpBoard);
			return;
		}
		addMoveToMovesList(board, color, getMovesCall,from, to, head, 0);
		freeBoard(tmpBoard);
	}
}
/*
 * prints all the moves on list to the screen
 */
void printAllMoves(char** board, linkedListElement *list){
	linkedListElement *tmp = list;
	while (tmp->data != NULL){ // if it isn't the last move
		printSingleMove(board, *(tmp->data));
		tmp = tmp->next;
	}
}

/*
 * gets a color (BLACK or WHITE) and returns the opposite color
 */
int flipColor (int color){
	if (color == WHITE)
		return BLACK;
	return WHITE;
}

/*
 * prints a single move to the screen
 */
void printSingleMove(char** board, moveElement move){
	if (move.to.column == -100)
		printf("castle <%c,%d>\n", move.from.column + 'a', move.from.row + 1);
	else{
		printf("<%c,%d> to <%c,%d>", move.from.column + 'a', move.from.row + 1, move.to.column + 'a', move.to.row + 1);
		if (move.type == 1){
			printf(" bishop\n");
			return;
		}
		if (move.type == 2){
			printf(" rook\n");
			return;
		}
		if (move.type == 3){
			printf(" knight\n");
			return;
		}
		if (move.to.row == 7 && board[move.from.column][move.from.row] == WHITE_P){
			printf(" queen\n");
			return;
		}
		if (move.to.row == 0 && board[move.from.column][move.from.row] == BLACK_P){
			printf(" queen\n");
			return;
		}
		printf("\n");
	}
}


/*
 * the final func that executes move on board in console mode
 */
void actuallyExecuteMove(char** board, moveElement move, int color){
	if (move.to.column != -100){ // it isn't a castle move
		if (move.type == 0){
			board[move.to.column][move.to.row] = board[move.from.column][move.from.row];
			if (move.to.row == 7 && board[move.to.column][move.to.row] == WHITE_P)
				board[move.to.column][move.to.row] = WHITE_Q;
			if (move.to.row == 0 && board[move.to.column][move.to.row] == BLACK_P)
				board[move.to.column][move.to.row] = BLACK_Q;
		}
		else
			board[move.to.column][move.to.row] = getPieceChar(move.type, color);
		board[move.from.column][move.from.row] = EMPTY;
	}
	else{ 		//its a castle move
		board[move.from.column][move.from.row] = EMPTY;
		if (color == WHITE){
			board[4][0] = EMPTY;
			if (move.from.column == 0){
				board[3][0] = WHITE_R;
				board[2][0] = WHITE_K;
			}
			else{
				board[5][0] = WHITE_R;
				board[6][0] = WHITE_K;
			}
		}
		else{ 				//color is black
			board[4][7] = EMPTY;
			if (move.from.column == 0){
				board[3][7] = BLACK_R;
				board[2][7] = BLACK_K;
			}
			else{
				board[5][7] = BLACK_R;
				board[6][7] = BLACK_K;
			}
		}
	}
}
/*
 * the final func that executes move on board in Gui mode
 */
void actuallyExecuteMoveGui(char** board, moveElement move, int color){
	if (move.to.column == -100){
		board[move.from.column][move.from.row] = EMPTY;
		if (color == WHITE){
			board[4][0] = EMPTY;
			if (move.from.column == 0){
				board[3][0] = WHITE_R;
				board[2][0] = WHITE_K;
			}
			else{
				board[5][0] = WHITE_R;
				board[6][0] = WHITE_K;
			}
		}
		else{ // color is black
			board[4][7] = EMPTY;
			if (move.from.column == 0){
				board[3][7] = BLACK_R;
				board[2][7] = BLACK_K;
			}
			else{
				board[5][7] = BLACK_R;
				board[6][7] = BLACK_K;
			}
		}
	}
	else if (move.type == 0){
		if (isPawnPromotionNeeded(board, &move) == 0){
			board[move.to.column][move.to.row] = board[move.from.column][move.from.row];
		}
		else{
			if (color == WHITE)
				board[move.to.column][move.to.row] = WHITE_Q;
			else if (color == BLACK)
				board[move.to.column][move.to.row] = BLACK_Q;
		}
	}
	else
		board[move.to.column][move.to.row] = getPieceChar(move.type, color);

	board[move.from.column][move.from.row] = EMPTY;
}

/*
 * returns 1 is pawn promotion needed, 0 else
 */
int isPawnPromotionNeeded(char** board, moveElement * move){
	if ((move->to.row == 7 && board[move->from.column][move->from.row] == WHITE_P) || (move->to.row == 0 && board[move->from.column][move->from.row] == BLACK_P))
		return 1;
	return 0;
}
/*
 * changes castle00, castle07, castle70, castle 77 according to move
 */
void changeCastlingGlobals(char** board, moveElement move){
	char s;
	s = board[move.from.column][move.from.row];
	if (s == WHITE_K){
		castle00 = 1;
		castle70 = 1;
	}
	if (s == BLACK_K){
		castle07 = 1;
		castle77 = 1;
	}
	if (move.from.column == 0 && move.from.row == 0){
		castle00 = 1;
		if (move.to.column == -100)
			castle70 = 1;
	}
	if (move.from.column == 0 && move.from.row == 7){
		castle07 = 1;
		if (move.to.column == -100)
			castle77 = 1;
	}
	if (move.from.column == 7 && move.from.row == 0){
		castle70 = 1;
		if (move.to.column == -100)
			castle00 = 1;
	}
	if (move.from.column == 7 && move.from.row == 7){
		castle77 = 1;
		if (move.to.column == -100)
			castle07 = 1;
	}
	if (move.to.column == 0 && move.to.row == 0)
		castle00 = 1;
	if (move.to.column == 0 && move.to.row == 7)
		castle07 = 1;
	if (move.to.column == 7 && move.to.row == 0)
		castle70 = 1;
	if (move.to.column == 7 && move.to.row == 7)
		castle77 = 1;
}

/*
 * creates new empty linked list
 */
linkedListElement* initLinkedList(){
	linkedListElement* list;
	list = (linkedListElement*)calloc(1, sizeof(linkedListElement));
	if (list == NULL){
		printf("%s" , CALLOC_FAILED);
		exit(0);
	}
	list->prev = NULL;
	list->next = NULL;
	list->data = NULL;
	return list;
}

/*
 *  adds move from "local from" to "local to" with type to list "head"
 */
void addMoveToMovesList(char** board, int color, int getMovesCall, location localFrom, location localTo, linkedListElement *head, int type){
	linkedListElement *new;
	moveElement *curMove;
	int fromRow, fromCol, toRow, toCol;
	fromCol=localFrom.column;
	fromRow=localFrom.row;
	toRow=localTo.row;
	toCol=localTo.column;
	char** tmpBoard;
	curMove = (moveElement*)calloc(1, sizeof(moveElement));
	if (curMove == NULL){
		printf("%s", CALLOC_FAILED);
		exit(0);
	}
	curMove->from.column = fromCol;
	curMove->from.row = fromRow;
	curMove->to.column = toCol;
	curMove->to.row = toRow;
	curMove->type = type;
	if (getMovesCall == 0){
		tmpBoard = copyBoard(board);
		if (runningMode == CONSOLE)
			actuallyExecuteMove(tmpBoard, *curMove, color);
		else
			actuallyExecuteMoveGui(tmpBoard, *curMove, color);
		if (isACheck(tmpBoard, flipColor(color))){
			free(curMove);
			freeBoard(tmpBoard);
			return;					//move is illegal - won't be in possible moves list
		}
		freeBoard(tmpBoard);
	}
	new = (linkedListElement*)calloc(1, sizeof(linkedListElement));
	if (new == NULL){
		printf("%s", CALLOC_FAILED);
		exit(0);
	}
	if (head->data == NULL){		//first node
		head->data = curMove;		//update first node
		new->data = NULL;
		head->next = new;			//create second node
		new->prev = head;
		new->next = NULL;
	}
	else{
		new->data = curMove;
		if (curMove->to.column != -100 && board[curMove->to.column][curMove->to.row] != EMPTY){
			/* eating move - insert before head */
			new->next = head;
			new->prev = head->prev;
			if (head->prev!=NULL)
				head->prev->next = new;
			head->prev = new;
		}
		else{
			/* regular move - insert after head */
			new->prev = head;
			new->next = head->next;
			head->next->prev = new;
			head->next = new;
		}
	}
}
/*
 * returns first linked list node
 */
linkedListElement* getListsHead(linkedListElement *head){
	if (head->prev!= NULL){
		return getListsHead(head->prev);
	}
	return head;
}

/*
 *  returns letter representation of the type. returns 'Z' if type does not represent a valid type
 */
char getPieceChar(int type, int color){
	if (color == WHITE){
		if (type == 0)
			return WHITE_Q;
		else if (type == 1)
			return WHITE_B;
		else if (type == 2)
			return WHITE_R;
		else if (type == 3)
			return WHITE_N;
	}
	else {
		if (type == 0)
			return BLACK_Q;
		else if (type == 1)
			return BLACK_B;
		else if (type == 2)
			return BLACK_R;
		else if (type == 3)
			return BLACK_N;
	}
	return 'Z';
}

/*
 * returns 1 if board is in check state , 0 elsewhere
 */
int isACheck(char** board, int color){
	linkedListElement* head;
	linkedListElement* moves;
	int i, j;
	head = getMoves(board, 1, 0, color); // holds illegal moves as well
	moves = head;
	while (moves->data != NULL){
		i = moves->data->to.column;
		j = moves->data->to.row;
		if (board[i][j] == WHITE_K || board[i][j] == BLACK_K){
			freeList(head);
			return 1;
		}
		moves = moves->next;
	}
	freeList(head);
	return 0;
}

/*
 * returns 1 if "color" is in a tie state, 0 elsewhere
 */
int isATie(char** board, int color){
	linkedListElement* head;
	head = getMoves(board, 0, 0, color); //possible legal moves list for all pieces.
	//deleteIllegalMoves (board, &head, color);
	if (head->data == NULL && isACheck(board, color) == 0){	//check if linked list is empty - no possible moves and no check
		freeList(head);
		return 1;
	}
	freeList(head);
	return 0;
}

/*
 * frees all memory allocated in a list
 */
void freeList(linkedListElement* head){
	linkedListElement *tmpNode;
	while (head->next != NULL){
		tmpNode = head->next;
		free(head->data);
		free(head);
		head = tmpNode;
	}
	free(head);
} 


/*
 * checks if there is a winning situation for "color". returns 1 if so
 */
int isAWinner(int color, char** board){
	linkedListElement *head;
	head = getMoves(board, 0, 0, flipColor(color));
	if (head->data == NULL){		//opponent has no possible moves
		freeList(head);
		return 1;
	}
	freeList(head);
	return 0;
}
/*
 * deletes a single node from a doubly linked list
 */
void deleteNode(linkedListElement **head, linkedListElement *nodeToDelete)
{
  /* base case */
  if((*head)->data == NULL || nodeToDelete->data == NULL)
    return;

  /* If node to be deleted is head node */
  if(*head == nodeToDelete)
    *head = nodeToDelete->next;

  /* Change next only if node to be deleted is NOT the last node */
  if(nodeToDelete->next != NULL){
	 if(nodeToDelete->next->data != NULL){
		 nodeToDelete->next->prev = nodeToDelete->prev;
	 }
  }

  /* Change prev only if node to be deleted is NOT the first node */
  if(nodeToDelete->prev != NULL){
	  if(nodeToDelete->prev->data !=NULL){
		  nodeToDelete->prev->next = nodeToDelete->next;
	  }
  }
  /* Finally, free the memory occupied by nodeToDelete*/
  free(nodeToDelete->data);
  free(nodeToDelete);
//  return;
}

/*
 * create new node and returns a pointer to the created node
 */
Node* initNode(){
	Node* node;
	node = (Node*)calloc(1, sizeof(Node));
	if (node == NULL){
		printf("ERROR: standard function calloc has failed\n");
		exit(0);
	}
	node->nextBrother = NULL;
	node->move = NULL;
	return node;
}

/*
 * gets a list of moves and deletes illegal moves from it
 */
void deleteIllegalMoves (char** board, linkedListElement** head, int color){
	char** tmpBoard;
	linkedListElement *curNode;
	curNode = *head;
	tmpBoard = copyBoard(board);
	while (curNode->data !=NULL && curNode->next->data != NULL){
		actuallyExecuteMove(tmpBoard, *(curNode->data), color);
		if (isACheck(tmpBoard, flipColor(color))){
			deleteNode(head, curNode);		//move is illegal - won't be in possible moves list
		}
		freeBoard(tmpBoard);
		tmpBoard = copyBoard(board);
		curNode = curNode->next;
	}

	actuallyExecuteMove(tmpBoard, *(curNode->data), color);
	if (isACheck(tmpBoard, flipColor(color))){
		deleteNode(head, curNode);		//move is illegal - won't be in possible moves list
	}

	freeBoard(tmpBoard);
}


