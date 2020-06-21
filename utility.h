#ifndef __UTILITY_H
#define __UTILITY_H

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>

extern MCUFRIEND_kbv tft;

// Constant defintions
#define TFT_WIDTH  480
#define TFT_HEIGHT 320
#define BOARD_SQUARE 35
#define BOARD_MARGIN 20
#define NUM_PIECES 32
#define PIECE_RADIUS 17


void posToScreen(int x_pos, int y_pos, int* coords);

void screenToPos(int x_coord, int y_coord, int* position);
	
void displayMoves(int x_pos, int y_pos);

#endif