/* This file is licensed under the GPL 3. 
 * For details see: www.gnu.org/licenses/gpl-3.0.txt 
 * Author: Konstantin Serezhkin (t.me/jojopko_bot) */

#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "figure.h"
#include "gamefield.h"
#include "global.h"
#include "log4c.h"

// Init game
GameField * _gamefield = nullptr;
Brush * _brush = nullptr;

int init_brush() {
    _brush = create_brush();
    if (_brush == nullptr) {
        printf("brush initialization failed\n");
        return 1;
    }
    set_figure(_brush, FIGURE_I);
    return 0;
}

int init_gamefield() {
    _gamefield = make_gamefield();
    if (_gamefield == nullptr) {
        printf("Gamefield initalization failed\n");
        return 1;
    }
    return 0;
}

int init_game() {
    int err = 0;
    err |= init_sdl();
    err |= init_ttf();
    err |= init_window();
    err |= init_renderer();
    err |= load_font16();
    err |= load_numbers_font16();
    err |= init_logger();
    err |= init_gamefield();
    err |= init_brush();
    return err;
}

// SDL loads

int init_sdl() {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
		printf("SDL failed\n");
        return 1;
	}
    return 0;
}

SDL_Window * _window = nullptr;

int init_window() {
    _window = SDL_CreateWindow("Tetrix\0", 0, 0, WIN_W, WIN_H, SDL_WINDOW_SHOWN);
    if (!_window) {
		printf("Window failed\n");
        return 1;
	}
    return 0;
}

SDL_Renderer * _renderer = nullptr;

int init_renderer() {
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer) {
		printf("Renderer failed\n");
        return 1;
	}
    return 0;
}

// TTF loads

int init_ttf() {
    if (TTF_Init()) {
		printf("SDL_TTF failed\n");	
		return 1;
	}
    return 0;
}

TTF_Font * _font16 = nullptr;

int load_font16() {
    _font16 = TTF_OpenFont("/home/jojopko/Desktop/Tetrix/assets/fonts/SpaceMono-Bold.ttf\0", 16);
    if (_font16 == nullptr) {
        printf("Load font 16pt -- failed\n");
        return 1;
    }
    return 0;
}

SDL_Texture ** _numbers_f16 = nullptr;

SDL_Texture * get_ttf_texture(char * text) {
    SDL_Surface * surf;
    SDL_Color fg = {255, 255, 255, 255}; // White
    surf = TTF_RenderText_Solid(_font16, text, fg);
    return SDL_CreateTextureFromSurface(_renderer, surf);
}

int load_numbers_font16() {
    const char * numbers_list = "0123456789-+\0";
    const int numbers_size = strlen(numbers_list);
    char buffer[2] = {};
    int err = 0;
    _numbers_f16 = (SDL_Texture **) calloc(numbers_size, sizeof(SDL_Texture *));
    for (int i = 0; i < numbers_size; i++) {
        buffer[0] = numbers_list[i];
        _numbers_f16[i] = get_ttf_texture(buffer);
        if (!_numbers_f16[i]) {
            err = 1;
        }
    }
    if (err) {
        printf("Textures for number (16pt) has problem.");
    }
    return 0;
}

// Init logger

log4c_category_t * logger = nullptr;

int init_logger() {
    log4c_init();
    logger = log4c_category_get("tetrix");
	if (logger == nullptr) {
		return 1;
	}
    log4c_appender_t *stdout_appender = log4c_appender_get("stdout");
    log4c_appender_set_layout(stdout_appender, log4c_layout_get("%-5p [%c] %m%n"));
    log4c_category_set_appender(logger, stdout_appender);
    log4c_category_set_priority(logger, LOG4C_PRIORITY_DEBUG);
    log4c_category_info(logger, "Logger is initialized.");
    return 0;
}


