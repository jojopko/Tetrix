/* This file is licensed under the GPL 3. 
 * For details see: www.gnu.org/licenses/gpl-3.0.txt 
 * Author: Konstantin Serezhkin (t.me/jojopko_bot) */

#ifndef _FIGURE_H_
#define _FIGURE_H_

#include <SDL.h>
#include "types.h"

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
    int type;
    int x;
    int y;
};

Brush * create_brush();

void free_brush(Brush * br);

void set_figure(Brush * br, FigureType type);

void rotate_figure(Brush * br);

bool can_move(GameField * gf, Brush * br);

bool can_rotate(GameField * gf, Brush * br);

void return_brush_to_start(Brush * br);

#endif

