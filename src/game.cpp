/* This file is licensed under the GPL 3. 
 * For details see: www.gnu.org/licenses/gpl-3.0.txt 
 * Author: Konstantin Serezhkin (t.me/jojopko_bot) */

#include "game.h"
#include "gamefield.h"
#include "draw.h"
#include "global.h"
#include "types.h"
#include <figure.h>
#include <SDL_events.h>
#include <SDL_scancode.h>

/* GameField * gamefield = nullptr; */
int score = 0;
bool _is_running = false;

int get_score() {
    return score;
}

void add_score(int n) {
    score += n;
}

void run_game() {
	_is_running = true;
	while(_is_running) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				_is_running = false;
			}
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                add_score(10);
            }
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.scancode == SDL_SCANCODE_A) {
                    _brush->x -= 1;
                }
                if (e.key.keysym.scancode == SDL_SCANCODE_D) {
                    _brush->x += 1;
                }
                if (e.key.keysym.scancode == SDL_SCANCODE_W) {
                    _brush->y -= 1;
                }
                if (e.key.keysym.scancode == SDL_SCANCODE_S) {
                    _brush->y += 1;
                }
                if (e.key.keysym.scancode == SDL_SCANCODE_R) {
                    rotate_figure(_brush);
                }
            }
		}
		draw_gamefield(_gamefield);
        draw_score();
        draw_brush();
		SDL_RenderPresent(_renderer);
		SDL_Delay(16);
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderClear(_renderer);
	}
}

