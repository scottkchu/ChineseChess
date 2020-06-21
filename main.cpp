/*
	Just a simple example of how to use the touch screen.
	The coordinates that were touched are displayed on the screen.
*/

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>
#include <TouchScreen.h>
MCUFRIEND_kbv tft;
#include "includes.h"

#define SD_CS 10

// touch screen pins, obtained from the documentaion
#define YP A3 // must be an analog pin, use "An" notation!
#define XM A2 // must be an analog pin, use "An" notation!
#define YM 9  // can be a digital pin
#define XP 8  // can be a digital pin

// calibration data for the touch screen, obtained from documentation
// the minimum/maximum possible readings from the touch point
#define TS_MINX 100
#define TS_MINY 110
#define TS_MAXX 960
#define TS_MAXY 910

// thresholds to determine if there was a touch
#define MINPRESSURE   10
#define MAXPRESSURE 1000

// a multimeter reading says there are 300 ohms of resistance across the plate,
// so initialize with this to get more accurate readings
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);


void drawBoard() {
	tft.fillScreen(TFT_WHITE);
	// Board outline
	tft.fillRect(BOARD_MARGIN, BOARD_MARGIN, 8*BOARD_SQUARE, 9*BOARD_SQUARE, 0xFFEA);
	
	//Drawing board (vertical lines)
	uint16_t width = BOARD_MARGIN;
	for (int i = 0; i < 9; i++) {
		tft.drawLine(width, BOARD_MARGIN, width, TFT_WIDTH - (TFT_WIDTH - BOARD_SQUARE * 9 - BOARD_MARGIN),
					 TFT_BLACK);
		width += BOARD_SQUARE;
	}

	//Drawing board (horizontal lines)
	uint16_t height = BOARD_MARGIN;
	for (int i = 0; i < 10; i++) {
		tft.drawLine(BOARD_MARGIN, height, TFT_HEIGHT - BOARD_MARGIN, height, TFT_BLACK);
		height += BOARD_SQUARE;
	}

	//Making the River
	tft.fillRect(BOARD_MARGIN, BOARD_MARGIN + 4*BOARD_SQUARE,
				 TFT_HEIGHT - 2*BOARD_MARGIN+1, BOARD_SQUARE+1, 0xFFEA);
	tft.drawRect(BOARD_MARGIN, BOARD_MARGIN + 4*BOARD_SQUARE,
				 TFT_HEIGHT - 2*BOARD_MARGIN+1, BOARD_SQUARE+1, TFT_BLACK);

	// Making the Palace
    tft.drawLine(BOARD_MARGIN + 3*BOARD_SQUARE, BOARD_MARGIN,
    			 BOARD_MARGIN + 5*BOARD_SQUARE, BOARD_MARGIN + 2*BOARD_SQUARE, TFT_BLACK);
    tft.drawLine(BOARD_MARGIN + 5*BOARD_SQUARE, BOARD_MARGIN,
    			 BOARD_MARGIN + 3*BOARD_SQUARE, BOARD_MARGIN + 2*BOARD_SQUARE, TFT_BLACK);
    tft.drawLine(BOARD_MARGIN + 3*BOARD_SQUARE, BOARD_MARGIN + 7*BOARD_SQUARE,
    			 BOARD_MARGIN + 5*BOARD_SQUARE, BOARD_MARGIN + 9*BOARD_SQUARE, TFT_BLACK);
    tft.drawLine(BOARD_MARGIN + 5*BOARD_SQUARE, BOARD_MARGIN + 7*BOARD_SQUARE,
    			 BOARD_MARGIN + 3*BOARD_SQUARE, BOARD_MARGIN + 9*BOARD_SQUARE, TFT_BLACK);

    // If a square is occupied, draw the piece on the board
    for (int y = 0; y < 9; y++) {
    	for (int x = 0; x < 10; x++) {
    		if (board[y][x].isOccupied()) {
    			board[y][x].getPiece()->drawPiece();
    		}
    	}
    }
}


void setup() {
	init();

	// do not actually need to change the pinmode of the digital TouchScreen
	// pins, the function to get a touchscreen reading will set and change them

	Serial.begin(9600);

	uint16_t ID = tft.readID();
	tft.begin(ID);

	tft.setRotation(0);

	//Putting Pieces on the board;
	for (int i = 0; i < NUM_PIECES; i++) {
		int position[2];
		pieces[i]->getPosition(position);
		board[position[0]][position[1]].addPiece(pieces[i]);
	}

	drawBoard();
}


bool generalBlocked(int x_move, int y_move, Piece* piece) {
	int position[2];
	piece->getPosition(position);

	// Generals cannnot directly face each other
	if (piece->isRed()) {
		for (int x = position[1]-1; x >= 0; x--) {
			if (board[y_move][x].isOccupied()) {
				if (board[y_move][x].getPiece()->isGeneral()) {
					return true;
				}
				else {
					return false;
				}
			}
		}
	}
	else {
		for (int x = position[1]+1; x <= 9; x++) {
			if (board[y_move][x].isOccupied()) {
				if (board[y_move][x].getPiece()->isGeneral()) {
					return true;
				}
				else {
					return false;
				}
			}
		}
	}
	return false;
}


