#include "utility.h"

// Converts board positions to touchscreen coordinates
void posToScreen(int x_pos, int y_pos, int* coords) {
	coords[0] = BOARD_MARGIN + y_pos*BOARD_SQUARE;
	coords[1] = BOARD_MARGIN + x_pos*BOARD_SQUARE;
}


// Coverts touchscreen coordinates to board positions
void screenToPos(int x_coord, int y_coord, int* position) {
	position[0] = floor((y_coord - 3) / 35);
	position[1] = floor((x_coord - 3) / 35);
}


// Draws a box around the position
void displayMoves(int x_pos, int y_pos) {
	int coords[2];
	posToScreen(x_pos, y_pos, coords);

	tft.drawRect(TFT_HEIGHT - BOARD_SQUARE - (coords[0] - PIECE_RADIUS), coords[1] - PIECE_RADIUS,
				 BOARD_SQUARE, BOARD_SQUARE, 0x83BF);
	tft.drawRect(TFT_HEIGHT - BOARD_SQUARE - (coords[0] - PIECE_RADIUS)+1, coords[1] - PIECE_RADIUS+1,
				 BOARD_SQUARE-2, BOARD_SQUARE-2, 0x83BF);

}