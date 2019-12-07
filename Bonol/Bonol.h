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

// graphics
#include "graphics.h"
#include "winbgim.h"

/// memset & memcpy
#include <cstring>

class Bonol
{
private:
	using CellValue = int;
	using CellCoord = int;

	struct	Position;
	struct	Board;
	class	GUI;

	static const unsigned kBoardSize = 4;
	enum Pieces	{ PIECE_FREE, PIECE_RED, PIECE_BLUE, PIECE_BLOCKED };
	const CellValue kStartingSetup[kBoardSize][kBoardSize] =
	{
		{PIECE_BLOCKED,	PIECE_RED,	PIECE_RED,	PIECE_FREE},
		{PIECE_FREE,	PIECE_BLUE,	PIECE_RED,	PIECE_FREE},
		{PIECE_FREE,	PIECE_BLUE,	PIECE_RED,	PIECE_FREE},
		{PIECE_FREE,	PIECE_BLUE,	PIECE_BLUE,	PIECE_BLOCKED}
	};
	enum Players { PLAYER_RED, PLAYER_BLUE };

	Board*	board_;
	bool	isOver_;
	GUI*	interface_;
	int		active_player_;


	CellValue&	GetCell			(const Position pos)	const;
	bool		IsValidPosition	(const Position pos)	const;
	bool		IsPlayerPiece	(const Position pos)	const;

public:
	using Dimensions = Position;

	Bonol(const Dimensions dim);

	bool Over()			const;
	void UpdateGUI()	const;
	void ChangePlayer();

};

struct Bonol::Position
{
	CellCoord x, y;

	Position() : x(0), y(0) {};
	Position(CellCoord column, CellCoord line) : x(column), y(line) {};
};

struct Bonol::Board
{
	CellValue cell[kBoardSize][kBoardSize];

	Board();
	Board(const CellValue copy[kBoardSize][kBoardSize]);
};

#include "GUI.h"