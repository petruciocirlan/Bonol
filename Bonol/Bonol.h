/*
	File: Bonol.h
	Written by:
		Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
		Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
	CS1105: Practica - Introducere in programare
	Faculty of Computer Science, UAIC, WINTER 2019

	This header contains the interface of the Bonol game class
*/

#pragma once

// graphics
#include "graphics.h"
#include "winbgim.h"

/// memset & memcpy
#include <cstring>

class Bonol
{
private:
	using cellValue = int;
	using cellCoord = int;
	struct Position;
	struct Board;

	static const unsigned kBoardSize = 4;
	enum Pieces { PIECE_FREE, PIECE_RED, PIECE_BLUE, PIECE_BLOCKED };
	const cellValue kStartingSetup[kBoardSize][kBoardSize] =
	{
		{PIECE_BLOCKED,	PIECE_RED,	PIECE_RED,	PIECE_FREE},
		{PIECE_FREE,	PIECE_BLUE,	PIECE_RED,	PIECE_FREE},
		{PIECE_FREE,	PIECE_BLUE,	PIECE_RED,	PIECE_FREE},
		{PIECE_FREE,	PIECE_BLUE,	PIECE_BLUE,	PIECE_BLOCKED}
	};

	bool isOver_;
	Board *board_;

	cellValue& cell(Position);
	bool ValidPosition(Position);
	void BonolDraw(Bonol* game);
public:
	Bonol();

	bool Over();

	void Draw();
};

struct Bonol::Position
{
	cellCoord x, y;
	Position(cellCoord _x, cellCoord _y) : x(_x), y(_y) {};
};

struct Bonol::Board
{
	cellValue cell[kBoardSize][kBoardSize];
	Board();
	Board(const cellValue copy[kBoardSize][kBoardSize]);
};
