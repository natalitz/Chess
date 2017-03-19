#include "GuiControls.h"

window* CreateWindow(){
	window *s;
	SDL_WM_SetCaption("Chess", "Chess");
	SDL_Surface *surface = NULL;
	surface = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (surface == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		exit(0);
	}
	s = (window*)calloc(1, sizeof(window));
	if (s == NULL){
		printf("%s", CALLOC_FAILED);
		exit(0);
	}
	s->self = surface;
	s->panel_child = NULL;
	return s;
}

control* CreateButton(int x, int y, int w, int h, SDL_Surface *pic, int(*f)(window*), char game_control){
	SDL_Rect rect;
	control *node = (control*)calloc(1, sizeof(control));
	if (node == NULL){
		printf("%s", CALLOC_FAILED);
		exit(0);
	}
	Button *b = (Button*)calloc(1, sizeof(Button));
	if (b == NULL){
		printf("%s", CALLOC_FAILED);
		exit(0);
	}
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	b->pic = pic;
	b->f = f;
	node->Button = b;
	node->rect = rect;
	node->Panel = NULL;
	node->Label = NULL;
	node->next_bro = NULL;
	return node;
}

control* CreateLabel(int x, int y, int w, int h, SDL_Surface *pic){
	SDL_Rect rect;
	control *node = (control*)calloc(1, sizeof(control));
	if (node == NULL){
		printf("%s", CALLOC_FAILED);
		exit(0);
	}
	Label *l = (Label*)calloc(1, sizeof(Label));
	if (l == NULL){
		printf("%s", CALLOC_FAILED);
		exit(0);
	}
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	l->pic = pic;
	node->Label = l;
	node->rect = rect;
	node->Panel = NULL;
	node->Button = NULL;
	node->next_bro = NULL;
	return node;
}

control* CreatePanel(int x, int y, int w, int h, int r, int g, int b){
	control *node = (control*)calloc(1, sizeof(control));
	if (node == NULL){
		printf("%s", CALLOC_FAILED);
		exit(0);
	}
	Panel *p = (Panel*)calloc(1, sizeof(Panel));
	if (p == NULL){
		printf("%s", CALLOC_FAILED);
		exit(0);
	}
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	p->r = r;
	p->g = g;
	p->b = b;
	p->children = NULL;
	node->Panel = p;
	node->rect = rect;
	node->Button = NULL;
	node->Label = NULL;
	node->next_bro = NULL;
	return node;
}

void AddPanelToWindow(window *parent, control *child){
	parent->panel_child = child;
	if (SDL_FillRect(parent->self, &(child->rect), SDL_MapRGB(parent->self->format, child->Panel->r, child->Panel->g, child->Panel->b))!= 0){
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
		exit(0);
	}
}

void AddButtonToPanel(control *parent, control *child, window *s){
	control *last_child;
	last_child = GetlastChild(parent);
	if (last_child == NULL)
		parent->Panel->children = child;
	else
		last_child->next_bro = child;
	child->rect.x += parent->rect.x;
	child->rect.y += parent->rect.y;
	if (SDL_BlitSurface(child->Button->pic, NULL, s->self, &(child->rect))!= 0) {
		SDL_FreeSurface(child->Button->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		exit(0);
	}
}

void AddPanelToPanel(control *parent, control *child, window *s){
	control *last_child;
	last_child = GetlastChild(parent);
	if (last_child == NULL)
		parent->Panel->children = child;
	else
		last_child->next_bro = child;
	child->rect.x += parent->rect.x;
	child->rect.y += parent->rect.y;
	if (SDL_FillRect(s->self, &(child->rect), SDL_MapRGB(s->self->format, child->Panel->r, child->Panel->g, child->Panel->b))!= 0){
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
		exit(0);
	}
}

void AddLabelToPanel(control *parent, control *child, window *s){
	control *last_child;
	last_child = GetlastChild(parent);
	if (last_child == NULL)
		parent->Panel->children = child;
	else
		last_child->next_bro = child;
	child->rect.x += parent->rect.x;
	child->rect.y += parent->rect.y;
	if (SDL_BlitSurface(child->Label->pic, NULL, s->self, &(child->rect))!= 0) {
		SDL_FreeSurface(child->Label->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		exit(0);
	}
}

control* GetlastChild(control* parent){
	control *child;
	child = parent->Panel->children;
	if (child == NULL)
		return NULL;
	while (child->next_bro != NULL)
		child = child->next_bro;
	return child;
}

int NullFunc(window *s){
	return 0;
}

void FreeUITree(control *control){
	if (control == NULL)
		return;
	if (control->next_bro != NULL)
		FreeUITree(control->next_bro);
	if (control->Button != NULL){
		SDL_FreeSurface(control->Button->pic);
		free(control->Button);
		free(control);
	}
	else if (control->Label != NULL){
		SDL_FreeSurface(control->Label->pic);
		free(control->Label);
		free(control);
	}
	else if (control->Panel != NULL){
		FreeUITree(control->Panel->children);
		free(control->Panel);
		free(control);
	}
}

void Createm(window *s, control *panel_node, int i, int j){
	control *m;
	if ((i + j) % 2 == 0)
		m = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("m_w_b.bmp"), &NullFunc, WHITE_P);
	else
		m= CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("m_w_w.bmp"), &NullFunc, WHITE_P);
	AddButtonToPanel(panel_node, m, s);
}

