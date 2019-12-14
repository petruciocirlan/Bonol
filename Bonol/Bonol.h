/*
	File: Bonol.h
	Written by:
		Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
		Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
	CS1105: Practica - Introducere in programare
	Faculty of Computer Science, UAIC, WINTER 2019

	ABOUT THIS FILE:
	Interface of the Bonol (L game).
*/

#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <windowsx.h>
/// memset & memcpy
#include <cstring>

class Bonol
{
private:
	using CellValue = int;
	using CellCoord = int;

	struct PosCell;
	struct Board;
	class GUI;

	static const unsigned kBoardSize = 4;
	enum class Piece
	{
		UNUSED,
		FREE, RED, BLUE, BLOCKED,
		RED_SELECTED, BLUE_SELECTED, BLOCKED_SELECTED
	};
	const Piece kStartingSetup[kBoardSize][kBoardSize] =
	{
		{Piece::BLOCKED,	Piece::RED,		Piece::RED,		Piece::FREE},
		{Piece::FREE,		Piece::BLUE,	Piece::RED,		Piece::FREE},
		{Piece::FREE,		Piece::BLUE,	Piece::RED,		Piece::FREE},
		{Piece::FREE,		Piece::BLUE,	Piece::BLUE,	Piece::BLOCKED}
	};

	Board* const board_;
	GUI* interface_;

	bool is_over_;
	Piece active_player_piece_;

	Piece& GetCellPiece(const PosCell pos) const;
	bool IsValidPosition(const PosCell pos) const;
	bool IsPlayerPiece(const PosCell pos) const;
	bool IsActivePlayerPiece(const PosCell pos) const;
	Piece GetActivePlayerSelectedPiece() const;

	void ValidateMove(Board& new_state);

public:
	using Dimensions = PosCell;

	Bonol(const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow);

	bool Over() const;
	void ChangePlayer();
	Piece GetActivePlayer() const;
};

struct Bonol::PosCell
{
	CellCoord x, y;

	PosCell() : x(0), y(0) {};
	PosCell(CellCoord column, CellCoord row) : x(column), y(row) {};
};

struct Bonol::Board
{
	Piece cell[kBoardSize][kBoardSize];

	Board();
	Board(const Piece copy_source[kBoardSize][kBoardSize]);
	void Clear();
	Piece& at(const PosCell pos);
};

#include "GUI.h"