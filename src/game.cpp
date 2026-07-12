#include "game.h"
#include "gamefield.h"
#include "draw.h"
#include "global.h"
#include "types.h"
#include <SDL_mouse.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <stdlib.h>
#include <stdio.h>
#include <figure.h>
#include <SDL_events.h>
#include <SDL_scancode.h>

void save_score() {
    const char * filename = "/tmp/tetrix.scores.txt\0";
    FILE * fd = fopen(filename, "a");
    if (fd == NULL) {
        fd = fopen(filename, "w");
        if (fd == NULL) {
            return;
        }
    }
    fprintf(fd, "%d\n", _gamestats->score);
    fclose(fd);
}

void find_largest_numbers() {
    FILE * file;
    int current_number;
    const char * file_path = "/tmp/tetrix.scores.txt\0";
    for (int i = 0; i < 10; i++) {
        _gamestats->scores_10[i] = 0;
    }
    file = fopen(file_path, "r");
    if (file == NULL) {
        return;
    }
    while (fscanf(file, "%d", &current_number) == 1) {
        for (int i = 0; i < 10; i++) {
            if (current_number > _gamestats->scores_10[i]) {
                for (int j = 9; j > i; j--) {
                    _gamestats->scores_10[j] = _gamestats->scores_10[j - 1];
                }
                _gamestats->scores_10[i] = current_number;
                break;
            }
        }
    }
    fclose(file);
}

int get_hight_score() {
    const char * filename = "/tmp/tetrix.scores.txt\0";
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }
    int maxNumber = 0;
    int number;
    while (fscanf(file, "%d", &number) != EOF) {
        if (number > maxNumber) {
            maxNumber = number;
        }
    }
    fclose(file);
    return maxNumber;
}

void save_current_state() {
    const char * filename = "/tmp/tetrix.save.txt\0";
    FILE * fd = fopen(filename, "w");
    if (fd == NULL) {
        return;
    }
    fprintf(fd, "%d ", _gamestats->score);
    fprintf(fd, "%d ", (int) _gamestats->is_stoped);
    fprintf(fd, "%d ", (int) _gamestats->is_running);
    fprintf(fd, "%d ", (int) _gamestats->gameover);
    fprintf(fd, "%d ", (int) _gamestats->current_scene);
    // gamefield
    fprintf(fd, "%d %d ", _gamestats->gamefield->w, _gamestats->gamefield->h);
    for (int i = 0; i < _gamestats->gamefield->w * _gamestats->gamefield->h; i++) {
        fprintf(fd, "%d ", (int) _gamestats->gamefield->field[i].color);
    }
    // Brush
    fprintf(fd, "%d ", (int) _gamestats->current_brush->type);
    fprintf(fd, "%d ", _gamestats->current_brush->x);
    fprintf(fd, "%d ", _gamestats->current_brush->y);
    fprintf(fd, "%d ", _gamestats->current_brush->rotate);
    fprintf(fd, "%d ", 16);
    for (int i = 0; i < 16; i++) {
        fprintf(fd, "%d ", (int) _gamestats->current_brush->mask[i]);
    }
    // Next figure brush
    fprintf(fd, "%d ", (int) _gamestats->current_brush->type);
    fprintf(fd, "%d ", _gamestats->current_brush->rotate);
    fprintf(fd, "%d ", 16);
    for (int i = 0; i < 16; i++) {
        fprintf(fd, "%d ", (int) _gamestats->current_brush->mask[i]);
    }
    fprintf(fd, "\n");
    fclose(fd);
}

