#pragma once

class Bonol
{
public:
	using cellValue = int;
	using cellCoord = int;
	struct Position;
private:
	static const unsigned BOARD_SIZE = 4;
	cellValue _board[BOARD_SIZE][BOARD_SIZE] = { 0 };
	bool _isOver = false;

	cellValue& cell(Position);
	bool validPosition(Position);
public:
	Bonol();

	bool over();

	void draw();
};

struct Bonol::Position
{
	cellCoord x, y;
	Position(cellCoord _x, cellCoord _y) : x(_x), y(_y) {};
};
