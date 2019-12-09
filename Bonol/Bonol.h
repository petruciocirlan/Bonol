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
/// memset & memcpy
#include <cstring>

class Bonol
{
private:
	using CellValue = int;
	using CellCoord = int;

	struct CellPos;
	struct Board;
	class GUI;

	static const unsigned kBoardSize = 4;
	enum class Piece
	{
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
	enum class Player { RED, BLUE };

	Board* const board_;
	GUI* const interface_;

	bool isOver_;
	Player active_player_;

	Piece& GetCellPiece(const CellPos pos) const;
	bool IsValidPosition(const CellPos pos) const;
	bool IsPlayerPiece(const CellPos pos) const;
	bool IsActivePlayerPiece(const CellPos pos) const;

public:
	using Dimensions = CellPos;

	Bonol(const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow);

	bool Over() const;
	void ChangePlayer();
	Player GetActivePlayer() const;
};

struct Bonol::CellPos
{
	CellCoord x, y;

	CellPos() : x(0), y(0) {};
	CellPos(CellCoord column, CellCoord row) : x(column), y(row) {};
};

struct Bonol::Board
{
	Piece cell[kBoardSize][kBoardSize];

	Board();
	Board(const Piece copy_source[kBoardSize][kBoardSize]);
};

#include "GUI.h"