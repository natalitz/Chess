#ifndef GUI_H_
#define GUI_H_

#include "GuiControls.h"
#include "SDL/SDL.h"

#define SLOTS 10
extern int access(const char *path, int amode);
int CreateMainWindow(window *s);
int CreateGameModeWindow(window *s);
int OnPlayerVsComputerPress(window *s);
int OnPlayerVsPlayerPress(window *s);
int CreateAlSettingsWindow(window *s);
int RebuildSettingsMenu(window *s, control **second_settings_window, control **choose_color, control **white, control **black, control **choose_difficulty, control **one, control **two, control **three, control **four, control **best, control **start_game);
void RebuildNextPlayerPanel(window *s, control **nextPlayerPanel, control **nextPlayerLabel, control **white,control **black,
		control **cancel, control **startGame);
int SetNextTurnAsWhite(window *s);
int SetNextTurnAsBlack(window *s);
int CreateNextPlayerWindow(window *s);
int SetPlayerAsWhite(window *s);
int SetPlayerAsBlack(window *s);
int SetDiffAsOne(window *s);
int SetDiffAsTwo(window *s);
int SetDiffAsThree(window *s);
int SetDiffAsFour(window *s);
int SetDiffAsBest(window *s);
int LoadGame(window *s);
void Load(window *s, char* path);
void SetFromFileGui(char*);
int Load1(window *s);
int Load2(window *s);
int Load3(window *s);
int Load4(window *s);
int Load5(window *s);
int Load6(window *s);
int Load7(window *s);
int Load8(window *s);
int Load9(window *s);
int Load10(window *s);
int SaveGame(window *s);
void SaveGui(char* file_name);
int Save1(window *s);
int Save2(window *s);
int Save3(window *s);
int Save4(window *s);
int Save5(window *s);
int Save6(window *s);
int Save7(window *s);
int Save8(window *s);
int Save9(window *s);
int Save10(window *s);
int BackFromSave(window *s);
int OnBoard(int x, int y);
int ColorByCoordinates(char** board, int x, int y);
int CreateBoardSettingsWindow(window *s);
int CreateGameWindow(window *s);
void RebuildGameMenu(window *s, control **full_panel, control **menu_panel, control **save_game, control **main_menu, control **quit_b); // creates menu shown in game window
void RebuildSettingsBoardMenu(window *s, control **full_panel, control **menu_panel, control **label, control **main_menu
		, control **white, control **black, control **pawn ,control **bishop
		, control **rook, control **knight, control **qween, control **king, control **delete
		, control **startGame);
int addPawnToTheBoard(window *s);
int addKnightToTheBoard(window *s);
int addRookToTheBoard(window *s);
int addBishopToTheBoard(window *s);
int addQweenToTheBoard(window *s);
int addKingToTheBoard(window *s);
int countPiece(char** board,char ch);
int deletePawn(window* s);
int setAddColorAsWhite(window* s);
int setAddColorAsBlack(window* s);
void handleChooseBoard(char** current_board);
void createGame(window *s, control *panel_node, char** current_board);
int IsMovePossible(char** current_board, moveElement move_item, int turn);
int RevertIllegalMove(window *s);
int ExecutePawnPromotion(window *s);
int SetPromotionAsBishop(window* s);
int SetPromotionAsKnight(window* s);
int SetPromotionAsRook(window* s);
int SetPromotionAsQueen(window* s);
void HandlePress(window *s, control *panel_node, SDL_Event e);
int ButtonPress(window *s, control *button_node, SDL_Event e);
void FreeBoardsAndTree(window *s);
int QuitGame(window *s);

#endif
