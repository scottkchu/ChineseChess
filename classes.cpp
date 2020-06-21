#include "classes.h"
#include "utility.h"

Piece::Piece(int y_pos, int x_pos, bool side): 
	y_pos(y_pos),
	x_pos(x_pos),
	side(side)
{}


Chariot::Chariot(int y_pos, int x_pos, bool side):
	Piece(y_pos, x_pos, side)	
{}


Horse::Horse(int y_pos, int x_pos, bool side):
	Piece(y_pos, x_pos, side)	
{}


Elephant::Elephant(int y_pos, int x_pos, bool side):
	Piece(y_pos, x_pos, side)	
{}


Advisor::Advisor(int y_pos, int x_pos, bool side):
	Piece(y_pos, x_pos, side)	
{}


General::General(int y_pos, int x_pos, bool side):
	Piece(y_pos, x_pos, side)	
{}


Cannon::Cannon(int y_pos, int x_pos, bool side):
	Piece(y_pos, x_pos, side)	
{}


Soldier::Soldier(int y_pos, int x_pos, bool side): 
	Piece(y_pos, x_pos, side),
	crossedRiver(false)
{}


BoardPoint::BoardPoint():
	occupied(false),
	piece(NULL)
{}


void Piece::getPosition(int* position) {
	position[0] = y_pos;
	position[1] = x_pos;
}


void Piece::setPosition(int x_pos, int y_pos) {
	this->x_pos = x_pos;
	this->y_pos = y_pos;
}


void Soldier::setPosition(int x_pos, int y_pos) {
	Piece::setPosition(x_pos, y_pos);
	if ((isRed() && x_pos <= 4) || (!isRed() && x_pos >= 5)) {
		crossedRiver = true;
	}
}


bool Piece::isRed() {
	return side;
}


bool BoardPoint::isOccupied() {
	return occupied;
}


void BoardPoint::addPiece(Piece* piece) {
	this->piece = piece;
	occupied = true;
}


Piece* BoardPoint::getPiece() {
	return piece;
}


void BoardPoint::removePiece() {
	piece = NULL;
	occupied = false;
}


bool Piece::isGeneral() {
	return false;
}


bool Piece::isElephant() {
	return false;
}


bool Piece::isHorse() {
	return false;
}


bool Piece::isChariot() {
	return false;
}


bool Piece::isCannon() {
	return false;
}


bool General::isGeneral() {
	return true;
}


bool Elephant::isElephant() {
	return true;
}


bool Horse::isHorse() {
	return true;
}


bool Chariot::isChariot() {
	return true;
}


bool Cannon::isCannon() {
	return true;
}


// Horizontal and vertical squares in the same row/column
bool Chariot::movePiece(int x_move, int y_move) {
	if (x_move <= 9 && y_move <= 8) {
		if ((x_pos == x_move) != (y_pos == y_move)) {
			return true;
		}
	}
	return false;
}


// Squares that are two away in one direction and one away in another
bool Horse::movePiece(int x_move, int y_move) {
	if (x_move <= 9 && y_move <= 8) {
		if ((abs(x_move - x_pos) == 2 && abs(y_move - y_pos) == 1) ||
			(abs(x_move - x_pos) == 1 && abs(y_move - y_pos) == 2)) {
			return true;
		}
	}
	return false;
}


// Squares that are two away diagonally
bool Elephant::movePiece(int x_move, int y_move) {
	if ((x_move >= 5 && x_move <= 9 && y_move <= 8 && isRed()) ||
		(x_move >= 0 && x_move <= 4 && y_move <= 8 && !isRed())) {
		if (abs(x_move - x_pos) == 2 && abs(y_move - y_pos) == 2) {
			return true;
		}
	}
	return false;
}


// Squares that are one away diagonally
bool Advisor::movePiece(int x_move, int y_move) {
	if ((x_move >= 7 && x_move <= 9 && y_move >= 3 && y_move <= 5 && isRed()) ||
		(x_move >= 0 && x_move <= 2 && y_move >= 3 && y_move <= 5 && !isRed())) {
		if (abs(x_move - x_pos) == 1 && abs(y_move - y_pos) == 1) {
			return true;
		}
	}
	return false;
}


// Squares that are one away horizontally or vertically
bool General::movePiece(int x_move, int y_move) {
	if ((x_move >= 7 && x_move <= 9 && y_move >= 3 && y_move <= 5 && isRed()) ||
		(x_move >= 0 && x_move <= 2 && y_move >= 3 && y_move <= 5 && !isRed())) {
		if ((abs(x_move - x_pos) == 0 && abs(y_move - y_pos) == 1) ||
			(abs(x_move - x_pos) == 1 && abs(y_move - y_pos) == 0)) {
			return true;
		}
	}
	return false;
}


// Squares that are one away horizontally or vertically
// Cannot move backwards
// Cannot move sideways prior to crossing the river
bool Soldier::movePiece(int x_move, int y_move) {
	if (x_move <= 9 && y_move <= 8) {
		if (crossedRiver) {
			if (isRed()) {
				if ((x_move - x_pos == -1 && y_move == y_pos) ||
					(x_move == x_pos && abs(y_move - y_pos) == 1)) {
					return true;
				}
			}
			else {
				if ((x_move - x_pos == 1 && y_move == y_pos) ||
					(x_move == x_pos && abs(y_move - y_pos) == 1)) {
					return true;
				}
			}
		}
		else {
			if (isRed()) {
				if (x_move - x_pos == -1 && y_move == y_pos) {
					return true;
				}
			}
			else {
				if (x_move - x_pos == 1 && y_move == y_pos) {
					return true;
				}
			}
		}
	}
	return false;
}


