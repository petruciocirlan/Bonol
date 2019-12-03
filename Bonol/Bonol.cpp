#include "Bonol.h"
#include "Bonol_draw.cpp"

Bonol::Bonol()
{
	for (cellCoord i = 0; i < BOARD_SIZE; ++i)
		for (cellCoord j = 0; j < BOARD_SIZE; ++j)
			cell(Position(i, j)) = 0;
	/// initialize stuff
}

bool Bonol::over()
{
	return _isOver;
}

bool Bonol::validPosition(Position pos)
{
	return ((0 <= pos.x && pos.x <= BOARD_SIZE) &&
		(0 <= pos.y && pos.y <= BOARD_SIZE));
}

Bonol::cellValue& Bonol::cell(Position pos)
{
	/// if (!validPosition(pos)) throw something
	return _board[pos.x][pos.y];
}

void Bonol::draw()
{
	Bonol_draw(this);
}