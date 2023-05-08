/* This file is licensed under the GPL 3. 
 * For details see: www.gnu.org/licenses/gpl-3.0.txt 
 * Author: Konstantin Serezhkin (t.me/jojopko_bot) */

#include "gamefield.h"
#include "game.h"
#include "types.h"

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
    add_score(100);
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

