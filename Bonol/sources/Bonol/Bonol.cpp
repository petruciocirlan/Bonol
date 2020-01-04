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

#include "../../headers/Bonol.h"

GUI::Bonol::Bonol(const GUI &gui)
	: old_board_(new Board(kStartingSetup)), update_board_(new Board()), interface_(&gui),
	  turn_move_piece_(true), turn_move_block_(false)
{
	memset(has_cell_updated_, false, sizeof(has_cell_updated_));
	active_player_piece_ = Piece::RED;
	is_over_ = false;
}

GUI::Bonol::Bonol(const GUI &gui, const Bonol &copy)
	: old_board_(new Board(kStartingSetup)), update_board_(new Board()), interface_(&gui),
	  turn_move_piece_(copy.turn_move_piece_), turn_move_block_(copy.turn_move_block_)
{
	memcpy((void*)old_board_, copy.old_board_, sizeof(Board));
	memcpy((void*)update_board_, copy.update_board_, sizeof(Board));
	memset(has_cell_updated_, 0, sizeof(has_cell_updated_));

	is_over_ = copy.is_over_;
	active_player_piece_ = copy.active_player_piece_;
}

//GUI::Bonol GUI::Bonol::operator=(const Bonol &assign)
//{
//	memcpy((void*)old_board_, assign.old_board_, sizeof(Board));
//	memcpy((void*)update_board_, assign.update_board_, sizeof(Board));
//	memset(has_cell_updated_, 0, sizeof(has_cell_updated_));
//	
//	is_over_ = assign.is_over_;
//	active_player_piece_ = assign.active_player_piece_;
//
//	turn_move_piece_ = assign.turn_move_piece_;
//	turn_move_block_ = assign.turn_move_block_;
//
//	return *this;
//}

//GUI::Bonol::~Bonol()
//{
//	delete old_board_, update_board_;
//}

/// Board struct

GUI::Bonol::Board::Board()
{
	memset(cell, 0, sizeof(cell));
	for (int line = 0; line < kBoardSize; ++line)
		for (int column = 0; column < kBoardSize; ++column)
			cell[line][column] = Piece::UNUSED;
}

GUI::Bonol::Board::Board(const Piece copy_source[kBoardSize][kBoardSize])
{
	memset(cell, 0, sizeof(cell));
	for (int line = 0; line < kBoardSize; ++line)
		for (int column = 0; column < kBoardSize; ++column)
			cell[line][column] = copy_source[line][column];
}

GUI::Bonol::Board::Board(const Board& copy)
{
	memcpy(cell, copy.cell, sizeof(cell));
}

GUI::Bonol::Board GUI::Bonol::Board::operator=(const Board& assign)
{
	memcpy(cell, assign.cell, sizeof(cell));
	return *this;
}

void GUI::Bonol::Board::Clear()
{
	for (int line = 0; line < kBoardSize; ++line)
		for (int column = 0; column < kBoardSize; ++column)
			cell[line][column] = Piece::UNUSED;
}

GUI::Bonol::Piece& GUI::Bonol::Board::at(const PosCell pos)
{
	return cell[pos.y][pos.x];
}

bool GUI::Bonol::PosCell::operator==(const PosCell& operand) const
{
	return (this->x == operand.x && this->y == operand.y);
}
