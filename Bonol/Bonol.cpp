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

Bonol::CellValue& Bonol::cell(const Position pos) const
{
	/// if (!validPosition(pos)) throw something
	return (*board_).cell[pos.y][pos.x];
}

bool Bonol::IsValidPosition(const Position pos) const
{
	return
		((0 <= pos.x && pos.x <= kBoardSize) &&
		(0 <= pos.y && pos.y <= kBoardSize));
}

bool Bonol::IsPlayerPiece(const Position pos) const
{
	return (cell(pos) == RED || cell(pos) == BLUE);
}

Bonol::Bonol()
{
	board_			= new Board(kStartingSetup);
	isOver_			= false;
	interface_		= new GUI(this);
	active_player_	= PLAYER_RED;
	UpdateGUI();
}

bool Bonol::Over() const
{
	return isOver_;
}

void Bonol::UpdateGUI() const
{
	(*interface_).DrawScreen();
}

void Bonol::ChangePlayer()
{
	if (active_player_ == PLAYER_RED)
		active_player_ = PLAYER_BLUE;
	else
		active_player_ = PLAYER_RED;
}

Bonol::Board::Board()
{
	memset(cell, 0, sizeof(cell));
}

Bonol::Board::Board(const CellValue copy[kBoardSize][kBoardSize])
{
	memcpy(cell, copy, sizeof(cell));
}