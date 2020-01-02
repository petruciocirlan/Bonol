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

/// checks

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

/// data access

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

std::basic_string<TCHAR> GUI::Bonol::GetActivePlayerName() const
{
	if (active_player_piece_ == Piece::RED)
	{
		return std::basic_string<TCHAR>(TEXT("RED"));
	}
	else
	{
		return std::basic_string<TCHAR>(TEXT("BLUE"));
	}
}

GUI::Bonol::Piece GUI::Bonol::GetCellPiece(const PosCell cell) const
{
	return old_board_->at(cell);
}

GUI::Bonol::Piece GUI::Bonol::GetUpdateCellPiece(const PosCell cell) const
{
	if (update_board_->at(cell) != Piece::UNUSED)
	{
		return update_board_->at(cell);
	}
	else
	{
		return GetCellPiece(cell);
	}
	return Piece();
}

GUI::Bonol::PosCell GUI::Bonol::GetCellFromGUI(const PointGUI pos) const
{
	PointGUI table_origin = interface_->GetTableOrigin();
	PointGUI pos_mapped_to_table_origin = PointGUI(pos.x - table_origin.x, pos.y - table_origin.y);
	return PosCell(pos_mapped_to_table_origin.x / interface_->cell_size_,
	               pos_mapped_to_table_origin.y / interface_->cell_size_);
}

GUI::PointGUI GUI::Bonol::GetGUIFromCell(const PosCell cell) const
{
	PointGUI origin = interface_->GetTableOrigin();
	PointGUI pos = PointGUI(origin.x + cell.x * interface_->cell_size_,
		origin.y + cell.y * interface_->cell_size_);
	return pos;
}

/// validate turn

bool GUI::Bonol::ValidateMove()
{
	/// there are 5 pieces in updated_board_:
	/// Piece::UNUSED, Piece::FREE, Piece::BLOCKED and Piece::RED_SELECTED

	/// ALL 'Board's are [0...kBoardSize-1, 0...kBoardSize-1]
	/// access each cell with new_state(pos) or board_(pos),
	/// where pos is a PosCell and constructor is PosCell(column, line)

	std::cout << "===============================\n";
	//std::cout << "Possible moves : "<<HowManyPossibleMoves()<<"\n"; 

	std::cout << "\n";
	std::cout << "Show the move\n";

	return ValidateL();
}

