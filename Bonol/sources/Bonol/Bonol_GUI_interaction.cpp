/*
	File: Bonol_GUI_interaction.cpp
	Written by:
		Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
		Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
	CS1105: Practica - Introducere in programare
	Faculty of Computer Science, UAIC, WINTER 2019

	ABOUT THIS FILE:
	Interaction between the game state and interface.
*/

#include "../../headers/Bonol.h"

void GUI::Bonol::ApplyMove()
{
	Board& old_state = *old_board_;
	Board& update = *update_board_;
	for (CoordCell row = 0; row < kBoardSize; ++row)
		for (CoordCell column = 0; column < kBoardSize; ++column)
		{
			PosCell pos = PosCell(column, row);
			if (IsActivePlayerPiece(pos))
			{
				old_state.at(pos) = Piece::FREE;
			}

			if (IsPlayerPiece(update.at(pos)))
			{
				old_state.at(pos) = GetActivePlayer();
			}
		}
	update.Clear();
}

void GUI::Bonol::InvalidateTable()
{
	memset(has_cell_updated_, true, sizeof(has_cell_updated_));
}

void GUI::Bonol::InvalidateCell(const PosCell cell)
{
	has_cell_updated_[cell.x][cell.y] = true;
}

void GUI::Bonol::ClearUpdate()
{
	update_board_->Clear();
}

void GUI::Bonol::UpdateCell(const PosCell cell, const Piece piece)
{
	update_board_->at(cell) = piece;
	InvalidateCell(cell);
}

void GUI::Bonol::HighlightBlockedPieces()
{
	for (CoordCell row = 0; row < kBoardSize; ++row)
		for (CoordCell column = 0; column < kBoardSize; ++column)
		{
			PosCell current_cell(column, row);
			Piece piece = GetCellPiece(current_cell);
			if (piece == Piece::BLOCKED)
			{
				SetCellPiece(current_cell, Piece::BLOCKED_HIGHLIGHTED);
				InvalidateCell(current_cell);
			}
		}
}

void GUI::Bonol::DeHighlightBlockedPieces()
{
	for (CoordCell row = 0; row < kBoardSize; ++row)
		for (CoordCell column = 0; column < kBoardSize; ++column)
		{
			PosCell current_cell(column, row);
			Piece piece = GetCellPiece(current_cell);
			if (piece == Piece::BLOCKED_HIGHLIGHTED)
			{
				SetCellPiece(current_cell, Piece::BLOCKED);
				InvalidateCell(current_cell);
			}
		}
}

void GUI::Bonol::DrawTable()
{
	for (CoordCell line = 0; line < kBoardSize; ++line)
		for (CoordCell column = 0; column < kBoardSize; ++column)
			if (has_cell_updated_[line][column])
			{
				interface_->DrawCell(GetGUIFromCell(PosCell(line, column)));
				has_cell_updated_[line][column] = false;
			}
}

void GUI::Bonol::SetCellPiece(const PosCell cell, const Piece piece)
{
	old_board_->at(cell) = piece;
	InvalidateCell(cell);
}

void GUI::Bonol::ChangePlayer()
{
	if (active_player_piece_ == Piece::RED)
	{
		active_player_piece_ = Piece::BLUE;
	}
	else
	{
		active_player_piece_ = Piece::RED;
	}

	std::cout << "Changed player\n";
	std::cout << "Possible moves : " << HowManyPossibleMoves() << "\n";

	if (HowManyPossibleMoves() == 0)
	{
		is_over_ = true;
	}
}
