#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <sys/types.h>
#include "game.h"
#include "global.h"
#include "draw.h"
#include "types.h"
#include "figure.h"

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

void draw_next_figure(Brush * br) {
    if (br == nullptr) {
        printf("Next brush is 'null'\n");
        return;
    }
    SDL_Point start_pos = {305, 4};
    int margin = 1;
    SDL_Rect draw_block = {start_pos.x, start_pos.y, 16, 16};
    for (int i = 0; i < 16-4; i++) {
        set_draw_color_block(br->mask[i]);
        //SDL_SetRenderDrawColor(_renderer, 0, 51, 204, 255);
        SDL_RenderFillRect(_renderer, &draw_block);
        if ((i+1) % 4) {
            draw_block.x += draw_block.w + margin;
        }
        else {
            draw_block.x = start_pos.x;
            draw_block.y += draw_block.h + margin;
        }
    }
}

void draw_score() {
    int score = _gamestats->score;
    int max_len = 6;
    int width_num, height_num;
    SDL_Texture * score_title = _assets->labels_16pt->textures[Label_Score]; 
    SDL_Rect rect = {63, 3};
    SDL_QueryTexture(score_title, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(_renderer, score_title, NULL, &rect); // Print score lable
    rect.x = rect.x + rect.w + 5;

    TextureSet * digits = _assets->digits_16pt;
    SDL_QueryTexture(digits->textures[0], NULL, NULL, &width_num, &height_num); // NOTE: work for monospace
    rect.w = width_num;
    rect.h = height_num;
    rect.x += rect.w * max_len;
    for (int i = 0; i < max_len; i++) { // Print numbers
        SDL_Texture * num_texture = digits->textures[score%10];
        rect.x -= width_num;
        SDL_RenderCopy(_renderer, num_texture, NULL, &rect);
        score /= 10;
    }

    SDL_Texture * hi_score_title = _assets->labels_16pt->textures[Label_HiScore]; 
    SDL_Rect rect_hi = {63, 16};
    SDL_QueryTexture(hi_score_title, NULL, NULL, &rect_hi.w, &rect_hi.h);
    SDL_RenderCopy(_renderer, hi_score_title, NULL, &rect_hi); // Print score lable
    rect_hi.x = rect_hi.x + rect_hi.w + 5;

    score = get_hight_score();
    digits = _assets->digits_16pt;
    SDL_QueryTexture(digits->textures[0], NULL, NULL, &width_num, &height_num); // NOTE: work for monospace
    rect_hi.w = width_num;
    rect_hi.h = height_num;
    rect_hi.x += rect_hi.w * max_len;
    for (int i = 0; i < max_len; i++) { // Print numbers
        SDL_Texture * num_texture = digits->textures[score%10];
        rect_hi.x -= width_num;
        SDL_RenderCopy(_renderer, num_texture, NULL, &rect_hi);
        score /= 10;
    }
}

void draw_brush(Brush * br) {
    int margin = 2;
    SDL_Point start_pos = {
        16 + br->x * (BLOCK_SIZE + margin),
        64 + br->y * (BLOCK_SIZE + margin)
    };
    SDL_Rect draw_block = {start_pos.x, start_pos.y, BLOCK_SIZE, BLOCK_SIZE};
    for (int i = 0; i < MASK_SIZE; i++) {
        set_draw_color_block(br->mask[i]);
        if (br->mask[i] != BLOCK_NONE) {
            SDL_RenderFillRect(_renderer, &draw_block);
        }
        if ((i+1) % (BLOCK_SIZE / 4 - 1)) {
            draw_block.x += draw_block.w + margin;
        }
        else {
            draw_block.x = start_pos.x;
            draw_block.y += draw_block.h + margin;
        }
    }
}

void draw_title_tetrix() {
    static int start = SDL_GetTicks(), direct = 1, i = 1, current;
    SDL_Rect text_box;
    TextureSet * title = _assets->title;
    current = SDL_GetTicks();
    if (current - start >= 500) {
        i += direct;
        if (i >= title->size - 1 || i <= 0) {
            direct *= -1;
        }
        start = current;
    }
    SDL_QueryTexture(title->textures[0], NULL, NULL, &text_box.w, &text_box.h);
    text_box.x = (WIN_W-text_box.w)/2;
    text_box.y = 10;
    SDL_RenderCopy(_renderer, title->textures[i], NULL, &text_box);
}

void draw_top_scores() {
    int width_num, height_num;
    SDL_Rect text_box;
    SDL_Texture * title = _assets->labels_16pt->textures[Label_TopScores];
    SDL_QueryTexture(title, NULL, NULL, &text_box.w, &text_box.h);
    text_box.x = (WIN_W-text_box.w)/2;
    text_box.y = 300;
    SDL_RenderCopy(_renderer, title, NULL, &text_box);
    
    SDL_QueryTexture(_assets->digits_16pt->textures[0], NULL, NULL, &width_num, &height_num);
    text_box.w = width_num;
    text_box.h = height_num;
    for (int i = 0; i < 10; i++) { // Print numbers
        int score = _gamestats->scores_10[i];
        text_box.x = (WIN_W-text_box.w)/2;
        text_box.y += height_num;
        do {
            SDL_Texture * num_texture = _assets->digits_16pt->textures[score%10];
            text_box.x -= width_num;
            SDL_RenderCopy(_renderer, num_texture, NULL, &text_box);
            score /= 10;
        } while (score > 0);
    }
}

void draw_button(SDL_Rect * box, TextureSetLabels16 label) {
    SDL_Texture * text = nullptr;
    SDL_Rect text_box = *box;
    if (label == Label_NewGame1) {
        SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(_renderer, box);
    }
    if (label == Label_NewGame2) {
        SDL_SetRenderDrawColor(_renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(_renderer, box);
    }
    if (label == Label_Continue) {
        if (_gamestats->gameover || _gamestats->score < 0) {
            SDL_SetRenderDrawColor(_renderer, 64, 64, 64, 255);
        }
        else {
            SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
        }
        SDL_RenderFillRect(_renderer, box);
    }
    if (label == Label_Exit) {
        SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(_renderer, box);
    }
    text = _assets->labels_16pt->textures[label];
    SDL_QueryTexture(text, NULL, NULL, &text_box.w, &text_box.h);
    text_box.x = (WIN_W-text_box.w)/2;
    text_box.y += 3;
    SDL_RenderCopy(_renderer, text, NULL, &text_box);
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(_renderer, box);
}


