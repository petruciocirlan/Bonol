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

#include "GUI.h"

/// memset & memcpy
#include <cstring>

class GUI::Bonol
{
public:
	using CoordCell = INT;

	struct PosCell;
	struct Board;

	static const unsigned kBoardSize = 4;
	enum class Piece
	{
		UNUSED,
		FREE, RED, BLUE, BLOCKED,
		RED_SELECTED, BLUE_SELECTED, BLOCKED_SELECTED,
		BLOCKED_HIGHLIGHTED
	};

private:
	const Piece kStartingSetup[kBoardSize][kBoardSize] =
	{
		{Piece::BLOCKED,	Piece::RED,		Piece::RED,		Piece::FREE},
		{Piece::FREE,		Piece::BLUE,	Piece::RED,		Piece::FREE},
		{Piece::FREE,		Piece::BLUE,	Piece::RED,		Piece::FREE},
		{Piece::FREE,		Piece::BLUE,	Piece::BLUE,	Piece::BLOCKED}
	};

	const GUI& interface_;

	Board *const old_board_;
	Board *const update_board_;
	bool has_cell_updated_[kBoardSize][kBoardSize];

	bool is_over_;
	Piece active_player_piece_;

	bool IsValidPosition(const PosCell pos) const;
	static bool IsPlayerPiece(const Piece piece);
	bool IsActivePlayerPiece(const PosCell pos) const;

	void DrawCell(const PosCell pos) const;

public:
	Bonol(const GUI& gui);

	bool Over() const;
	void ChangePlayer();
	bool ValidateMove();
	void DrawTable();
	bool ValidateL();

	Piece GetActivePlayer() const;
	Piece GetActivePlayerSelected() const;
	bool IsFreeForActivePlayer(const PosCell pos) const;

	Piece GetCellPiece(const PosCell pos) const;
	PosCell GetCellFromGUI(const PointGUI pos) const;

	void InvalidateTable();
	void InvalidateCell(const PosCell pos);
	void InitiateUpdate();
	void UpdateCell(const PosCell pos, const Piece piece);

	void HighlightBlockedPieces();
	void DeHighlightBlockedPieces();

	void SetCellPiece(const PosCell pos, const Piece piece);
};

struct GUI::Bonol::PosCell
{
	CoordCell x, y;

	PosCell() : x(0), y(0) {};
	PosCell(CoordCell column, CoordCell row) : x(column), y(row) {};
	bool operator==(const PosCell& operand) const;
};

struct GUI::Bonol::Board
{
	Piece cell[kBoardSize][kBoardSize];

	Board();
	Board(const Piece copy_source[kBoardSize][kBoardSize]);
	void Clear();
	Piece& at(const PosCell pos);
};