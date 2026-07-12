#include "gamefield.h"
#include "game.h"
#include "types.h"
#include "global.h"
#include <stdlib.h>

GameField * make_gamefield() {
    GameField * gf = (GameField *) calloc(1, sizeof(GameField));
    gf->w = GAMEFIELD_W;
    gf->h = GAMEFIELD_H;
    gf->field = (Cell *) calloc(gf->w * gf->h, sizeof(Cell));
    return gf;
}

void free_gamefield(GameField * gf) {
    free(gf->field);
    free(gf);
}

// FIXME: bug on filled screen
void fall_row(GameField * gf, int y) {
    for (y = y - 1; y > 0; y--) {
        for (int x = 0; x < gf->w; x++) {
            int index_copy = y * gf->w + x;
            int index_paste = (y+1) * gf->w + x;
            gf->field[index_paste] = gf->field[index_copy];
        }
    }
    _gamestats->score += 100;
}

void do_delete_row(GameField * gf) {
    for (int y = 0; y < gf->h; y++) {
        bool do_delete = true;
        for (int x = 0; x < gf->w; x++) {
            if (gf->field[y*gf->w + x].color == BLOCK_NONE) {
                do_delete = false;
                break;
            }
        }
        if (do_delete) {
            fall_row(gf, y);
        }
    }
}

void clear_gamefield(GameField * gf) {
    for (int i = 0; i < gf->w * gf->h; i++) {
        gf->field[i].color = BLOCK_NONE;
    }
}

void fall_blocks_gm2(GameField * gf) {
    for (int i = 0; i < gf->h; i++)
    for (int col = 0; col < gf->w; col++) {
        for (int row = gf->h-1; row > 1; row--) {
            Cell * current_block = &gf->field[row*gf->w + col];
            Cell * up_block;
            if (row-1 >= 0) {
                up_block = &gf->field[(row-1)*gf->w + col];
                if (current_block->color == BLOCK_NONE) {
                    current_block->color = up_block->color;
                    up_block->color = BLOCK_NONE;
                }
            } 
        }
    }
}

int index(int x, int y) {
    return y * _gamestats->gamefield->w + x;
}

void fill(BlockColor * colors, int * filled, int start_x, int start_y, int new_color, BlockColor target_color, int * count) {
    if (start_x < 0 || start_x >= _gamestats->gamefield->w ||
            start_y < 0 || start_y >= _gamestats->gamefield->h)
        return;
    if (filled[index(start_x, start_y)] != 0 || colors[index(start_x, start_y)] != target_color)
        return;

    filled[index(start_x, start_y)] = new_color;
    (*count)++;

    fill(colors, filled, start_x + 1, start_y, new_color, target_color, count);
    fill(colors, filled, start_x - 1, start_y, new_color, target_color, count);
    fill(colors, filled, start_x, start_y + 1, new_color, target_color, count);
    fill(colors, filled, start_x, start_y - 1, new_color, target_color, count);
}

void fill_blocks_gm2(GameField * gf) {
    BlockColor * colors_copy = (BlockColor *) malloc(gf->w*gf->h*sizeof(BlockColor));
    int * filled = (int *) calloc(gf->w*gf->h, sizeof(int));
    int * count_colors = (int *) calloc(gf->w * gf->h, sizeof(int));
    for (int i = 0; i < gf->w * gf->h; i++) {
        colors_copy[i] = gf->field[i].color;
    }
    int color = 1;
    int counter = 0;
    for (int i = 0; i < gf->w * gf->h; i++) {
        if (colors_copy[i] == BLOCK_NONE || filled[i] != 0) 
            continue;
        fill(colors_copy, filled, i % gf->w, i / gf->w, color, colors_copy[i], &counter);
        count_colors[color-1] = counter;
        counter = 0;
        color++;
    }
    for (; color > 0; --color) {
        if (count_colors[color-1] >= 4) {
            for (int i = 0; i < gf->w * gf->h; i++) {
                if (filled[i] == color) {
                    gf->field[i].color = BLOCK_NONE;
                }
            }
            _gamestats->score += 10 * count_colors[color-1];
        }
    }
    free(count_colors);
    free(filled);
    free(colors_copy);
}

