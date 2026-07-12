#ifndef _DRAW_H_
#define _DRAW_H_

#include "gamefield.h"
#include "global.h"

void draw_score();

void draw_gamefield(GameField * gf);

void draw_brush(Brush * br);

void draw_button(SDL_Rect * box, TextureSetLabels16 label);

void draw_title_tetrix();

void draw_top_scores();

void draw_next_figure(Brush * br);

#endif

