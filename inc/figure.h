#ifndef _FIGURE_H_
#define _FIGURE_H_

#include <SDL.h>
#include "types.h"

#define FIGURES_COUNT 7

enum FigureType {
    /* |#|#|#|#|
     * | | | | | */
    FIGURE_I,

    /* | |#| | |
     * |#|#|#| | */
    FIGURE_T,

    /* | | | |#|
     * |#|#|#|#| */
    FIGURE_J,

    /* | |#|#| | 
     * |#|#| | | */
    FIGURE_S,

    /* |#|#| | |
     * |#|#| | |*/
    FIGURE_O,

    /* |#| | | |
     * |#|#|#|#|*/
    FIGURE_L,

    /* |#|#| | |
     * | |#|#| |*/
    FIGURE_Z
};

#define MASK_SIZE 16

struct Brush {
    BlockColor * mask; // 4x4
    int rotate;
    FigureType type;
    int x;
    int y;
};

Brush * create_brush();

void free_brush(Brush * br);

void set_figure(Brush * br, FigureType type, BlockColor color);

void try_rotate(Brush * br, GameField * gf);

void rotate_figure(Brush * br);

void move(GameField * gf, Brush * br, int dx, int dy);

bool can_rotate(GameField * gf, Brush * br);

void random_figure(Brush * br);

void copy_brush_mask(BlockColor * source, BlockColor * dest);

void move2(GameField * gf, Brush * br, int dx, int dy);

#endif