// Horizontal and vertical squares in the same row/column
// Can skip one piece and capture piece on other side
bool Cannon::movePiece(int x_move, int y_move) {
	if (x_move <= 9 && y_move <= 8) {
		if ((x_pos == x_move) != (y_pos == y_move)) {
			return true;
		}
	}
	return false;
}


// Draws the outer circles of the piece
void Piece::drawPiece() {
	int coords[2];
	posToScreen(x_pos, y_pos, coords);

	if (isRed()) {
		tft.fillCircle(TFT_HEIGHT - coords[0], coords[1], PIECE_RADIUS, 0xFFFA);
		tft.fillCircle(TFT_HEIGHT - coords[0], coords[1], PIECE_RADIUS-3, TFT_RED);
		tft.fillCircle(TFT_HEIGHT - coords[0], coords[1], PIECE_RADIUS-5, 0xFFFA);
		tft.drawCircle(TFT_HEIGHT - coords[0], coords[1], PIECE_RADIUS, TFT_RED);
	}
	else {
		tft.fillCircle(TFT_HEIGHT - coords[0], coords[1], PIECE_RADIUS, 0xFFFA);
		tft.fillCircle(TFT_HEIGHT - coords[0], coords[1], PIECE_RADIUS-3, TFT_BLACK);
		tft.fillCircle(TFT_HEIGHT - coords[0], coords[1], PIECE_RADIUS-5, 0xFFFA);
		tft.drawCircle(TFT_HEIGHT - coords[0], coords[1], PIECE_RADIUS, TFT_BLACK);
	}
}


// Draws the inner characters
void Chariot::drawPiece() {
	Piece::drawPiece();

	int coords[2];
	posToScreen(x_pos, y_pos, coords);
	if (isRed()) {
		tft.drawChar(TFT_HEIGHT - (coords[0])-4, coords[1]-6, 'R', TFT_RED, 0xFFFA, 2);
	}
	else {
		tft.drawChar(TFT_HEIGHT - (coords[0])-4, coords[1]-6, 'R', TFT_BLACK, 0xFFFA, 2);
	}
}


void Horse::drawPiece() {
	Piece::drawPiece();

	int coords[2];
	posToScreen(x_pos, y_pos, coords);
	if (isRed()) {
		tft.drawChar(TFT_HEIGHT - (coords[0])-4, coords[1]-6, 'H', TFT_RED, 0xFFFA, 2);
	}
	else {
		tft.drawChar(TFT_HEIGHT - (coords[0])-4, coords[1]-6, 'H', TFT_BLACK, 0xFFFA, 2);
	}
}


void Elephant::drawPiece() {
	Piece::drawPiece();

	int coords[2];
	posToScreen(x_pos, y_pos, coords);
	if (isRed()) {
		tft.drawChar(TFT_HEIGHT - (coords[0])-4, coords[1]-6, 'E', TFT_RED, 0xFFFA, 2);
	}
	else {
		tft.drawChar(TFT_HEIGHT - (coords[0])-4, coords[1]-6, 'E', TFT_BLACK, 0xFFFA, 2);
	}
}


void Advisor::drawPiece() {
	Piece::drawPiece();

	int coords[2];
	posToScreen(x_pos, y_pos, coords);
	if (isRed()) {
		tft.drawChar(TFT_HEIGHT - (coords[0])-4, coords[1]-6, 'A', TFT_RED, 0xFFFA, 2);
	}
	else {
		tft.drawChar(TFT_HEIGHT - (coords[0])-4, coords[1]-6, 'A', TFT_BLACK, 0xFFFA, 2);
	}
}


void General::drawPiece() {
	Piece::drawPiece();

	int coords[2];
	posToScreen(x_pos, y_pos, coords);
	if (isRed()) {
		tft.drawChar(TFT_HEIGHT - (coords[0])-4, coords[1]-6, 'G', TFT_RED, 0xFFFA, 2);
	}
	else {
		tft.drawChar(TFT_HEIGHT - (coords[0])-4, coords[1]-6, 'G', TFT_BLACK, 0xFFFA, 2);
	}
}


void Soldier::drawPiece() {
	Piece::drawPiece();

	int coords[2];
	posToScreen(x_pos, y_pos, coords);
	if (isRed()) {
		tft.drawChar(TFT_HEIGHT - (coords[0])-4, coords[1]-6, 'S', TFT_RED, 0xFFFA, 2);
	}
	else {
		tft.drawChar(TFT_HEIGHT - (coords[0])-4, coords[1]-6, 'S', TFT_BLACK, 0xFFFA, 2);
	}
}


void Cannon::drawPiece() {
	Piece::drawPiece();

	int coords[2];
	posToScreen(x_pos, y_pos, coords);
	if (isRed()) {
		tft.drawChar(TFT_HEIGHT - (coords[0])-4, coords[1]-6, 'C', TFT_RED, 0xFFFA, 2);
	}
	else {
		tft.drawChar(TFT_HEIGHT - (coords[0])-4, coords[1]-6, 'C', TFT_BLACK, 0xFFFA, 2);
	}
}


