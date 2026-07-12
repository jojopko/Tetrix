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

void clear_gamefield(GameField * gf);

void fall_blocks_gm2(GameField * gf);

void fill_blocks_gm2(GameField * gf);

#endif

