#include "Minimax.h"

static int evaluatedBoards = 0;


/*
 * creates minimax tree when depth 1-4 are chosen
 * if moveElem != NULL then board= the board after executing the move (for get_score) (user's use of this func)
 * else - board = current board		(computer's use of this func)
 * player can be either computer or user
 */
void createDepthTree(char** board, Node *node, int depth, int playersColor, int curColor, int isBest){
	linkedListElement *moves;
	linkedListElement *head = NULL;
	Node *lastBrother = NULL;
	int flag, done = 0;
	char** tmpBoard;
//	printf ("%d\n",evaluatedBoards);
	if (depth == 0)		//no depth, end of recursion
		done = 1;
	else{
		head = getMoves(board, 0, 1, curColor);	//get all possible moves for current state
		if (head->data == NULL)		//no possible moves
			done = 1;
	}
	if (done){
		node->score = scoreFunc(board, playersColor);
		if (node -> parent == NULL)
			return;
		if (curColor == playersColor && node->score < node->parent->beta){		//if its computer's (or current player) turn - take maximum node to be next move
			node->parent->beta = node->score;
			node->parent->childOfChosenMove = node;
		}
		else if (curColor == flipColor(playersColor) && node->score > node->parent->alpha){		//if its user's turn (or opponent) take minimum node to be next move
			node->parent->alpha = node->score;
			node->parent->childOfChosenMove = node;
		}
		if (head != NULL)
			freeList(head);
		return;
	}
	else {	//done = 0
		moves = head;
		flag = 1;
		node->children = initNode();
		while (flag == 1){
			evaluatedBoards++;
			if (isBest && evaluatedBoards == 1000000){		//maximum 10^6 if its best mode
				freeList(head);
				return;
			}
			node->numberOfChildren++;
			tmpBoard = copyBoard(board);
			actuallyExecuteMoveGui(tmpBoard, *(moves->data), curColor);
			if (lastBrother == NULL)
				lastBrother = node->children;
			else
				lastBrother = lastBrother->nextBrother;
			lastBrother->parent = node;
			lastBrother->move = (moveElement *)calloc(1, sizeof(moveElement));
			if (lastBrother->move == NULL){
				printf("%s", CALLOC_FAILED);
				exit(0);
			}
			memcpy(lastBrother->move, moves->data, sizeof(moveElement));
			lastBrother->alpha = node->alpha;
			lastBrother->beta = node->beta;
			lastBrother->numberOfChildren = 0;
			lastBrother->currentDepth = node->currentDepth + 1;
			lastBrother->children = NULL;

			AddBrother(lastBrother);		//add NULL brother after lastBrother
			createDepthTree(tmpBoard, lastBrother, depth - 1, playersColor, flipColor(curColor), isBest);	//recursion

			if (evaluatedBoards == 1000000){
				freeBoard(tmpBoard);
				freeList(head);
				return;
			}
			if (moves->next->data == NULL || node->beta <= node->alpha){
				if (node->parent != NULL){
					if (curColor != playersColor && node->beta > node->parent->alpha){
						node->parent->childOfChosenMove = node;
						node->parent->alpha = node->beta;
					}
					else if (curColor == playersColor && node->alpha < node->parent->beta){
						node->parent->childOfChosenMove = node;
						node->parent->beta = node->alpha;
					}
				}
				flag = 0; // stop adding brothers
				freeBoard(tmpBoard);
				continue;
			}
			moves = moves->next;
			freeBoard(tmpBoard);
		}
		freeList(head);
	}
}


/*
 * computes the best moves for user and prints them to the screen
 */
void printBestMovesForUser(char** board, int depth, int currentTurn){//, int isBest){
	linkedListElement* head;
	int maxScore;
	int isBest = 0;

	if (depth == -1)
		isBest = 1;

	head = getMoves(board,0,1,currentTurn);
	//	deleteIllegalMoves(board, &head, currentTurn);
	maxScore = getMaxScore (head, board, depth, currentTurn, isBest);
	deleteLowScoreMoves (&head,board, depth, maxScore, currentTurn, isBest);
	getListsHead(head);
	printAllMoves(board, head);
	freeList(head);
	evaluatedBoards = 0;

}

/*
 * passes over all root's children and seeks all the best moves
 */
linkedListElement* getBestMovesFromRoot(Node* root){
	int i;
	Node* curNode;
	linkedListElement* list;
	curNode = (Node*) calloc (1, sizeof(Node));
	if(curNode == NULL){
		printf("%s", CALLOC_FAILED);
		exit(0);
	}
	list=initLinkedList();
	curNode=root->children;
	for (i=0; i<root->numberOfChildren; i++){
		if(curNode->beta == root->alpha){
			list->data = curNode->move;
			if(i != root->numberOfChildren - 1){
				list->next = initLinkedList();
				list->next->prev = list;
				list= list->next;
			}
		}
		curNode = curNode->nextBrother;
	}

	return getListsHead(list);
}


/*
 * returns score according to board and user's color
 */
