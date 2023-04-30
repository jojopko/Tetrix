/* This file is licensed under the GPL 3. 
 * For details see: www.gnu.org/licenses/gpl-3.0.txt 
 * Author: Konstantin Serezhkin (t.me/jojopko_bot) */

#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <sys/types.h>
#include "global.h"
#include "draw.h"
#include "types.h"


#define BLOCK_SIZE 22

void set_draw_color_block(BlockColor color) {
	switch (color) {
		case BLOCK_NONE:
			SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
			break;
		case BLOCK_RED:
			SDL_SetRenderDrawColor(_renderer, 255, 51, 0, 255);
			break;
		case BLOCK_GREEN:
			SDL_SetRenderDrawColor(_renderer, 0, 153, 51, 255);
			break;
		case BLOCK_BLUE:
			SDL_SetRenderDrawColor(_renderer, 0, 51, 204, 255);
			break;
		case BLOCK_YELLOW:
			SDL_SetRenderDrawColor(_renderer, 255, 204, 102, 255);
			break;
		case BLOCK_VIOLET:
			SDL_SetRenderDrawColor(_renderer, 115, 0, 153, 255);
			break;
	}
}

void draw_gamefield(GameField * gf) {
	if (gf == nullptr) {
		return;
	}
	// Fill background
	SDL_Rect gamefield_area = {0, 61, WIN_W, WIN_H};
	SDL_SetRenderDrawColor(_renderer, 0x0f, 0x0f, 0x0f, 255);
	SDL_RenderFillRect(_renderer, &gamefield_area);
	// Draw Cells
	SDL_Point start_pos = {16, 64};
	int margin = 2;
	SDL_Rect draw_block = {start_pos.x, start_pos.y, BLOCK_SIZE, BLOCK_SIZE};
	for (int i = 0; i < gf->w * gf->h; i++) {
		set_draw_color_block(gf->field[i].color);
		SDL_RenderFillRect(_renderer, &draw_block);
		if ((i+1) % gf->w) {
			draw_block.x += draw_block.w + margin;
		}
		else {
			draw_block.x = start_pos.x;
			draw_block.y += draw_block.h + margin;
		}
	}
}

SDL_Texture * make_texture_from_text(SDL_Renderer *renderer, char *text, TTF_Font *font, u_char r, u_char g, u_char b) {
    SDL_Surface * surface;
	SDL_Texture * texture;
    SDL_Color textColor = {r, g, b, 0};
    surface = TTF_RenderText_Solid(font, text, textColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
	return texture;
}

void draw_score() {
	
}



