#ifndef GAMEFLOW_H_
#define GAMEFLOW_H_

#include "Minimax.h"

#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define ENTER_SETTINGS "Enter game settings:\n"
#define WRONG_GAME_MODE "Wrong game mode\n"
#define WRONG_MINIMAX_DEPTH "Wrong value for minimax depth. The value should be between 1 to 4\n"
#define WRONG_BOARD_INITIALIZATION "Wrong board initialization\n"
#define ILLEGAL_MOVE "Illegal move\n"
#define WRONG_POSITION "Invalid position on the board\n"
#define NO_PIECE "The specified position does not contain your piece\n"
#define PIECE_CREATES_INVALID_BOARD "Setting this piece creates an invalid board\n"
#define WRONG_FILE_NAME "Wrong file name\n"
#define MATE_WHITE "Mate! White player wins the game\n"
#define MATE_BLACK "Mate! Black player wins the game\n"
#define CHECK "Check!\n"
#define TIE "The game ends in a tie\n"
#define WRONG_ROOK_POSITION "Wrong position for a rook\n"
#define ILLEGAL_CASTLING_MOVE "Illegal castling move\n"
#define TWO_PLAYERS_GAME_MODE "Running game in 2 players mode\n"
#define PLAYER_VS_AI_GAME_MODE "Running game in player vs. AI mode\n"

#define perror_message(func_name) (perror("Error: standard function %s has failed", func_name));
#define print_message(message) (printf("%s", message));

//typedef int myState;
#define SETTINGS 0
#define GAME 1


void handleConsole();
void settingsMode(char**, int*, int*, int*, int*, int*);
void gameMode(char**, int*, int*, int*, int*, int*);
void clear(char**);
int strToInt(char*);
void handleUsersTurn(char**, int*, int*, int*, int*, int*);
void saveGame(char**, char[51], int, int, int, int);
void getMovesForUser(char**, int, char[51]);
int isUsersColorPiece(char, int);
moveElement* convertStrToMove(char[51]);
moveElement* convertStrToCastleMove(char[51]);
void executeMove(char**, moveElement, int*);
int isBoardEmpty(char**);
int setMoveType(char*);
location convertPositionStrToLocation(char[6]);
void handleComputersTurn(char**, int, int);
void handleEndOfGameCases(char**, int);
void setSquare(char**, location, char[6], char[6]);
int countPiece(char**,char);
void setNextPlayer(char[6], int*);
void setGameMode(int, int*);
void setDifficulty(int*, int);
void setUsersColor(char[6], int*);
void loadFile(char[51], char**, int*, int*, int*, int*);
void handlePrintFailure(int);
void handleScanfFailure(int);
void cleanSquare(char**, location);
void print_board(char**);
void print_line();


#endif
