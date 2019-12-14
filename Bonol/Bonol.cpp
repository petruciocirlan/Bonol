/*
	File: Bonol.cpp
	Written by:
		Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
		Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
	CS1105: Practica - Introducere in programare
	Faculty of Computer Science, UAIC, WINTER 2019

	ABOUT THIS FILE:
	Implementation of the mechanics of the Bonol (L game).
*/

#include "Bonol.h"

Bonol::Piece& Bonol::GetCellPiece(const PosCell pos)
{
	if (IsValidPosition(pos))
	{
		return board_->at(pos);
	}
	else
	{
		return inaccessible_;
	}
}

bool Bonol::IsValidPosition(const PosCell pos) const
{
	return ((0 <= pos.x && pos.x <= kBoardSize) &&
			(0 <= pos.y && pos.y <= kBoardSize));
}

bool Bonol::IsPlayerPiece(const Piece piece)
{
	return (piece == Piece::RED || piece == Piece::RED_SELECTED ||
		    piece == Piece::BLUE || piece == Piece::BLUE_SELECTED);
}

bool Bonol::IsActivePlayerPiece(const PosCell pos)
{
	return GetCellPiece(pos) == GetActivePlayer();
}

Bonol::Piece Bonol::GetActivePlayerSelectedPiece() const
{
	if (GetActivePlayer() == Piece::RED) return Piece::RED_SELECTED;
	else /*(GetActivePlayer() == Player::BLUE)*/ return Piece::BLUE_SELECTED;
}

bool Bonol::IsFreeForActivePlayer(const PosCell pos)
{
	return GetCellPiece(pos) == Piece::FREE || IsActivePlayerPiece(pos);
}

void Bonol::ValidateMove(Board& new_state)
{
	/// TODO(vali): check if new_state is a valid move
	/// notes: check if the pieces form an L
	/// there are only 2 pieces in new_state:
	/// Piece::UNUSED and Piece::RED_SELECTED
	/// (or Piece::BLUE_SELECTED depending on active player)

	/// if the move is valid, update the current board state with the new one
	/// and change the current player ( ChangePlayer() )

	/// later: instead of changing the current player,
	/// enter the "move one block" state
}

Bonol::Bonol(const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow)
	: board_(new Board(kStartingSetup))
{
	inaccessible_ = Piece::FORBIDDEN;
	is_over_ = false;
	active_player_piece_ = Piece::RED;
	interface_ = new GUI(this, window_dimensions, hInstance, nCmdShow);
}

bool Bonol::Over() const
{
	return is_over_;
}

void Bonol::ChangePlayer()
{
	if (active_player_piece_ == Piece::RED)
		active_player_piece_ = Piece::BLUE;
	else
		active_player_piece_ = Piece::RED;
}

Bonol::Piece Bonol::GetActivePlayer() const
{
	return active_player_piece_;
}

Bonol::Board::Board()
{
	memset(cell, 0, sizeof(cell));
	for (int line = 0; line < kBoardSize; ++line)
		for (int column = 0; column < kBoardSize; ++column)
			cell[line][column] = Piece::UNUSED;
}

Bonol::Board::Board(const Piece copy_source[kBoardSize][kBoardSize])
{
	memset(cell, 0, sizeof(cell));
	for (int line = 0; line < kBoardSize; ++line)
		for (int column = 0; column < kBoardSize; ++column)
			cell[line][column] = copy_source[line][column];
}

void Bonol::Board::Clear()
{
	for (int line = 0; line < kBoardSize; ++line)
		for (int column = 0; column < kBoardSize; ++column)
			cell[line][column] = Piece::UNUSED;
}

Bonol::Piece& Bonol::Board::at(const PosCell pos)
{
	return cell[pos.y][pos.x];
}
