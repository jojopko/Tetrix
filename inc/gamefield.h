/* This file is licensed under the GPL 3. 
 * For details see: www.gnu.org/licenses/gpl-3.0.txt 
 * Author: Konstantin Serezhkin (t.me/jojopko_bot) */

#ifndef _GAMEFIELD_H_
#define _GAMEFIELD_H_

#include <SDL.h>
#include "types.h"

#define GAMEFIELD_W 15
#define GAMEFIELD_H 22

struct Cell {
	BlockColor color;
};

struct GameField {
	Cell * field;
	int w;
	int h;
};

/* Game field creation */
GameField * make_gamefield();

/* Free game field */
void free_gamefield(GameField * gf);

void do_delete_row(GameField * gf);

#endif

