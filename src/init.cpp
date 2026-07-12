#include <SDL_render.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "figure.h"
#include "gamefield.h"
#include "global.h"
#include "types.h"

// Init game
GameField * _gamefield = nullptr;
Brush * _brush = nullptr;
Brush * _next_block = nullptr;
GameAssets * _assets = nullptr;
GameStats * _gamestats = nullptr;

TTF_Font * font16 = nullptr;
TTF_Font * font32 = nullptr;
SDL_Window * _window = nullptr;
SDL_Renderer * _renderer = nullptr;

int init_brush() {
    _gamestats->current_brush = create_brush();
    _gamestats->next_brush = create_brush();
    if (_gamestats->current_brush == nullptr || _gamestats->next_brush == nullptr) {
        printf("brush initialization failed\n");
        return 1;
    }
    random_figure(_gamestats->current_brush);
    _gamestats->next_brush->x = 10;
    _gamestats->next_brush->y = -2;
    random_figure(_gamestats->next_brush);
    return 0;
}

int init_gamefield() {
    _gamestats->gamefield = make_gamefield();
    if (_gamestats->gamefield == nullptr) {
        printf("Gamefield initalization failed\n");
        return 1;
    }
    return 0;
}

int init_ttf() {
    if (TTF_Init()) {
        printf("SDL_TTF failed\n");	
        return 1;
    }
    return 0;
}

int load_font16() {
    font16 = TTF_OpenFont("/home/jojopko/Desktop/Tetrix/assets/fonts/SpaceMono-Bold.ttf\0", 16);
    if (font16 == nullptr) {
        printf("Load font 16pt -- failed\n");
        return 1;
    }
    return 0;
}

int load_font32() {
    font32 = TTF_OpenFont("/home/jojopko/Desktop/Tetrix/assets/fonts/SpaceMono-Bold.ttf\0", 32);
    if (font16 == nullptr) {
        printf("Load font 32pt -- failed\n");
        return 1;
    }
    return 0;
}

SDL_Texture * get_ttf_texture(TTF_Font * font, const char * text, Uint8 r, Uint8 g, Uint8 b) {
    SDL_Surface * surf;
    SDL_Color fg = {r, g, b, 255};
    surf = TTF_RenderText_Solid(font, text, fg);
    return SDL_CreateTextureFromSurface(_renderer, surf);
}

int load_numbers_font16() {
    const char * numbers_list = "0123456789-+\0";
    const int numbers_size = strlen(numbers_list);
    char buffer[2] = {};
    int err = 0;
    TextureSet * digits = _assets->digits_16pt;
    digits->size = numbers_size;
    digits->textures = (SDL_Texture **) calloc(digits->size, sizeof(SDL_Texture *));
    for (int i = 0; i < digits->size; i++) {
        buffer[0] = numbers_list[i];
        digits->textures[i] = get_ttf_texture(font16, buffer, 255, 255, 255);
        if (!digits->textures[i]) {
            err = 1;
        }
    }
    if (err) {
        printf("Textures for number (16pt) has problem.\n");
    }
    return err;
}

int load_labels_font16() {
    int err = 0;
    TextureSet * labels = _assets->labels_16pt;
    labels->size = 10;
    labels->textures = (SDL_Texture **) calloc(labels->size, sizeof(SDL_Texture *));
    labels->textures[Label_Score] = get_ttf_texture(font16, "Score:\0", 255, 255, 255);
    labels->textures[Label_NewGame1] = get_ttf_texture(font16, "New Game 1\0", 255, 255, 255);
    labels->textures[Label_NewGame2] = get_ttf_texture(font16, "New Game 2\0", 255, 255, 255);
    labels->textures[Label_Continue] = get_ttf_texture(font16, "Continue\0", 255, 255, 255);
    labels->textures[Label_Exit] = get_ttf_texture(font16, "Exit\0", 255, 255, 255);
    labels->textures[Label_Last_Score] = get_ttf_texture(font16, "Last score:\0", 255, 255, 255);
    labels->textures[Label_HiScore] = get_ttf_texture(font16, "Hi score:\0", 255, 255, 255);
    labels->textures[Label_TopScores] = get_ttf_texture(font16, "Top 10:\0", 255, 255, 255);
    return err;
}