int scoreFunc (char** board, int playersColor){
	int col, row;
	int whiteScore = 0;
	int blackScore = 0;

	for (col = 0; col < BOARD_SIZE; col++){
			for (row = 0; row < BOARD_SIZE; row++){
				if (board[col][row] == WHITE_P){
					whiteScore += 1;
				}
				else if (board[col][row] == WHITE_B){
					whiteScore += 3;
				}
				else if (board[col][row] == WHITE_R){
					whiteScore += 5;
				}
				else if (board[col][row] == WHITE_N){
					whiteScore += 3;
				}
				else if (board[col][row] == WHITE_Q){
					whiteScore += 9;
				}
				else if (board[col][row] == WHITE_K){
					whiteScore += 400;
				}
				else if (board[col][row] == BLACK_P){
					blackScore += 1;
				}
				else if (board[col][row] == BLACK_B){
					blackScore += 3;
				}
				else if (board[col][row] == BLACK_R){
					blackScore += 5;
				}
				else if (board[col][row] == BLACK_N){
					blackScore += 3;
				}
				else if (board[col][row] == BLACK_Q){
					blackScore += 9;
				}
				else if (board[col][row] == BLACK_K){
					blackScore += 400;
				}
			} // end for row
		} // end for col

	if(isAWinner(playersColor, board))	//if player wins
		return 1000;
	if (isAWinner(flipColor(playersColor), board)) //if the opponent wins
		return -1000;
	if (isATie(board, playersColor))
		return -500;

	return (playersColor==WHITE) ? (whiteScore - blackScore) : (blackScore - whiteScore);

}

/*
 * return the move's score according to the depth
 */
int getScore (char ** board, int depth, moveElement* move, int currentColor, int calledFromMain){
	Node * root;
	int score;
	int isBest = 0;
	char** tmpBoard;
	tmpBoard = copyBoard(board);
	actuallyExecuteMove(tmpBoard, *move, currentColor);	//execute move. depth reduces by 1
	if (depth == 1){										//no need for minimax tree, need only the score after the move is executed
		score = scoreFunc(tmpBoard, currentColor);				//now check score
		freeBoard(tmpBoard);
		if (calledFromMain)
			evaluatedBoards = 0;
		return score;
	}
	root = (Node *)calloc(1, sizeof(Node));
	if (root == NULL){
		printf("%s", CALLOC_FAILED);
		exit(0);
	}
	if (depth == -1){
		isBest = 1;
		depth = 4;
	}
	root->move = NULL;
	root->alpha = INT_MIN;		// "-infinity"
	root->beta = INT_MAX;		// "+infinity"
	root->score = scoreFunc(tmpBoard, currentColor);
	root->numberOfChildren = 0;
	root->currentDepth = 0;
	root->nextBrother = NULL;
	root->parent = NULL;
	createDepthTree(tmpBoard, root, depth-1, currentColor, flipColor(currentColor), isBest);
	if (isAWinner(currentColor,tmpBoard))
		score = root->score;
	else
		score = root->beta;
	freeTree(root);
	freeBoard(tmpBoard);
	if (calledFromMain)
		evaluatedBoards = 0;
	return score;
}

/*
 * returns best score of all the moves on move's list m
 */
int getMaxScore (linkedListElement* head,char** board, int depth, int currentTurn, int isBest){

	int curScore;
	int maxScore = INT_MIN;
	linkedListElement* curNode;
	curNode = head;
	while (curNode->data != NULL && curNode->next->data != NULL){
		if ((curScore=getScore(board, depth ,curNode->data, currentTurn, 0)) > maxScore){
			maxScore = curScore;
		}
		curNode = curNode -> next;
	}
	if ((curScore=getScore(board, depth ,curNode->data, currentTurn, 0)) > maxScore){
		maxScore = curScore;
	}
	return maxScore;
}

/*
 * gets moves list and deletes the all low-score moves (lower than maxScore)
 */
void  deleteLowScoreMoves (linkedListElement** head, char** board, int depth, int maxScore, int currentTurn, int isBest){
	linkedListElement *curNode;
	curNode = *head;
	while (curNode->data != NULL && curNode->next->data != NULL){
		if ((getScore(board,depth ,curNode->data, currentTurn, 0)) < maxScore){
			deleteNode(head, curNode);
		}
		curNode = curNode->next;
	}
	if ((getScore(board,depth ,curNode->data , currentTurn, 0)) < maxScore){
		deleteNode(head, curNode);
	}

}

/*
 * computes and returns next move for computer (best move for it)
 */
moveElement calcNextMove(char** board, int depth, int computersColor, int isBest) {
	Node *root;
	char** tmpBoard;
	moveElement nextMove;
	tmpBoard = copyBoard(board);

	root = (Node *)calloc(1, sizeof(Node));
	if (root == NULL){
		printf("%s",CALLOC_FAILED);
		exit(0);
	}
	root->move = NULL;
	root->alpha = INT_MIN;		// "-infinity"
	root->beta = INT_MAX;		// "+infinity"
	root->score = scoreFunc(board, computersColor);
	root->numberOfChildren = 0;
	root->currentDepth = 0;
	root->nextBrother = NULL;
	root->parent = NULL;
	createDepthTree(tmpBoard, root, depth, computersColor, computersColor, isBest);
	nextMove = *(root->childOfChosenMove->move);
	freeTree(root);
	freeBoard(tmpBoard);
	evaluatedBoards = 0;
	return nextMove;
}

/*
 * adds new NULL sibling to last brother
 */
void AddBrother(Node *lastBrother){
	Node *newEnd;
	newEnd = (struct Node*)calloc(1, sizeof(struct Node));
	if (newEnd == NULL){
		printf("%s", CALLOC_FAILED);
		exit(0);
	}
	newEnd->nextBrother = NULL;
	newEnd->move = NULL;
	lastBrother->nextBrother = newEnd;
}

/*
 *  frees all the memory that was allocated for this tree
 */
void freeTree(Node *root){
	Node *tmp;
	Node *tmp2;
	if (root->numberOfChildren == 0){
		if (root->move != NULL)
			free(root->move);
		if (root->children != NULL)
			free(root->children);
		free(root);
		return;
	}
	tmp = root->children;
	while (tmp->move != NULL){
		tmp2 = tmp->nextBrother;
		freeTree(tmp);
		tmp = tmp2;
	}
	free(tmp);
	if (root->move != NULL)
		free(root->move);
	free(root);
}
