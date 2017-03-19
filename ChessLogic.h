#ifndef CHESSLOGIC_H_
#define CHESSLOGIC_H_


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <SDL_video.h>

#define WHITE_P 'm'
#define WHITE_B 'b'
#define WHITE_N 'n'
#define WHITE_R 'r'
#define WHITE_Q 'q'
#define WHITE_K 'k'

#define BLACK_P 'M'
#define BLACK_B 'B'
#define BLACK_N 'N'
#define BLACK_R 'R'
#define BLACK_Q 'Q'
#define BLACK_K 'K'

#define EMPTY ' '
#define BOARD_SIZE 8

#define BLACK 0
#define WHITE 1

#define CALLOC_FAILED "ERROR: standard function calloc has failed\n"

int castle00, castle70, castle07, castle77; // 0- hasn't moved, 1- moved
int runningMode;

#define CONSOLE 1
#define GUI 2

typedef struct locationOnTheBoard {
	int row;
	int column;
}location;

typedef struct move {
	location from; // castling move is represented as rook's move
	location to; // [-100,-100] if castle move
	int type; // 0-null/queen, 1-bishop, 2-rook, 3-knight
}moveElement;

typedef struct Node {
	int score;
	int alpha;
	int beta;
	int numberOfChildren;
	struct Node * children;
	struct Node * parent;
	struct Node * nextBrother;
	struct Node * childOfChosenMove;
	moveElement * move;
	unsigned int currentDepth;
}Node;


typedef struct linkedlist {
	moveElement * data;
	struct linkedlist * next;
	struct linkedlist * prev;
}linkedListElement;


void initFullBoard(char**);
char** copyBoard(char**);
int getPieceColor(char**, location);
int isValidSquare(int, int);
int flipColor (int);
void freeBoard(char**);
linkedListElement* getMoves(char**, int, int, int);
void moveAPiece(char**, int, int, location, linkedListElement*, int);
void pawnMoves(char**, int, location, linkedListElement*, int);
void bishopMoves(char**, int, location, linkedListElement*, int);
void rookMoves(char**, int, location, linkedListElement*, int);
void knightMoves(char**, int, location, linkedListElement*, int);
void kingMoves(char**, int, location, linkedListElement*, int);
void castleMoves(char**, int, location, linkedListElement*, int);
void actuallyExecuteMove(char**, moveElement, int);
void actuallyExecuteMoveGui(char**, moveElement, int); // executes move_item on current_board in GUI mode
void deleteIllegalMoves (char**, linkedListElement**, int);
void deleteNode(linkedListElement **, linkedListElement*);
int isPawnPromotionNeeded(char** , moveElement*);
void changeCastlingGlobals(char** , moveElement);
linkedListElement* initLinkedList();

void addMoveToMovesList(char**, int, int, location, location, linkedListElement *, int);
linkedListElement* getListsHead(linkedListElement *);
char getPieceChar(int, int);
void freeList(linkedListElement*);
void printAllMoves(char**, linkedListElement *);
void printSingleMove(char**, moveElement);
int isATie(char**, int);
int isACheck(char**, int);
int isAWinner(int, char**);
Node* initNode();

#endif