bool GUI::Bonol::ValidateL()
{
	Board& old_state = *old_board_;
	Board& update = *update_board_;

	short unsigned HowManySelectedSquares = 0, HowManyTheSame=0, i=0, CoordOfValues[4] = { 0,0,0,0 };

	for (CoordCell row = 0; row < kBoardSize; ++row)
		for (CoordCell column = 0; column < kBoardSize; ++column)
		{
			PosCell pos = PosCell(column, row);
			if (IsPlayerPiece(update.at(pos)))
			{
				//Show the squares on console
				if (update_board_->at(PosCell(column, row)) == Piece::BLUE_SELECTED 
					||	update_board_->at(PosCell(column, row))== Piece::RED_SELECTED)
				std::cout << row << " " << column << "\n";

				//Store the squares coordonates 
				if (i < 4)
				{
					CoordOfValues[i] = row * 10 + column;
					i++;
				}

				//Check how many squares are teh same w the ones from old_board_
				if ((update_board_->at(PosCell(column, row)) == Piece::RED_SELECTED ||
					update_board_->at(PosCell(column, row)) == Piece::BLUE_SELECTED)	
					&& old_board_->at(PosCell(column, row)) != Piece::FREE)
						HowManyTheSame++;
				
				//Count how many squares we've selected
				HowManySelectedSquares++;
			}
		}

	for (int i = 0; i <=3 ; i++)
		std::cout << CoordOfValues[i]<<" ";

	std::cout << "\n";
	bool GoodForm = false;


	//From there until the end of the of the big if we are checking if the L is valid

	if (SearchOnRows() != -1)
	{
		short unsigned SearchNextTo = SearchOnRows();

		if(SearchNextTo - 1 != -1)
		{
			//std::cout << "Searching on row " << SearchNextTo - 1 << "\n";
			for (CoordCell col = 0; col < kBoardSize; ++col)
			{
				if (update_board_->at(PosCell(col, SearchNextTo - 1)) == Piece::BLUE_SELECTED
					|| update_board_->at(PosCell(col, SearchNextTo - 1)) == Piece::RED_SELECTED)
					{
						short unsigned CheckGoodSquare = (SearchNextTo - 1) * 10 + col;
						if (CheckGoodForm(CheckGoodSquare,CoordOfValues))
							{
								GoodForm=true;
							}
					}
					
			}
		}

		if (SearchNextTo + 1 != 4)
		{
			//std::cout<< "Searching on row " << SearchNextTo+1 << "\n";
			for (CoordCell col = 0; col < kBoardSize; ++col)
			{
				if (update_board_->at(PosCell(col, SearchNextTo + 1)) == Piece::BLUE_SELECTED
					|| update_board_->at(PosCell(col, SearchNextTo + 1)) == Piece::RED_SELECTED)
					{
						short unsigned CheckGoodSquare = (SearchNextTo + 1) * 10 + col;
						if (CheckGoodForm(CheckGoodSquare, CoordOfValues))
							{
								GoodForm = true;
							}
					}
			}

		}
	}

	if (SearchOnColumns() != -1)
	{
		short unsigned SearchNextTo = SearchOnColumns();

		if (SearchNextTo - 1 != -1)
		{
			//std::cout <<"Searching on column "<< SearchNextTo - 1 << "\n";
			for (CoordCell row = 0; row < kBoardSize; ++row)
			{
				if (update_board_->at(PosCell(SearchNextTo -1,row)) == Piece::BLUE_SELECTED
					|| update_board_->at(PosCell(SearchNextTo - 1,row)) == Piece::RED_SELECTED)
					{
						short unsigned CheckGoodSquare = row * 10 + (SearchNextTo - 1);
						if (CheckGoodForm(CheckGoodSquare, CoordOfValues))
							{
								GoodForm = true;
							}
					}	
			}
		}	

		if (SearchNextTo + 1 != 4)
		{
			//std::cout << "Searching on column " << SearchNextTo + 1 << "\n";
			for (CoordCell row = 0; row < kBoardSize; ++row)
			{
				if (update_board_->at(PosCell(SearchNextTo + 1, row)) == Piece::BLUE_SELECTED
					|| update_board_->at(PosCell(SearchNextTo + 1, row)) == Piece::RED_SELECTED)
					{
						short unsigned CheckGoodSquare = row * 10 + (SearchNextTo + 1);
						if (CheckGoodForm(CheckGoodSquare, CoordOfValues))
							{
								GoodForm = true;
							}
				}
			}
		}
		
	}
	
	if (HowManySelectedSquares == 4 && HowManyTheSame < 4)
		if(SearchOnRows()!=-1 || SearchOnColumns()!=-1)	
			if(GoodForm)
				return true;
	return false	;
}

short GUI::Bonol::SearchOnRows()
{
	Board& old_state = *old_board_;
	Board& update = *update_board_;

	for (CoordCell row = 0; row < kBoardSize; ++row)
	{
		int CountConsecutives = 0;
		for (CoordCell column = 0; column < kBoardSize; ++column)
		{
			if (update_board_->at(PosCell(column, row)) == Piece::BLUE_SELECTED
				|| update_board_->at(PosCell(column, row)) == Piece::RED_SELECTED)
				CountConsecutives++;
		}
		
		if (CountConsecutives == 3 && update_board_->at(PosCell(1,row)) != Piece::UNUSED
			&& update_board_->at(PosCell(2,row)) != Piece::UNUSED)
			return row;
	}

	return -1;
}

short GUI::Bonol::SearchOnColumns()
{
	Board& old_state = *old_board_;
	Board& update = *update_board_;

	for (CoordCell row = 0; row < kBoardSize; ++row)
	{
		int CountConsecutives = 0;
		for (CoordCell column = 0; column < kBoardSize; ++column)
		{
			if (update_board_->at(PosCell(row,column)) == Piece::BLUE_SELECTED
				|| update_board_->at(PosCell(row,column)) == Piece::RED_SELECTED)
				CountConsecutives++;
		}

		if (CountConsecutives == 3 && update_board_->at(PosCell(row,1)) != Piece::UNUSED 
			&& update_board_->at(PosCell(row,2)) != Piece::UNUSED)
			return row;
	}

	return -1;
}

bool GUI::Bonol::CheckGoodForm(short unsigned ToCheck, short unsigned ArrayToSearch[4])
{
	//std::cout << " ( " << ToCheck << " ) ";

	for (int i = 0; i <= 3; i++)
		if (ArrayToSearch[i] == ToCheck)
			while (i < 3)
			{
				ArrayToSearch[i] = ArrayToSearch[i + 1];
				i++;
			}

	for (int i = 0; i <= 2; i++)
		if (i != 1)
			{
				if (ToCheck == ArrayToSearch[i] + 1 || ToCheck == ArrayToSearch[i] + 10
				|| ToCheck == ArrayToSearch[i] - 1 || ToCheck == ArrayToSearch[i] - 10)
					{
						//std::cout << ArrayToSearch[i]<<" ";
						return true;
					}
			}

	std::cout << "Wrong move\n";
	return false;
}

