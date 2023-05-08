/* This file is licensed under the GPL 3. 
 * For details see: www.gnu.org/licenses/gpl-3.0.txt 
 * Author: Konstantin Serezhkin (t.me/jojopko_bot) */

#include "figure.h"
#include "gamefield.h"
#include "types.h"
#include "global.h"
#include <cstdlib>
#include <malloc.h>

Brush * create_brush() {
    Brush * brush = (Brush *) calloc(1, sizeof(Brush));
    brush->rotate = 0;
    brush->x = 0;
    brush->y = 0;
    brush->mask = (BlockColor *) calloc(MASK_SIZE, sizeof(BlockColor));
    return brush;
}

void free_brush(Brush * br) {
    free(br->mask);
    free(br);
} 

void parse_figure_pattern(const char * pattern, BlockColor color, Brush * br) {
    for (int i = 0; i < 16; i++) {
        if (pattern[i] == '1') {
            br->mask[i] = color;
        }
        else {
            br->mask[i] = BLOCK_NONE;
        }
    }
}

// FIXME: redesign brush for normar rotate (4x2)
void set_figure(Brush * br, FigureType type, BlockColor color) {
    if (type == FIGURE_I) {
        parse_figure_pattern("0000" "1111" "0000" "0000", color, br);
    }
    if (type == FIGURE_T) {
        parse_figure_pattern("0010" "0111" "0000" "0000", color, br);
    }
    if (type == FIGURE_J) {
        parse_figure_pattern("0001" "1111" "0000" "0000", color, br);
    }
    if (type == FIGURE_S) {
        parse_figure_pattern("0110" "1100" "0000" "0000", color, br);
    }
    if (type == FIGURE_O) {
        parse_figure_pattern("0000" "0110" "0110" "0000", color, br);
    }
    if (type == FIGURE_L) {
        parse_figure_pattern("0000" "1000" "1111" "0000", color, br);
    }
    if (type == FIGURE_Z) {
        parse_figure_pattern("0000" "1100" "0110" "0000", color, br);
    }
}

void rotate_figure(Brush * br) {
    BlockColor temp;
    for (int i = 0; i < 4; i++) {
        for (int j = i; j < 4; j++) {
            temp = _brush->mask[i*4 + j];
            _brush->mask[i*4 + j] = _brush->mask[j*4 + i];
            _brush->mask[j*4 + i] = temp;
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            temp = _brush->mask[i*4 + j];
            _brush->mask[i*4 + j] = _brush->mask[i*4 + (3-j)];
            _brush->mask[i*4 + (3-j)] = temp;
        }
    }
}

void do_freeze_brush(Brush * br, GameField * gf) {
    gf = _gamefield;
    br = _brush;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int real_y = (br->y + y);
            int real_x = (br->x + x);
            BlockColor c = br->mask[y*4 + x];
            if (c != BLOCK_NONE) {
                gf->field[real_y * gf->w + real_x].color = c;
            }
        }
    }
    br->y = 0;
}

void try_move(GameField * gf, Brush * br, int dx, int dy) {
    gf = _gamefield;
    br = _brush;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int real_y = (br->y + y);
            int real_x = (br->x + x);
            if (br->mask[y*4 + x] != BLOCK_NONE) {
                BlockColor bottom = gf->field[(real_y+1) * gf->w + real_x].color;
                BlockColor side = gf->field[(real_y) * gf->w + real_x + dx].color;
                if (real_x + dx < 0) {
                    dx = 0;
                }
                if (real_x + dx >= gf->w) {
                    dx = 0;
                }
                if (side != BLOCK_NONE) {
                    dx = 0;
                }
                if (real_y + dy < 0) {
                    dy = 0;
                }
                if (real_y + dy >= gf->h) {
                    do_freeze_brush(br, gf);
                    dy = 0;
                }
                if (dy > 0 && bottom != BLOCK_NONE) {
                    do_freeze_brush(br, gf);
                    dy = 0;
                }
            }
        }
    }
    br->x += dx;
    br->y += dy;
}

void move(GameField * gf, Brush * br, int dx, int dy) {
    try_move(gf, br, dx, dy);
}

void random_figure(Brush * br) {
    FigureType r_figure = (FigureType) (rand() % FIGURES_COUNT);
    BlockColor r_color = (BlockColor) (rand() % BLOCK_COLORS_COUNT + 1);
    set_figure(br, r_figure, r_color);
}

