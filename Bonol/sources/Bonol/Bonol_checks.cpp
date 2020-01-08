/*
	File: Bonol_checks.cpp
	Written by:
		Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
		Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
	CS1105: Practica - Introducere in programare
	Faculty of Computer Science, UAIC, WINTER 2019

	ABOUT THIS FILE:
	Checks about the state of the game.
*/

#include "../../headers/Bonol.h"

bool GUI::Bonol::Over() const
{
	return is_over_;
}

bool GUI::Bonol::IsValidPosition(const PosCell pos) const
{
	return ((0 <= pos.x && pos.x <= kBoardSize) &&
		(0 <= pos.y && pos.y <= kBoardSize));
}

bool GUI::Bonol::IsPlayerPiece(const Piece piece) const
{
	return (piece == Piece::RED || piece == Piece::RED_SELECTED ||
		piece == Piece::BLUE || piece == Piece::BLUE_SELECTED);
}

bool GUI::Bonol::IsActivePlayerPiece(const PosCell pos) const
{
	return GetCellPiece(pos) == GetActivePlayer();
}

bool GUI::Bonol::IsFreeForActivePlayer(const PosCell cell) const
{
	return (GetCellPiece(cell) == Piece::FREE || IsActivePlayerPiece(cell));
}

bool GUI::Bonol::IsSelected(const PosCell cell) const
{
	return (update_board_->at(cell) != Piece::UNUSED);
}
