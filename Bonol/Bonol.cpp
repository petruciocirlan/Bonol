#include "Bonol.h"

Bonol::Bonol()
{
	board_ = new Board(kStartingSetup);
	isOver_ = false;
}

bool Bonol::Over()
{
	return isOver_;
}

bool Bonol::ValidPosition(Position pos)
{
	return
		(
		(0 <= pos.x && pos.x <= kBoardSize) &&
		(0 <= pos.y && pos.y <= kBoardSize)
		);
}

Bonol::cellValue& Bonol::cell(Position pos)
{
	/// if (!validPosition(pos)) throw something
	return board_->cell[pos.x][pos.y];
}

void Bonol::Draw()
{
	BonolDraw(this);
}

Bonol::Board::Board()
{
	memset(cell, 0, sizeof(cell));
}

Bonol::Board::Board(const cellValue copy[kBoardSize][kBoardSize])
{
	memcpy(cell, copy, sizeof(cell));
}