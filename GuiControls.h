#ifndef GUICONTROLS_H_
#define GUICONTROLS_H_

#include "Minimax.h"

#define SCREEN_H 640
#define SCREEN_W 900

struct window{
	struct control* panel_child;
	SDL_Surface * self;
};

typedef struct window window;

struct Button{
	int(*f)(window*);
	SDL_Surface *pic;
	char game_control; // x means not a game control
};

typedef struct Button Button;

struct Panel{
	int r;
	int g;
	int b;
	struct control* children;
};

typedef struct Panel Panel;

struct Label{
	SDL_Surface *pic;
};

typedef struct Label Label;

struct control{
	struct Button *Button;
	struct Panel *Panel;
	struct Label *Label;
	struct control* next_bro;
	SDL_Rect rect;
};

typedef struct control control;

window* CreateWindow(); // creates window
control* CreateButton(int x, int y, int w, int h, SDL_Surface *pic, int(*f)(window*), char game_control); // creates button
control* CreateLabel(int x, int y, int w, int h, SDL_Surface *pic); // creates label
control* CreatePanel(int x, int y, int w, int h, int r, int g, int b); // creates panel
void AddPanelToWindow(window *parent, control *child); // adds panel as child of parent (window) in UI tree
void AddButtonToPanel(control *parent, control *child, window *s); // adds button as child of parent (panel) in UI tree
void AddPanelToPanel(control *parent, control *child, window *s); // adds panel as child of parent (panel) in UI tree
void AddLabelToPanel(control *parent, control *child, window *s); // adds label as child of parent (panel) in UI tree
control* GetlastChild(control* parent); // returns last control_node child of parent
int NullFunc(window *s); // returns 0
void FreeUITree(control *control); // frees all memory allocated for tree whose control is its root, and trees whose control's siblings are their roots
void Createm(window *s, control *panel_node, int i, int j); // creates white pawn in [i][j]
void CreateM(window *s, control *panel_node, int i, int j); // creates black pawn in [i][j]
void Createb(window *s, control *panel_node, int i, int j); // creates white bishop in [i][j]
void CreateB(window *s, control *panel_node, int i, int j); // creates black bishop in [i][j]
void Creater(window *s, control *panel_node, int i, int j); // creates white rook in [i][j]
void CreateR(window *s, control *panel_node, int i, int j); // creates black rook in [i][j]
void Createn(window *s, control *panel_node, int i, int j); // creates white knight in [i][j]
void CreateN(window *s, control *panel_node, int i, int j); // creates black knight in [i][j]
void Createq(window *s, control *panel_node, int i, int j); // creates white queen in [i][j]
void CreateQ(window *s, control *panel_node, int i, int j); // creates black queen in [i][j]
void Createk(window *s, control *panel_node, int i, int j); // creates white king in [i][j]
void CreateK(window *s, control *panel_node, int i, int j); // creates black king in [i][j]
void CreateBlank(window *s, control *panel_node, int i, int j); // creates EMPTY in [i][j]
SDL_Surface* UploadPic(char *path); // returns SDL_Surface* pointing to image from path
#endif
