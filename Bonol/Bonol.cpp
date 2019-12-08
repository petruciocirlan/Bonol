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

Bonol::Piece& Bonol::GetCellPiece(const Position pos) const
{
	/// if (!validPosition(pos)) throw something
	return (*board_).cell[pos.y][pos.x];
}

bool Bonol::IsValidPosition(const Position pos) const
{
	return ((0 <= pos.x && pos.x <= kBoardSize) &&
			(0 <= pos.y && pos.y <= kBoardSize));
}

bool Bonol::IsPlayerPiece(const Position pos) const
{
	return (GetCellPiece(pos) == Piece::RED || GetCellPiece(pos) == Piece::BLUE);
}

bool Bonol::IsActivePlayerPiece(const Position pos) const
{
	return ((GetCellPiece(pos) == Piece::RED && GetActivePlayer() == Player::RED) ||
			(GetCellPiece(pos) == Piece::BLUE && GetActivePlayer() == Player::BLUE));
}

Bonol::Bonol(const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow)
	: board_(new Board(kStartingSetup)), interface_(new GUI(this, window_dimensions, hInstance, nCmdShow))
{
	isOver_			= false;
	active_player_	= Player::RED;
}

bool Bonol::Over() const
{
	return isOver_;
}

void Bonol::ChangePlayer()
{
	if (active_player_ == Player::RED)
		active_player_ = Player::BLUE;
	else
		active_player_ = Player::RED;
}

Bonol::Player Bonol::GetActivePlayer() const
{
	return active_player_;
}

Bonol::Board::Board()
{
	memset(cell, 0, sizeof(cell));
}

Bonol::Board::Board(const Piece copy_source[kBoardSize][kBoardSize])
{
	memcpy(cell, copy_source, sizeof(cell));
}