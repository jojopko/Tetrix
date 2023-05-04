/* This file is licensed under the GPL 3. 
 * For details see: www.gnu.org/licenses/gpl-3.0.txt 
 * Author: Konstantin Serezhkin (t.me/jojopko_bot) */

#include "figure.h"
#include "types.h"
#include "global.h"
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

void set_figure(Brush * br, FigureType type) {
    if (type == FIGURE_I) {
        for(int i = 0; i < MASK_SIZE; i++) {
            br->mask[i] = BLOCK_BLUE;
        }
        br->mask[MASK_SIZE-1] = BLOCK_NONE;
    }
}

void rotate_figure(Brush * br) {
    BlockColor temp;

    // Транспонирование матрицы
    for (int i = 0; i < 4; i++) {
        for (int j = i; j < 4; j++) {
            temp = _brush->mask[i*4 + j];
            _brush->mask[i*4 + j] = _brush->mask[j*4 + i];
            _brush->mask[j*4 + i] = temp;
        }
    }

    // Переворот матрицы на 90 градусов
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            temp = _brush->mask[i*4 + j];
            _brush->mask[i*4 + j] = _brush->mask[i*4 + (3-j)];
            _brush->mask[i*4 + (3-j)] = temp;
        }
    }
}