bool elephantBlocked(int x_move, int y_move, Piece* piece) {
	int position[2];
	piece->getPosition(position);

	// Negative is right/down, positive is left/up
	int x_dif = position[1] - x_move;
	int y_dif = position[0] - y_move;
	// Check the square one away diagonally
	if (board[position[0] - y_dif/2][position[1] - x_dif/2].isOccupied()) {
		return true;
	}
	return false;
}


bool horseBlocked(int x_move, int y_move, Piece* piece) {
	int position[2];
	piece->getPosition(position);

	// Negative is right/down, positive is left/up
	int x_dif = position[1] - x_move;
	int y_dif = position[0] - y_move;
	// Check the square one away in the longer direction
	if (abs(x_dif) > abs(y_dif)) {
		if (board[position[0]][position[1] - x_dif/2].isOccupied()) {
			return true;
		}
	}
	else {
		if (board[position[0] - y_dif/2][position[1]].isOccupied()) {
			return true;
		}
	}
	return false;
}


bool chariotBlocked(int x_move, int y_move, Piece* piece) {
	int position[2];
	piece->getPosition(position);

	// Check if there is a piece inbetween
	if (x_move > position[1]) {
		for (int x = position[1]+1; x <= x_move-1; x++) {
			if (board[position[0]][x].isOccupied()) {
				return true;
			}
		}
	}
	else if (x_move < position[1]) {
		for (int x = position[1]-1; x >= x_move+1; x--) {
			if (board[position[0]][x].isOccupied()) {
				return true;
			}
		}
	}
	else if (y_move > position[0]) {
		for (int y = position[0]+1; y <= y_move-1; y++) {
			if (board[y][position[1]].isOccupied()) {
				return true;
			}
		}
	}
	else if (y_move < position[0]) {
		for (int y = position[0]-1; y >= y_move+1; y--) {
			if (board[y][position[1]].isOccupied()) {
				return true;
			}
		}
	}
	return false;
}


bool cannonBlocked(int x_move, int y_move, Piece* piece) {
	int position[2];
	piece->getPosition(position);

	// Checks if there if a piece inbetween
	// Or if we can skip a piece to capture a piece on the other side
	int skip = 0;
	if (x_move > position[1]) {
		for (int x = position[1]+1; x <= x_move; x++) {
			if (board[position[0]][x].isOccupied()) {
				skip++;
			}
		}
	}
	else if (x_move < position[1]) {
		for (int x = position[1]-1; x >= x_move; x--) {
			if (board[position[0]][x].isOccupied()) {
				skip++;
			}
		}
	}
	else if (y_move > position[0]) {
		for (int y = position[0]+1; y <= y_move; y++) {
			if (board[y][position[1]].isOccupied()) {
				skip++;
			}
		}
	}
	else if (y_move < position[0]) {
		for (int y = position[0]-1; y >= y_move; y--) {
			if (board[y][position[1]].isOccupied()) {
				skip++;
			}
		}
	}

	if (skip == 0 ||
	   (skip == 2 && board[y_move][x_move].isOccupied())) {
		return false;
	}
	return true;
}


bool generalsFacing(int x_move, int y_move, Piece* piece) {
	int position[2];
	piece->getPosition(position);

	bool between = false;
	int pieceBetween = 0;
	// Checks if the piece is the only piece between the two generals
	if (y_move != position[0]) {
		for (int x = 0; x <= 9; x++) {
			if (board[position[0]][x].isOccupied()) {
				if (board[position[0]][x].getPiece()->isGeneral()) {
					between = !between;
				}
				else {
					if (between) {
						pieceBetween++;
					}
				}
			}
		}

		if (!between && pieceBetween == 1) {
			return true;
		}
	}
	return false;
}



bool pieceSelect(int* position) {
	TSPoint touch = ts.getPoint();

	// restore pinMode to output after reading the touch
    // this is necessary to talk to tft display
	pinMode(YP, OUTPUT); 
	pinMode(XM, OUTPUT); 

	if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {
		return false;
	}

	int16_t x_coord = map(touch.y, TS_MINX, TS_MAXX, TFT_WIDTH-1, 0);
	int16_t y_coord = map(touch.x, TS_MINY, TS_MAXY, TFT_HEIGHT-1, 0);
	// Converts touchscreen coordinates to board positions
	screenToPos(x_coord, y_coord, position);

	if (board[position[0]][position[1]].isOccupied()) {
		Piece* piece = board[position[0]][position[1]].getPiece();
		// Can only select the same color as turn
		if (piece->isRed() == redTurn) {
			Serial.print("Position Selected: ");
			Serial.print(position[0]);
			Serial.print(", ");
			Serial.println(position[1]);

			// Iterate through all possible moves (end positions)
			int possibleMoves = 0;
			for (int y = 0; y < 9; y++) {
				for (int x = 0; x < 10; x++) {
					// Determines if the move if valid
					if (piece->movePiece(x, y) &&
					  !(!piece->isGeneral() && generalsFacing(x, y, piece)) &&
					  !(piece->isGeneral() && generalBlocked(x, y, piece)) &&
					  !(piece->isElephant() && elephantBlocked(x, y, piece)) &&
					  !(piece->isHorse() && horseBlocked(x, y, piece)) &&
					  !(piece->isChariot() && chariotBlocked(x, y, piece)) &&
					  !(piece->isCannon() && cannonBlocked(x, y, piece))) {
						if (board[y][x].isOccupied()) {
							// Cannot move to square with same colored piece
							if (piece->isRed() != board[y][x].getPiece()->isRed()) {
								Serial.print("Possible Move: ");
								Serial.print(y);
								Serial.print(", ");
								Serial.println(x);

								displayMoves(x, y);
								possibleMoves++;
							}
						}
						else {
							Serial.print("Possible Move: ");
							Serial.print(y);
							Serial.print(", ");
							Serial.println(x);

							displayMoves(x, y);
							possibleMoves++;
						}
					}
				}
			}

			delay(200);
			if (possibleMoves > 0) {
				return true;
			}
			else {
				tft.setTextSize(2);
				tft.setCursor(TFT_HEIGHT/2-96, 7*TFT_WIDTH/8-16);
				tft.setTextColor(TFT_BLACK);
				tft.print("No Possible Moves");
				return false;
			}
		}
	}
	delay(200);
	return false;
}


