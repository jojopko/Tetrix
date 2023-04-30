/* This file is licensed under the GPL 3. 
 * For details see: www.gnu.org/licenses/gpl-3.0.txt 
 * Author: Konstantin Serezhkin (t.me/jojopko_bot) */

#include "gamefield.h"
#include "types.h"

GameField * init_gamefield() {
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


