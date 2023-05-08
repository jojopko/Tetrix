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
        int dx = 0, dy = 0;
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				_is_running = false;
			}
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                random_figure(_brush);
                add_score(10);
            }
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.scancode == SDL_SCANCODE_A) {
                    dx = -1;
                }
                if (e.key.keysym.scancode == SDL_SCANCODE_D) {
                    dx = 1;
                }
                if (e.key.keysym.scancode == SDL_SCANCODE_W) {
                    dy = -1;
                }
                if (e.key.keysym.scancode == SDL_SCANCODE_S) {
                    dy = 1;
                }
                if (e.key.keysym.scancode == SDL_SCANCODE_R) {
                    rotate_figure(_brush);
                }
            }
		}
        move(_gamefield, _brush, dx, dy);
		draw_gamefield(_gamefield);
        draw_score();
        draw_brush();
		SDL_RenderPresent(_renderer);
		SDL_Delay(16);
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderClear(_renderer);
	}
}