void append_current_state() {
    const char * filename = "/tmp/tetrix.demo.txt\0";
    FILE * fd = fopen(filename, "a");
    if (fd == NULL) {
        fd = fopen(filename, "w");
        if (fd == NULL) {
            return;
        }
    }
    fprintf(fd, "%d ", _gamestats->score);
    fprintf(fd, "%d ", (int) _gamestats->is_stoped);
    fprintf(fd, "%d ", (int) _gamestats->is_running);
    fprintf(fd, "%d ", (int) _gamestats->gameover);
    //fprintf(fd, "%d\n", (int) _gamestats->current_scene);
    // gamefield
    fprintf(fd, "%d %d ", _gamestats->gamefield->w, _gamestats->gamefield->h);
    for (int i = 0; i < _gamestats->gamefield->w * _gamestats->gamefield->h; i++) {
        fprintf(fd, "%d ", (int) _gamestats->gamefield->field[i].color);
    }
    // Brush
    fprintf(fd, "%d ", (int) _gamestats->current_brush->type);
    fprintf(fd, "%d ", _gamestats->current_brush->x);
    fprintf(fd, "%d ", _gamestats->current_brush->y);
    fprintf(fd, "%d ", _gamestats->current_brush->rotate);
    fprintf(fd, "%d ", 16);
    for (int i = 0; i < 16; i++) {
        fprintf(fd, "%d ", (int) _gamestats->current_brush->mask[i]);
    }
    // Next figure brush
    fprintf(fd, "%d ", (int) _gamestats->current_brush->type);
    fprintf(fd, "%d ", _gamestats->current_brush->rotate);
    fprintf(fd, "%d ", 16);
    for (int i = 0; i < 16; i++) {
        fprintf(fd, "%d ", (int) _gamestats->current_brush->mask[i]);
    }
    fprintf(fd, "\n");
    fclose(fd);
}

void load_state_from_file() {
    const char * filename = "/tmp/tetrix.save.txt\0";
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }
    fscanf(file, "%d", &_gamestats->score);
    fscanf(file, "%d", (int *) &_gamestats->is_stoped);
    fscanf(file, "%d", (int *) &_gamestats->is_running);
    fscanf(file, "%d", (int *) &_gamestats->gameover);
    fscanf(file, "%d", (int *) &_gamestats->current_scene);
    // gamefield
    GameField * gf = _gamestats->gamefield;
    fscanf(file, "%d %d", &gf->w, &gf->h);
    for (int i = 0; i < gf->w * gf->h; i++) {
        fscanf(file, "%d", (int *) &gf->field[i].color);
    }
    // brush
    Brush * curr_br = _gamestats->current_brush;
    fscanf(file, "%d", (int *) &curr_br->type);
    fscanf(file, "%d", &curr_br->x);
    fscanf(file, "%d", &curr_br->y);
    fscanf(file, "%d", &curr_br->rotate);
    int size;
    fscanf(file, "%d", &size);
    for (int i = 0; i < 16; i++) {
        fscanf(file, "%d", (int *) &curr_br->mask[i]);
    }
    // next figure
    Brush * next_br = _gamestats->current_brush;
    fscanf(file, "%d", (int *) &next_br->type);
    fscanf(file, "%d", &next_br->rotate);
    fscanf(file, "%d", &size);
    for (int i = 0; i < 16; i++) {
        fscanf(file, "%d", (int *) &next_br->mask[i]);
    }
    fclose(file);
}

FILE * file = nullptr;

void load_demo_state_from_file() {
    const char * filename = "/tmp/tetrix.demo.txt\0";
    file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }
    fscanf(file, "%d", &_gamestats->score);
    fscanf(file, "%d", (int *) &_gamestats->is_stoped);
    fscanf(file, "%d", (int *) &_gamestats->is_running);
    fscanf(file, "%d", (int *) &_gamestats->gameover);
    //fscanf(file, "%d", (int *) &_gamestats->current_scene);
    // gamefield
    GameField * gf = _gamestats->gamefield;
    fscanf(file, "%d %d", &gf->w, &gf->h);
    for (int i = 0; i < gf->w * gf->h; i++) {
        fscanf(file, "%d", (int *) &gf->field[i].color);
    }
    // brush
    Brush * curr_br = _gamestats->current_brush;
    fscanf(file, "%d", (int *) &curr_br->type);
    fscanf(file, "%d", &curr_br->x);
    fscanf(file, "%d", &curr_br->y);
    fscanf(file, "%d", &curr_br->rotate);
    int size;
    fscanf(file, "%d", &size);
    for (int i = 0; i < 16; i++) {
        fscanf(file, "%d", (int *) &curr_br->mask[i]);
    }
    // next figure
    Brush * next_br = _gamestats->current_brush;
    fscanf(file, "%d", (int *) &next_br->type);
    fscanf(file, "%d", &next_br->rotate);
    fscanf(file, "%d", &size);
    for (int i = 0; i < 16; i++) {
        fscanf(file, "%d", (int *) &next_br->mask[i]);
    }
}

