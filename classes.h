#ifndef __CLASSES_H
#define __CLASSES_H


#include <Arduino.h>
#include <MCUFRIEND_kbv.h>

extern MCUFRIEND_kbv tft;

class Piece {
public:
	Piece(int y_pos, int x_pos, bool side);
	virtual void drawPiece();
	virtual bool movePiece(int x_move, int y_move);
	void getPosition(int* position);
	virtual void setPosition(int x_pos, int y_pos);
	virtual bool isGeneral();
	virtual bool isElephant();
	virtual bool isHorse();
	virtual bool isChariot();
	virtual bool isCannon();
	bool isRed();
protected:
	int y_pos, x_pos;
	bool side;
};


class BoardPoint {
public:
	BoardPoint();
	Piece* getPiece();
	void addPiece(Piece* piece);
	void removePiece();
	bool isOccupied();
private:
	bool occupied;
	Piece* piece;
};


class Chariot : public Piece {
public:
	Chariot(int y_pos, int x_pos, bool side);
	virtual bool movePiece(int x_move, int y_move);
	virtual void drawPiece();
	virtual bool isChariot();
};


class Horse : public Piece {
public:
	Horse(int y_pos, int x_pos, bool side);
	virtual bool movePiece(int x_move, int y_move);
	virtual void drawPiece();
	virtual bool isHorse();
};


class Elephant : public Piece {
public:
	Elephant(int y_pos, int x_pos, bool side);
	virtual bool movePiece(int x_move, int y_move);
	virtual void drawPiece();
	virtual bool isElephant();
};


class Advisor : public Piece {
public:
	Advisor(int y_pos, int x_pos, bool side);
	virtual bool movePiece(int x_move, int y_move);
	virtual void drawPiece();
};


class General : public Piece {
public:
	General(int y_pos, int x_pos, bool side);
	virtual bool movePiece(int x_move, int y_move);
	virtual void drawPiece();
	virtual bool isGeneral();
};


class Soldier : public Piece {
public:
	Soldier(int y_pos, int x_pos, bool side);
	virtual void setPosition(int x_pos, int y_pos);
	virtual bool movePiece(int x_move, int y_move);
	virtual void drawPiece();
private:
	bool crossedRiver;
};


class Cannon : public Piece {
public:
	Cannon(int y_pos, int x_pos, bool side);
	virtual bool movePiece(int x_move, int y_move);
	virtual void drawPiece();
	virtual bool isCannon();
};


#endif