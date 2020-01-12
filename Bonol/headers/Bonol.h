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

	const GUI *interface_;

	Board *old_board_;
	Board *update_board_;
	bool has_cell_updated_[kBoardSize][kBoardSize];

	bool is_over_;
	Piece active_player_piece_;
	bool turn_move_piece_, turn_move_block_;

public:

	Bonol(const GUI &gui);
	Bonol(const GUI &gui, const Bonol &copy);
	//Bonol operator=(const Bonol& assign);
	//~Bonol();

	/// checks
	bool IsOver() const;
	bool CheckOver();
	bool IsValidPosition(const PosCell pos) const;
	bool IsPlayerPiece(const Piece piece) const;
	bool IsActivePlayerPiece(const PosCell pos) const;
	bool IsFreeForActivePlayer(const PosCell cell) const;
	bool IsSelected(const PosCell cell) const;

	/// data access
	Piece GetActivePlayer() const;
	Piece GetActivePlayerSelected() const;
	short GetActivePlayerName() const;

	Piece GetCellPiece(const PosCell cell) const;
	Piece GetUpdateCellPiece(const PosCell cell) const;

	PosCell GetCellFromGUI(const PointGUI pos) const;
	PointGUI GetGUIFromCell(const PosCell cell) const;

	/// validate turn
	bool ValidateMove();
	bool ValidateL();
	short SearchOnRows();
	short SearchOnColumns();
	short unsigned CountMoves(short unsigned column, short unsigned row);
	unsigned HowManyPossibleMoves();
	bool CheckGoodForm(short unsigned ToCheck ,short unsigned ArrayToSearch[4]);

	// computer turn moves
	void FindPcMove(String WhichMove);
	void DeleteCurrentPlayerPieces();
	void DeleteFirstRock();
	

	/// update state for GUI interaction
	void ApplyMove();

	bool& MovePieceTurn();
	bool& MoveBlockTurn();

	void InvalidateTable();
	void InvalidateCell(const PosCell cell);
	void ClearUpdate();
	void UpdateCell(const PosCell cell, const Piece piece);
	void RemoveUpdateCell(const PosCell cell);
	bool IsUpdateSelectionConnected() const;

	void HighlightBlockedPieces();
	void DeHighlightBlockedPieces();

	void DrawTable();
	void SetCellPiece(const PosCell cell, const Piece piece);
	void ChangePlayer();
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
	Board(const Board &copy);
	Board operator=(const Board &assign);
	void Clear();
	Piece& at(const PosCell pos);
};