void clean_demo() {
    const char * filename = "/tmp/tetrix.demo.txt\0";
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return;
    }
    fclose(file);
}

void check_gameover(GameField * gf) {
    for (int i = 0; i < gf->w; i++) {
        if (gf->field[i].color != BLOCK_NONE) {
            _gamestats->gameover = true;
            break;
        }
    }
}

void use_scene_gamemode1() {
    int dx = 0, dy = 0;
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            _gamestats->is_running = false;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            //random_figure(_gamestats->current_brush);
            //append_current_state();
        }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.scancode == SDL_SCANCODE_A) {
                dx = -1;
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_D) {
                dx = 1;
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_W) {
//                dy = -1;
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_S) {
                dy = 1;
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_R) {
                try_rotate(_gamestats->current_brush, _gamestats->gamefield);
                append_current_state();
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                _gamestats->current_scene = Scene_Menu;
                find_largest_numbers();
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_O) {
                save_current_state();
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_P) {
                load_state_from_file();
            }
        }
    }
    static int ticks_start = SDL_GetTicks();
    int curr_ticks = SDL_GetTicks();
    if (dy) ticks_start = curr_ticks;
    if (curr_ticks - ticks_start >= 1000) {
        dy = 1;
        ticks_start = curr_ticks;
    }
    check_gameover(_gamestats->gamefield);
    if (_gamestats->gameover) {
        _gamestats->current_scene = Scene_Menu;
        save_score();
        find_largest_numbers();
    }
    move(_gamestats->gamefield, _gamestats->current_brush, dx, dy);
    draw_gamefield(_gamestats->gamefield);
    draw_score();
    draw_brush(_gamestats->current_brush);
    draw_next_figure(_gamestats->next_brush);
}

void use_scene_menu() {
    // Кнопки
    SDL_Rect btn_continue  = {(WIN_W - 256)/2, 128, 256, 32};
    SDL_Rect btn_new_game1 = {(WIN_W - 256)/2, btn_continue.y+40, 256, 32};
    SDL_Rect btn_new_game2 = {(WIN_W - 256)/2, btn_new_game1.y+40, 256, 32};
    SDL_Rect btn_quit      = {(WIN_W - 256)/2, btn_new_game2.y+40, 256, 32};

    SDL_Point mouse_pos;
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_MOUSEBUTTONUP) {
            SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
            if (SDL_PointInRect(&mouse_pos, &btn_new_game1)) {
                srand(_gamestats->random_seed);
                random_figure(_gamestats->current_brush);
                random_figure(_gamestats->next_brush);
                _gamestats->current_scene = Scene_Gamemode1;
                _gamestats->gameover = false;
                _gamestats->current_brush->y = 0;
                _gamestats->score = 0;
                clear_gamefield(_gamestats->gamefield);
                clean_demo();
            }
            if (SDL_PointInRect(&mouse_pos, &btn_new_game2)) {
                srand(_gamestats->random_seed);
                _gamestats->current_scene = Scene_Gamemode2;
                random_figure(_gamestats->current_brush);
                random_figure(_gamestats->next_brush);
                _gamestats->gameover = false;
                _gamestats->current_brush->y = 0;
                _gamestats->score = 0;
                clear_gamefield(_gamestats->gamefield);
                clean_demo();
            }
            if (SDL_PointInRect(&mouse_pos, &btn_continue)) {
                if (_gamestats->gameover == false && _gamestats->score >= 0) {
                    _gamestats->current_scene = Scene_Gamemode1;
                }
            }
            if (SDL_PointInRect(&mouse_pos, &btn_quit)) {
                exit_game();
                clean_demo();
                exit(0);
            }
        }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.scancode == SDL_SCANCODE_P) {
                load_state_from_file();
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_I) {
                _gamestats->current_scene = Scene_Demomode;
            }
        }
        if (e.type == SDL_QUIT) {
            _gamestats->is_running = false;
        }
    }
    static int r = 0, g = 0, b = 0;
    static int direction = 1;
    r += direction;
    g += direction;
    b += direction;
    if (r >= 128) {
        direction = -1;
    } 
    else if (r <= 0) {
        direction = 1;
    }
    
    SDL_SetRenderDrawColor(_renderer, r, g, b, 255);
    SDL_RenderFillRect(_renderer, NULL);
    draw_title_tetrix();
    draw_button(&btn_continue, Label_Continue);
    draw_button(&btn_new_game1, Label_NewGame1);
    draw_button(&btn_new_game2, Label_NewGame2);
    draw_button(&btn_quit, Label_Exit);
    draw_top_scores();
}