unsigned GUI::Bonol::HowManyPossibleMoves()
{
	Board& old_state = *old_board_;
	Board& update = *update_board_;
	unsigned int Count = 0;

	for (CoordCell row = 0; row < kBoardSize; ++row)
		for (CoordCell column = 0; column < kBoardSize; ++column)
			if (CountMoves(row, column)>0)
			{
				Count += CountMoves(row, column);
				std::cout << column << " " << row << " - " << CountMoves(row, column) << "\n";
			}

	std::cout << "===============================\n";

	return Count - 1;
}

short unsigned GUI::Bonol::CountMoves(short unsigned column, short unsigned row)
{	
	Board& old_state = *old_board_;
	Board& update = *update_board_;
	short unsigned Count = 0 ;

		if (IsActivePlayerPiece(PosCell(column, row)) || old_board_->at(PosCell(column, row)) == Piece::FREE && 
		old_board_->at(PosCell(column, row)) != Piece::BLOCKED)
		{
			//Check upper side
			if (row >= 2)
			 if (IsActivePlayerPiece(PosCell(column, row - 1)) || old_board_->at(PosCell(column, row - 1)) == Piece::FREE && row - 2 >= 0)
				if (IsActivePlayerPiece(PosCell(column, row - 2)) || old_board_->at(PosCell(column, row - 2)) == Piece::FREE)
				{
					if (IsActivePlayerPiece(PosCell(column + 1, row - 2)) ||
						old_board_->at(PosCell(column + 1, row - 2)) == Piece::FREE)
							if(column + 1 <= 3)
								Count++;
					if (IsActivePlayerPiece(PosCell(column - 1, row - 2)) ||
						old_board_->at(PosCell(column - 1, row - 2)) == Piece::FREE)
							if(column - 1 >= 0)
								Count++;
				}

			//Check right side
			if(column<=1)
				if (IsActivePlayerPiece(PosCell(column + 1, row)) || old_board_->at(PosCell(column + 1, row)) == Piece::FREE && column + 2 <= 3)
					if (IsActivePlayerPiece(PosCell(column + 2, row)) || old_board_->at(PosCell(column + 2, row)) == Piece::FREE)
					{
						if (IsActivePlayerPiece(PosCell(column + 2, row + 1)) ||
							old_board_->at(PosCell(column + 2, row + 1)) == Piece::FREE)
								if(row + 1 <= 3)
									Count++;
						if (IsActivePlayerPiece(PosCell(column +2, row - 1)) ||
							old_board_->at(PosCell(column + 2, row - 1)) == Piece::FREE)
								if(row - 1 >= 0)
									Count++;
					}

			//Check bottom side
			if(row<=1)
				if (IsActivePlayerPiece(PosCell(column, row + 1)) || old_board_->at(PosCell(column, row + 1)) == Piece::FREE && row + 2 <= 3)
					if (IsActivePlayerPiece(PosCell(column, row + 2)) || old_board_->at(PosCell(column, row + 2)) == Piece::FREE)
					{
						if (IsActivePlayerPiece(PosCell(column + 1, row + 2)) ||
							old_board_->at(PosCell(column + 1, row + 2)) == Piece::FREE)
								if (column + 1 <= 3)
									Count++;
						if (IsActivePlayerPiece(PosCell(column - 1, row + 2)) ||
							old_board_->at(PosCell(column - 1, row + 2)) == Piece::FREE)
								if(column - 1 >= 0)
									Count++;
					}

			//Check left side
			if (column >= 2)
				if (IsActivePlayerPiece(PosCell(column - 1, row)) || old_board_->at(PosCell(column - 1, row)) == Piece::FREE && column - 2 >= 0)
					if (IsActivePlayerPiece(PosCell(column - 2, row)) || old_board_->at(PosCell(column - 2, row)) == Piece::FREE)
					{
						if (IsActivePlayerPiece(PosCell(column - 2, row + 1)) ||
							old_board_->at(PosCell(column - 2, row + 1)) == Piece::FREE)
								if(row + 1 <= 3)
									Count++;
						if (IsActivePlayerPiece(PosCell(column - 2, row - 1)) ||
							old_board_->at(PosCell(column - 2, row - 1)) == Piece::FREE)
								if(row - 1 >= 0)
									Count++;
					}
		}
		return Count;
}

/// update state for GUI interaction

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
