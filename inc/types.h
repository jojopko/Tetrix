/* This file is licensed under the GPL 3. 
 * For details see: www.gnu.org/licenses/gpl-3.0.txt 
 * Author: Konstantin Serezhkin (t.me/jojopko_bot) */

#ifndef _TYPES_H_
#define _TYPES_H_

struct GameField;

struct Brush;

struct Cell;

struct GameField;

#define BLOCK_COLORS_COUNT 5
enum BlockColor {
	BLOCK_NONE,
	BLOCK_RED,
	BLOCK_BLUE,
	BLOCK_GREEN,
	BLOCK_YELLOW,
	BLOCK_VIOLET
};

#endif