void use_scene_gamemode2() {
    int dx = 0, dy = 0;
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            _gamestats->is_running = false;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            //fall_blocks_gm2(_gamestats->gamefield);
            //random_figure(_gamestats->current_brush);
        }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.scancode == SDL_SCANCODE_A) {
                dx = -1;
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_D) {
                dx = 1;
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_W) {
//                dy = -1;
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_S) {
                dy = 1;
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_R) {
                try_rotate(_gamestats->current_brush, _gamestats->gamefield);
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_O) {
                save_current_state();
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_P) {
                load_state_from_file();
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                _gamestats->current_scene = Scene_Menu;
                find_largest_numbers();
            }
        }
    }
    static int ticks_start = SDL_GetTicks();
    int curr_ticks = SDL_GetTicks();
    if (dy) ticks_start = curr_ticks;
    if (curr_ticks - ticks_start >= 1000) {
        dy = 1;
        ticks_start = curr_ticks;
        fill_blocks_gm2(_gamestats->gamefield);
        fall_blocks_gm2(_gamestats->gamefield);
    }
    check_gameover(_gamestats->gamefield);
    if (_gamestats->gameover) {
        save_score();
        _gamestats->current_scene = Scene_Menu;
        find_largest_numbers();
    }
    move2(_gamestats->gamefield, _gamestats->current_brush, dx, dy);
    draw_gamefield(_gamestats->gamefield);
    draw_score();
    draw_brush(_gamestats->current_brush);
    draw_next_figure(_gamestats->next_brush);
}

void use_scene_demomode() {
    int dx = 0, dy = 0;
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            _gamestats->is_running = false;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
        }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.scancode == SDL_SCANCODE_A) {
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_D) {
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_W) {
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_S) {
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_R) {
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                _gamestats->current_scene = Scene_Menu;
                find_largest_numbers();
            }
        }
    }
    static int ticks_start = SDL_GetTicks();
    int curr_ticks = SDL_GetTicks();
    if (dy) ticks_start = curr_ticks;
    if (curr_ticks - ticks_start >= 1000) {
        load_demo_state_from_file();
    }
    draw_gamefield(_gamestats->gamefield);
    draw_score();
    draw_brush(_gamestats->current_brush);
    //draw_next_figure(_gamestats->next_brush);
}

void update() {
    if (_gamestats->current_scene == Scene_Gamemode1) {
        use_scene_gamemode1();
    }
    if (_gamestats->current_scene == Scene_Gamemode2) {
        use_scene_gamemode2();
    }
    if (_gamestats->current_scene == Scene_Menu) {
        use_scene_menu();
    }
    if (_gamestats->current_scene == Scene_Demomode) {
        use_scene_demomode();
    }
}

void run_game() {
    _gamestats->is_running = true;
    while(_gamestats->is_running) {
        update();
        SDL_RenderPresent(_renderer);
        SDL_Delay(16);
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
        SDL_RenderClear(_renderer);
    }
    fclose(file);
}

