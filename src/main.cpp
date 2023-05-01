/* This file is licensed under the GPL 3. 
 * For details see: www.gnu.org/licenses/gpl-3.0.txt 
 * Author: Konstantin Serezhkin (t.me/jojopko_bot) */
#include <stdlib.h>
#include <SDL.h> 
#include <log4c.h>
#include "game.h"
#include "global.h"
#include "draw.h"
#include <SDL_ttf.h>

log4c_category_t * logger = nullptr;
SDL_Renderer * _renderer = nullptr;
SDL_Window * _window = nullptr;
TTF_Font * _font16 = nullptr;
bool _is_running = false;

void init_logger() {
    log4c_init();

    logger = log4c_category_get("tetrix");
	if (logger == nullptr) {
		exit(1);
	}

    log4c_appender_t *stdout_appender = log4c_appender_get("stdout");
    log4c_appender_set_layout(stdout_appender, log4c_layout_get("%-5p [%c] %m%n"));
    log4c_category_set_appender(logger, stdout_appender);

    log4c_category_set_priority(logger, LOG4C_PRIORITY_DEBUG);

    log4c_category_info(logger, "Logger is initialized.");
	
}

void init_sdl() {
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		printf("SDL failed\n");
        exit(1);
	}
	printf("SDL is initialized\n");
    _window = SDL_CreateWindow("Tetrix\0", 0, 0, WIN_W, WIN_H, SDL_WINDOW_SHOWN);
    if (!_window) {
		printf("Window failed\n");
        exit(1);
	}
	printf("Renderer is initialized\n");
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer) {
		printf("Renderer failed\n");
        exit(1);
	}
	printf("Window is initialized\n");
	if (TTF_Init()) {
		printf("SDL_TTF failed\n");	
		exit(1);
	}
	printf("SDL_TTF is initialized\n");
	_font16 = TTF_OpenFont("/home/jojopko/Desktop/Tetrix/assets/fonts/SpaceMono-Bold.ttf\0", 16);
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
    SDL_RenderPresent(_renderer);

}

void Event() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            _is_running = false;
        }
    }
}

int main(int argc, char ** argv) {
	init_logger();
	init_sdl();
	init_game();
	run_game();
	return 0;
}

