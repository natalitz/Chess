#ifndef MINIMAX_H_
#define MINIMAX_H_

#include "ChessLogic.h"

void createDepthTree(char**, Node *, int, int, int, int);
linkedListElement* getBestMovesFromRoot(Node*);
int getScore (char **, int, moveElement*, int, int);
void printBestMovesForUser(char**, int, int);//, int);
int getMaxScore (linkedListElement*,char**, int, int, int);
void  deleteLowScoreMoves (linkedListElement**, char**, int, int, int, int);
void freeTree(Node *);
int calcScore(char**, int);
int scoreFunc (char** board, int playersColor);
moveElement calcNextMove(char**, int, int, int);
void AddBrother(Node *);

#endif
