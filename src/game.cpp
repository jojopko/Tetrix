/* This file is licensed under the GPL 3. 
 * For details see: www.gnu.org/licenses/gpl-3.0.txt 
 * Author: Konstantin Serezhkin (t.me/jojopko_bot) */

#include "game.h"
#include "gamefield.h"
#include "draw.h"
#include "global.h"

GameField * gamefield = nullptr;
int score = 0;
bool _is_running = false;

int get_score() {
    return score;
}

void run_game() {
	_is_running = true;
	while(_is_running) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				_is_running = false;
			}
		}
		draw_gamefield(_gamefield);
        //draw_score();
		SDL_RenderPresent(_renderer);
		SDL_Delay(16);
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderClear(_renderer);
	}
}