bool pieceMove(int* selectPos, int* movePos) {
	TSPoint touch = ts.getPoint();

	// restore pinMode to output after reading the touch
    // this is necessary to talk to tft display
	pinMode(YP, OUTPUT);
	pinMode(XM, OUTPUT);

	if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {
		return false;
	}

	int16_t x_coord = map(touch.y, TS_MINX, TS_MAXX, TFT_WIDTH-1, 0);
	int16_t y_coord = map(touch.x, TS_MINY, TS_MAXY, TFT_HEIGHT-1, 0);
	// Converts touchscreen coordinates to board positions
	screenToPos(x_coord, y_coord, movePos);

	Piece* piece = board[selectPos[0]][selectPos[1]].getPiece();

	// Determines if the move is valid from the selected piece
	if (piece->movePiece(movePos[1], movePos[0]) &&
	  !(!piece->isGeneral() && generalsFacing(movePos[1], movePos[0], piece)) &&
	  !(piece->isGeneral() && generalBlocked(movePos[1], movePos[0], piece)) &&
	  !(piece->isElephant() && elephantBlocked(movePos[1], movePos[0], piece)) &&
	  !(piece->isHorse() && horseBlocked(movePos[1], movePos[0], piece)) &&
	  !(piece->isChariot() && chariotBlocked(movePos[1], movePos[0], piece)) &&
	  !(piece->isCannon() && cannonBlocked(movePos[1], movePos[0], piece))) {
		if (board[movePos[0]][movePos[1]].isOccupied()) {
			Piece* capturedPiece = board[movePos[0]][movePos[1]].getPiece();
			// Cannot move to the square with same colored piece
			if (piece->isRed() != capturedPiece->isRed()) {
				Serial.print("Moving To: ");
				Serial.print(movePos[0]);
				Serial.print(", ");
				Serial.println(movePos[1]);

				// Remove piece from old positions and add it to the new position
				// Also remove captured pieces
				board[selectPos[0]][selectPos[1]].removePiece();
				board[movePos[0]][movePos[1]].removePiece();
				board[movePos[0]][movePos[1]].addPiece(piece);
				piece->setPosition(movePos[1], movePos[0]);

				drawBoard();

				if (capturedPiece->isGeneral()) {
					gameEnd = true;

					tft.setTextSize(3);
					if (capturedPiece->isRed()) {
						tft.setCursor(TFT_HEIGHT/2-90, 7*TFT_WIDTH/8-24);
						tft.setTextColor(TFT_BLACK);
						tft.print("BLACK WINS!");
					}
					else {
						tft.setCursor(TFT_HEIGHT/2-72, 7*TFT_WIDTH/8-24);
						tft.setTextColor(TFT_RED);
						tft.print("RED WINS!");
					}
				}

				delay(200);
				return true;
			}
		}
		else {
			Serial.print("Moving To: ");
			Serial.print(movePos[0]);
			Serial.print(", ");
			Serial.println(movePos[1]);

			// Remove piece from old positions and add it to the new position
			board[selectPos[0]][selectPos[1]].removePiece();
			board[movePos[0]][movePos[1]].addPiece(piece);
			piece->setPosition(movePos[1], movePos[0]);

			drawBoard();

			delay(200);
			return true;
		}
	}

	delay(200);
	return false;
}


int main() {
	setup();

	enum State {select, move};
	State state = select;
	int selectPos[2];
	int movePos[2];

	while (true) {
		if (state == select) {
			if (pieceSelect(selectPos)) {
				state = move;
				Serial.println("State Changed to Move");
			}
		}
		else if (state == move) {
			if (pieceMove(selectPos, movePos)) {
				if (!gameEnd) {
					redTurn = !redTurn;
					state = select;
					Serial.println("State Changed to Select");
				}
				else {
					break;
				}
			}
		}
	}

	Serial.end();

	return 0;
}
