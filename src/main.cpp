/* This file is licensed under the GPL 3. 
 * For details see: www.gnu.org/licenses/gpl-3.0.txt 
 * Author: Konstantin Serezhkin (t.me/jojopko_bot) */
#include "game.h"
#include "global.h"

int main(int argc, char ** argv) {
	if (init_game()) return 1;
	run_game();
	return 0;
}