SDL_Texture* IMG_LoadTexture(SDL_Renderer* renderer, const char* filename) {
    SDL_Surface* surface = SDL_LoadBMP(filename);
    if (!surface) {
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Ошибка создания текстуры: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(surface);
    return texture;
}

int load_title() {
    int err = 0;
    TextureSet * title = _assets->title;
    title->size = 13;
    title->textures = (SDL_Texture **) calloc(title->size, sizeof(SDL_Texture *));
    title->textures[0] = IMG_LoadTexture(_renderer, "/home/jojopko/Desktop/Tetrix/assets/title/0.bmp\0");
    title->textures[1] = IMG_LoadTexture(_renderer, "/home/jojopko/Desktop/Tetrix/assets/title/1.bmp\0");
    title->textures[2] = IMG_LoadTexture(_renderer, "/home/jojopko/Desktop/Tetrix/assets/title/2.bmp\0");
    title->textures[3] = IMG_LoadTexture(_renderer, "/home/jojopko/Desktop/Tetrix/assets/title/3.bmp\0");
    title->textures[4] = IMG_LoadTexture(_renderer, "/home/jojopko/Desktop/Tetrix/assets/title/4.bmp\0");
    title->textures[5] = IMG_LoadTexture(_renderer, "/home/jojopko/Desktop/Tetrix/assets/title/5.bmp\0");
    title->textures[6] = IMG_LoadTexture(_renderer, "/home/jojopko/Desktop/Tetrix/assets/title/6.bmp\0");
    title->textures[7] = IMG_LoadTexture(_renderer, "/home/jojopko/Desktop/Tetrix/assets/title/7.bmp\0");
    title->textures[8] = IMG_LoadTexture(_renderer, "/home/jojopko/Desktop/Tetrix/assets/title/8.bmp\0");
    title->textures[9] = IMG_LoadTexture(_renderer, "/home/jojopko/Desktop/Tetrix/assets/title/9.bmp\0");
    title->textures[10] = IMG_LoadTexture(_renderer, "/home/jojopko/Desktop/Tetrix/assets/title/10.bmp\0");
    title->textures[11] = IMG_LoadTexture(_renderer, "/home/jojopko/Desktop/Tetrix/assets/title/11.bmp\0");
    title->textures[12] = IMG_LoadTexture(_renderer, "/home/jojopko/Desktop/Tetrix/assets/title/12.bmp\0");
    return 0;
}

int load_labels_font32() {
    int err = 0;
    TextureSet * labels = _assets->labels_32pt;
    labels->size = 5;
    labels->textures = (SDL_Texture **) calloc(labels->size, sizeof(SDL_Texture *));
    labels->textures[Label_Tetrix] = get_ttf_texture(font32, "Tetrix\0", 255, 255, 255);
    return err;
}

int load_assets() {
    int err = 0;
    _assets = (GameAssets *) calloc(1, sizeof(GameAssets));
    _assets->digits_16pt = (TextureSet *) calloc(1, sizeof(TextureSet));
    _assets->labels_16pt = (TextureSet *) calloc(1, sizeof(TextureSet));
    _assets->labels_32pt = (TextureSet *) calloc(1, sizeof(TextureSet));
    _assets->title = (TextureSet *) calloc(1, sizeof(TextureSet));
    err |= load_font16();
    err |= load_font32();
    err |= load_numbers_font16();
    err |= load_labels_font16();
    err |= load_labels_font32();
    err |= load_title();
    return err;
}

int init_game() {
    int err = 0;
    _gamestats = (GameStats *) calloc(1, sizeof(GameStats));
    err |= init_sdl();
    err |= init_ttf();
    err |= init_window();
    err |= init_renderer();
    err |= load_assets();
    err |= init_gamefield();
    err |= init_brush();
    if (!err) {
        _gamestats->is_stoped = false;
        _gamestats->is_running = true;
        _gamestats->score = -1;
        _gamestats->current_scene = Scene_Menu;
        _gamestats->random_seed = 0;
        _gamestats->scores_10 = (int *) calloc(10, sizeof(int));
        find_largest_numbers();
    }
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


int init_window() {
    _window = SDL_CreateWindow("Tetrix\0", 0, 0, WIN_W, WIN_H, SDL_WINDOW_SHOWN);
    if (!_window) {
        printf("Window failed\n");
        return 1;
    }
    return 0;
}


int init_renderer() {
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer) {
        printf("Renderer failed\n");
        return 1;
    }
    return 0;
}

// Quit

void exit_game() {
    SDL_DestroyRenderer(_renderer);    
    SDL_DestroyWindow(_window);
    TTF_CloseFont(font16);
    SDL_Quit();
}

