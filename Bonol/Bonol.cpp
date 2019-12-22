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

bool GUI::Bonol::IsValidPosition(const PosCell pos) const
{
	return ((0 <= pos.x && pos.x <= kBoardSize) &&
	        (0 <= pos.y && pos.y <= kBoardSize));
}

bool GUI::Bonol::IsPlayerPiece(const Piece piece)
{
	return (piece == Piece::RED || piece == Piece::RED_SELECTED ||
		    piece == Piece::BLUE || piece == Piece::BLUE_SELECTED);
}

bool GUI::Bonol::IsActivePlayerPiece(const PosCell pos) const
{
	return GetCellPiece(pos) == GetActivePlayer();
}

void GUI::Bonol::DrawCell(const PosCell cell) const
{
	const GUI& gui = interface_;
	PointGUI origin = gui.GetTableOrigin();
	PointGUI pos = PointGUI(origin.x + cell.x * gui.cell_size_,
	                        origin.y + cell.y * gui.cell_size_);
	Piece cell_piece;
	const INT cool_padding = 5;
	Rect rc(
		pos.x + cool_padding,
		pos.y + cool_padding,
		gui.cell_size_ - 2 * cool_padding,
		gui.cell_size_ - 2 * cool_padding);

	Board& new_board = *update_board_;
	if (new_board.at(cell) != Piece::UNUSED)
	{
		cell_piece = new_board.at(cell);
	}
	else
	{
		cell_piece = GetCellPiece(cell);
	}

	switch (cell_piece)
	{
	case Piece::FREE:
	{
		gui.DrawRect(rc, Color::Black, 1.0f);
		gui.FillRect(rc, Color::White);
		break;
	}
	case Piece::RED:
	{
		gui.DrawRect(rc, Color::Black, 1.0f);
		gui.FillRect(rc, Color::PaleVioletRed);
		break;
	}
	case Piece::BLUE:
	{
		gui.DrawRect(rc, Color::Black, 1.0f);
		gui.FillRect(rc, Color::DodgerBlue);
		break;
	}
	case Piece::BLOCKED:
	{
		gui.DrawRect(rc, Color::Black, 1.0f);
		gui.FillRect(rc, Color::LightSlateGray);
		break;
	}
	case Piece::RED_SELECTED:
	{
		gui.DrawRect(rc, Color::Black, 1.0f);
		gui.FillRect(rc, Color::Firebrick);
		break;
	}
	case Piece::BLUE_SELECTED:
	{
		gui.DrawRect(rc, Color::Black, 1.0f);
		gui.FillRect(rc, Color::RoyalBlue);
		break;
	}
	case Piece::BLOCKED_SELECTED:
	{
		gui.DrawRect(rc, Color::Black, 1.0f);
		gui.FillRect(rc, Color::DarkSlateGray);
		break;
	}
	case Piece::BLOCKED_HIGHLIGHTED:
	{
		gui.DrawRect(rc, Color::Black, 1.0f);
		gui.FillRect(rc, Color::DarkGray);
		break;
	}
	}
}

GUI::Bonol::Bonol(const GUI& gui)
	: old_board_(new Board(kStartingSetup)), update_board_(new Board()), interface_(gui)
{
	memset(has_cell_updated_, false, sizeof(has_cell_updated_));
	active_player_piece_ = Piece::RED;
	is_over_ = false;
}

bool GUI::Bonol::Over() const
{
	return is_over_;
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
	interface_.current_player_->updated = true;
	std::cout << "Changed player\n";
}

void GUI::Bonol::DrawTable()
{
	for (CoordCell line = 0; line < kBoardSize; ++line)
		for (CoordCell column = 0; column < kBoardSize; ++column)
			if (has_cell_updated_[line][column])
			{
				DrawCell(PosCell(line, column));
				has_cell_updated_[line][column] = false;
			}
}

bool GUI::Bonol::ValidateL()
{
	return true;
}

bool GUI::Bonol::ValidateMove()
{
	/// TODO(@vali): check if update_board_ is a valid move

	/// NOTES: check if the pieces form an L
	/// there are 5 pieces in updated_board_:
	/// Piece::UNUSED, Piece::FREE, Piece::BLOCKED and Piece::RED_SELECTED
	/// (or Piece::BLUE_SELECTED depending on active player)
	/// ALL 'Board's are [0...kBoardSize-1, 0...kBoardSize-1]
	/// access each cell with new_state(pos) or board_(pos),
	/// where pos is a PosCell and constructor is PosCell(column, line)

	/// ACTION: if the move is valid, update the current board state with the new one
	Board& old_state = *old_board_;
	Board& update = *update_board_;
	std::cout << "Show the move\n";
	
	//old_board_->at(PosCell(3, 2)) e echivalent cu matrice[2][3]

    //if (update_board_->at(PosCell(0, 0)) == Piece::UNUSED)
	//std::cout << "YESSS";

	//Hardcore coding
	short int HowManySelectedSquares = 0;
	for (CoordCell row = 0; row < kBoardSize; ++row)
		for (CoordCell column = 0; column < kBoardSize; ++column)
		{
			PosCell pos = PosCell(column, row);
			if (IsPlayerPiece(update.at(pos)))
			{
				HowManySelectedSquares++;
				std::cout << column << " " << row << "\n";
			}
		}

	if (HowManySelectedSquares == 4 && ValidateL())
	{
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
	}
	else
	{
		update.Clear();
		return false;
	}

	update.Clear();
	return true;
}

GUI::Bonol::Piece GUI::Bonol::GetActivePlayer() const
{
	return active_player_piece_;
}

GUI::Bonol::Piece GUI::Bonol::GetActivePlayerSelected() const
{
	if (GetActivePlayer() == Piece::RED)
	{
		return Piece::RED_SELECTED;
	}
	else /*(GetActivePlayer() == Player::BLUE)*/
	{
		return Piece::BLUE_SELECTED;
	}
}

bool GUI::Bonol::IsFreeForActivePlayer(const PosCell pos) const
{
	return (GetCellPiece(pos) == Piece::FREE || IsActivePlayerPiece(pos));
}

GUI::Bonol::Piece GUI::Bonol::GetCellPiece(const PosCell pos) const
{
	return old_board_->at(pos);
}

GUI::Bonol::PosCell GUI::Bonol::GetCellFromGUI(const PointGUI pos) const
{
	PointGUI table_origin = interface_.GetTableOrigin();
	PointGUI pos_mapped_to_table_origin = PointGUI(pos.x - table_origin.x, pos.y - table_origin.y);
	return PosCell(pos_mapped_to_table_origin.x / interface_.cell_size_,
	               pos_mapped_to_table_origin.y / interface_.cell_size_);
}

void GUI::Bonol::InvalidateTable()
{
	memset(has_cell_updated_, true, sizeof(has_cell_updated_));
}

void GUI::Bonol::InvalidateCell(const PosCell pos)
{
	has_cell_updated_[pos.x][pos.y] = true;
}

void GUI::Bonol::InitiateUpdate()
{
	update_board_->Clear();
}

void GUI::Bonol::UpdateCell(const PosCell pos, const Piece piece)
{
	update_board_->at(pos) = piece;
	InvalidateCell(pos);
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

void GUI::Bonol::SetCellPiece(const PosCell pos, const Piece piece)
{
	old_board_->at(pos) = piece;
	InvalidateCell(pos);
}

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