void CreateM(window *s, control *panel_node, int i, int j){
	control *M;
	if ((i + j) % 2 == 0)
		M = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("m_b_b.bmp"), &NullFunc, BLACK_P);
	else
		M = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("m_b_w.bmp"), &NullFunc, BLACK_P);
	AddButtonToPanel(panel_node, M, s);
}

void Createb(window *s, control *panel_node, int i, int j){
	control *b;
	if ((i + j) % 2 == 0)
		b = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("b_w_b.bmp"), &NullFunc, WHITE_B);
	else
		b = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("b_w_w.bmp"), &NullFunc, WHITE_B);
	AddButtonToPanel(panel_node, b, s);
}

void CreateB(window *s, control *panel_node, int i, int j){
	control *B;
	if ((i + j) % 2 == 0)
		B = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("b_b_b.bmp"), &NullFunc, BLACK_B);
	else
		B = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("b_b_w.bmp"), &NullFunc, BLACK_B);
	AddButtonToPanel(panel_node, B, s);
}

void Creater(window *s, control *panel_node, int i, int j){
	control *r;
	if ((i + j) % 2 == 0)
		r = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("r_w_b.bmp"), &NullFunc, WHITE_R);
	else
		r = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("r_w_w.bmp"), &NullFunc, WHITE_R);
	AddButtonToPanel(panel_node, r, s);
}

void CreateR(window *s, control *panel_node, int i, int j){
	control *R;
	if ((i + j) % 2 == 0)
		R = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("r_b_b.bmp"), &NullFunc, BLACK_R);
	else
		R = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("r_b_w.bmp"), &NullFunc, BLACK_R);
	AddButtonToPanel(panel_node, R, s);
}

void Createn(window *s, control *panel_node, int i, int j){
	control *n;
	if ((i + j) % 2 == 0)
		n = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("n_w_b.bmp"), &NullFunc, WHITE_N);
	else
		n = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("n_w_w.bmp"), &NullFunc, WHITE_N);
	AddButtonToPanel(panel_node, n, s);
}

void CreateN(window *s, control *panel_node, int i, int j){
	control *N;
	if ((i + j) % 2 == 0)
		N = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("n_b_b.bmp"), &NullFunc, BLACK_N);
	else
		N = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("n_b_w.bmp"), &NullFunc, BLACK_N);
	AddButtonToPanel(panel_node, N, s);
}

void Createq(window *s, control *panel_node, int i, int j){
	control *q;
	if ((i + j) % 2 == 0)
		q = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("q_w_b.bmp"), &NullFunc, WHITE_Q);
	else
		q = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("q_w_w.bmp"), &NullFunc, WHITE_Q);
	AddButtonToPanel(panel_node, q, s);
}

void CreateQ(window *s, control *panel_node, int i, int j){
	control *Q;
	if ((i + j) % 2 == 0)
		Q = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("q_b_b.bmp"), &NullFunc, BLACK_Q);
	else
		Q = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("q_b_w.bmp"), &NullFunc, BLACK_Q);
	AddButtonToPanel(panel_node, Q, s);
}

void Createk(window *s, control *panel_node, int i, int j){
	control *k;
	if ((i + j) % 2 == 0)
		k = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("k_w_b.bmp"), &NullFunc, WHITE_K);
	else
		k = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("k_w_w.bmp"), &NullFunc, WHITE_K);
	AddButtonToPanel(panel_node, k, s);
}

void CreateK(window *s, control *panel_node, int i, int j){
	control *K;
	if ((i + j) % 2 == 0)
		K = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("k_b_b.bmp"), &NullFunc, BLACK_K);
	else
		K = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("k_b_w.bmp"), &NullFunc, BLACK_K);
	AddButtonToPanel(panel_node, K, s);
}

void CreateBlank(window *s, control *panel_node, int i, int j){
	control *blank;
	if ((i + j) % 2 == 0)
		blank = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("blank_b.bmp"), &NullFunc, EMPTY);
	else
		blank = CreateButton(80 * i, 560 - 80 * j, 80, 80, UploadPic("blank_w.bmp"), &NullFunc, EMPTY);
	AddButtonToPanel(panel_node, blank, s);
}

SDL_Surface* UploadPic(char *path){
	SDL_Surface *pic = SDL_LoadBMP(path);
	SDL_Surface *new_pic = SDL_DisplayFormat(pic);
	if (new_pic==NULL){
		printf("ERROR: failed to convert image to new format: %s\n", SDL_GetError());
		exit(0);
	}
	SDL_FreeSurface(pic);
	return new_pic;
}
