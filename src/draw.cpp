/* This file is licensed under the GPL 3. 
 * For details see: www.gnu.org/licenses/gpl-3.0.txt 
 * Author: Konstantin Serezhkin (t.me/jojopko_bot) */

#include <SDL_ttf.h>
#include <stdio.h>
#include <sys/types.h>
#include "game.h"
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
        printf("Gamefield is 'null'\n");
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

void draw_score() {
	int score = get_score();
    int max_len = 6;
    int width_num, height_num;
    SDL_Texture * score_title = get_ttf_texture("hello\0"); // TODO: one time initialization
    SDL_Rect rect = {0, 0};
    SDL_QueryTexture(score_title, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(_renderer, score_title, NULL, &rect); // Print score lable
    rect.x = rect.w + 5;
    SDL_QueryTexture(_numbers_f16[0], NULL, NULL, &width_num, &height_num);
    rect.w = width_num;
    rect.h = height_num;
    rect.x += rect.w * max_len;
    for (int i = 0; i < max_len; i++) {
        SDL_Texture * num_texture = _numbers_f16[score%10];
        rect.x -= width_num;
        SDL_RenderCopy(_renderer, num_texture, NULL, &rect);
        score /= 10;
    }
    SDL_DestroyTexture(score_title);
}



