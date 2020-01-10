/*
	File: Bonol_mechanics.cpp
	Written by:
		Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
		Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
	CS1105: Practica - Introducere in programare
	Faculty of Computer Science, UAIC, WINTER 2019

	ABOUT THIS FILE:
	Mechanics of the game.
*/

#include "../../headers/Bonol.h"

bool GUI::Bonol::ValidateMove()
{	
	/// there are 5 pieces in updated_board_:
	/// Piece::UNUSED, Piece::FREE, Piece::BLOCKED and Piece::RED_SELECTED

	/// ALL 'Board's are [0...kBoardSize-1, 0...kBoardSize-1]
	/// access each cell with new_state(pos) or board_(pos),
	/// where pos is a PosCell and constructor is PosCell(column, line)

	///std::cout << "===============================\n";

	FindBestMove();

	//std::cout << "Possible moves : "<<HowManyPossibleMoves()<<"\n"; 

	std::cout << "\n";
	std::cout << "Show the move\n";

	return ValidateL();
}

bool GUI::Bonol::ValidateL()
{
	Board& old_state = *old_board_;
	Board& update = *update_board_;

	short unsigned HowManySelectedSquares = 0, HowManyTheSame = 0, i = 0, CoordOfValues[4] = { 0,0,0,0 };

	for (CoordCell row = 0; row < kBoardSize; ++row)
		for (CoordCell column = 0; column < kBoardSize; ++column)
		{
			PosCell pos = PosCell(column, row);
			if (IsPlayerPiece(update.at(pos)))
			{
				//Show the squares on console
				if (update_board_->at(PosCell(column, row)) == Piece::BLUE_SELECTED
					|| update_board_->at(PosCell(column, row)) == Piece::RED_SELECTED)
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

	for (int i = 0; i <= 3; i++)
		std::cout << CoordOfValues[i] << " ";

	std::cout << "\n";
	bool GoodForm = false;


	//From there until the end of the of the big if we are checking if the L is valid

	if (SearchOnRows() != -1)
	{
		short unsigned SearchNextTo = SearchOnRows();

		if (SearchNextTo - 1 != -1)
		{
			//std::cout << "Searching on row " << SearchNextTo - 1 << "\n";
			for (CoordCell col = 0; col < kBoardSize; ++col)
			{
				if (update_board_->at(PosCell(col, SearchNextTo - 1)) == Piece::BLUE_SELECTED
					|| update_board_->at(PosCell(col, SearchNextTo - 1)) == Piece::RED_SELECTED)
				{
					short unsigned CheckGoodSquare = (SearchNextTo - 1) * 10 + col;
					if (CheckGoodForm(CheckGoodSquare, CoordOfValues))
					{
						GoodForm = true;
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
				if (update_board_->at(PosCell(SearchNextTo - 1, row)) == Piece::BLUE_SELECTED
					|| update_board_->at(PosCell(SearchNextTo - 1, row)) == Piece::RED_SELECTED)
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
		if (SearchOnRows() != -1 || SearchOnColumns() != -1)
			if (GoodForm)
				return true;
	return false;
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

		if (CountConsecutives == 3 && update_board_->at(PosCell(1, row)) != Piece::UNUSED
			&& update_board_->at(PosCell(2, row)) != Piece::UNUSED)
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
			if (update_board_->at(PosCell(row, column)) == Piece::BLUE_SELECTED
				|| update_board_->at(PosCell(row, column)) == Piece::RED_SELECTED)
				CountConsecutives++;
		}

		if (CountConsecutives == 3 && update_board_->at(PosCell(row, 1)) != Piece::UNUSED
			&& update_board_->at(PosCell(row, 2)) != Piece::UNUSED)
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
			if (CountMoves(row, column) > 0)
			{
				Count += CountMoves(row, column);
				//std::cout << column << " " << row << " - " << CountMoves(row, column) << "\n";
			}

	//std::cout << "===============================\n";

	return Count - 1;
}

short unsigned GUI::Bonol::CountMoves(short unsigned column, short unsigned row)
{
	Board& old_state = *old_board_;
	Board& update = *update_board_;
	short unsigned Count = 0;

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
						if (column + 1 <= 3)
							Count++;
					if (IsActivePlayerPiece(PosCell(column - 1, row - 2)) ||
						old_board_->at(PosCell(column - 1, row - 2)) == Piece::FREE)
						if (column - 1 >= 0)
							Count++;
				}

		//Check right side
		if (column <= 1)
			if (IsActivePlayerPiece(PosCell(column + 1, row)) || old_board_->at(PosCell(column + 1, row)) == Piece::FREE && column + 2 <= 3)
				if (IsActivePlayerPiece(PosCell(column + 2, row)) || old_board_->at(PosCell(column + 2, row)) == Piece::FREE)
				{
					if (IsActivePlayerPiece(PosCell(column + 2, row + 1)) ||
						old_board_->at(PosCell(column + 2, row + 1)) == Piece::FREE)
						if (row + 1 <= 3)
							Count++;
					if (IsActivePlayerPiece(PosCell(column + 2, row - 1)) ||
						old_board_->at(PosCell(column + 2, row - 1)) == Piece::FREE)
						if (row - 1 >= 0)
							Count++;
				}

		//Check bottom side
		if (row <= 1)
			if (IsActivePlayerPiece(PosCell(column, row + 1)) || old_board_->at(PosCell(column, row + 1)) == Piece::FREE && row + 2 <= 3)
				if (IsActivePlayerPiece(PosCell(column, row + 2)) || old_board_->at(PosCell(column, row + 2)) == Piece::FREE)
				{
					if (IsActivePlayerPiece(PosCell(column + 1, row + 2)) ||
						old_board_->at(PosCell(column + 1, row + 2)) == Piece::FREE)
						if (column + 1 <= 3)
							Count++;
					if (IsActivePlayerPiece(PosCell(column - 1, row + 2)) ||
						old_board_->at(PosCell(column - 1, row + 2)) == Piece::FREE)
						if (column - 1 >= 0)
							Count++;
				}

		//Check left side
		if (column >= 2)
			if (IsActivePlayerPiece(PosCell(column - 1, row)) || old_board_->at(PosCell(column - 1, row)) == Piece::FREE && column - 2 >= 0)
				if (IsActivePlayerPiece(PosCell(column - 2, row)) || old_board_->at(PosCell(column - 2, row)) == Piece::FREE)
				{
					if (IsActivePlayerPiece(PosCell(column - 2, row + 1)) ||
						old_board_->at(PosCell(column - 2, row + 1)) == Piece::FREE)
						if (row + 1 <= 3)
							Count++;
					if (IsActivePlayerPiece(PosCell(column - 2, row - 1)) ||
						old_board_->at(PosCell(column - 2, row - 1)) == Piece::FREE)
						if (row - 1 >= 0)
							Count++;
				}
	}
	return Count;
}

void GUI::Bonol::FindBestMove()
{
	///this function will find the best move
	Board& old_state = *old_board_;
	Board& update = *update_board_;
	Board aux_board = old_state;

	struct {
		short LT;
		short LB;
		short RT;
		short RB;
		short TL;
		short TR;
		short BL;
		short BR;
	}AuxMoves[3][3];

	std::cout << "\n" << "===== Show Board =====\n";

	
	//ChangePlayer();


	for (CoordCell row = 0; row < kBoardSize; ++row)
	{
		for (CoordCell column = 0; column < kBoardSize; ++column)
		{
			if (IsActivePlayerPiece(PosCell(column, row)) || old_board_->at(PosCell(column, row)) == Piece::FREE &&
				old_board_->at(PosCell(column, row)) != Piece::BLOCKED)	
			{
				unsigned short Count = 0;

				//Check upper side
				if (row >= 2)
					if (IsActivePlayerPiece(PosCell(column, row - 1)) || old_board_->at(PosCell(column, row - 1)) == Piece::FREE && row - 2 >= 0)
						if (IsActivePlayerPiece(PosCell(column, row - 2)) || old_board_->at(PosCell(column, row - 2)) == Piece::FREE)
						{
							if (IsActivePlayerPiece(PosCell(column + 1, row - 2)) || old_board_->at(PosCell(column + 1, row - 2)) == Piece::FREE)
								if (column + 1 <= 3)
								{
									if(IsActivePlayerPiece(PosCell(column, row)) && IsActivePlayerPiece(PosCell(column, row - 1)) && 
										IsActivePlayerPiece(PosCell(column, row - 2)) && IsActivePlayerPiece(PosCell(column + 1, row - 2)))
									{}else Count++;

									/// apoi pe old board stergi intai piesele 
									/// aplici mutarea
									/// changeplayer apoi pui in AuxMoves in ce variabial trebuie cate mutari ii raman la pc
									/// ChangePlayer();
									/// aduci old board la starea initiala 
								}
							old_state = aux_board;

							if (IsActivePlayerPiece(PosCell(column - 1, row - 2)) ||
								old_board_->at(PosCell(column - 1, row - 2)) == Piece::FREE)
								if (column - 1 >= 0)
								{
									if (IsActivePlayerPiece(PosCell(column, row)) && IsActivePlayerPiece(PosCell(column, row - 1)) &&
										IsActivePlayerPiece(PosCell(column, row - 2)) && IsActivePlayerPiece(PosCell(column - 1, row - 2)))
									{}else Count++;
								}
     						old_state = aux_board;

						}

				//Check right side
				if (column <= 1)
					if (IsActivePlayerPiece(PosCell(column + 1, row)) || old_board_->at(PosCell(column + 1, row)) == Piece::FREE && column + 2 <= 3)
						if (IsActivePlayerPiece(PosCell(column + 2, row)) || old_board_->at(PosCell(column + 2, row)) == Piece::FREE)
						{
							if (IsActivePlayerPiece(PosCell(column + 2, row + 1)) ||
								old_board_->at(PosCell(column + 2, row + 1)) == Piece::FREE)
								if (row + 1 <= 3)
									Count++;
							if (IsActivePlayerPiece(PosCell(column + 2, row - 1)) ||
								old_board_->at(PosCell(column + 2, row - 1)) == Piece::FREE)
								if (row - 1 >= 0)
									Count++;
						}

				//Check bottom side
				if (row <= 1)
					if (IsActivePlayerPiece(PosCell(column, row + 1)) || old_board_->at(PosCell(column, row + 1)) == Piece::FREE && row + 2 <= 3)
						if (IsActivePlayerPiece(PosCell(column, row + 2)) || old_board_->at(PosCell(column, row + 2)) == Piece::FREE)
						{
							if (IsActivePlayerPiece(PosCell(column + 1, row + 2)) ||
								old_board_->at(PosCell(column + 1, row + 2)) == Piece::FREE)
								if (column + 1 <= 3)
									Count++;
							if (IsActivePlayerPiece(PosCell(column - 1, row + 2)) ||
								old_board_->at(PosCell(column - 1, row + 2)) == Piece::FREE)
								if (column - 1 >= 0)
									Count++;
						}

				//Check left side
				if (column >= 2)
					if (IsActivePlayerPiece(PosCell(column - 1, row)) || old_board_->at(PosCell(column - 1, row)) == Piece::FREE && column - 2 >= 0)
						if (IsActivePlayerPiece(PosCell(column - 2, row)) || old_board_->at(PosCell(column - 2, row)) == Piece::FREE)
						{
							if (IsActivePlayerPiece(PosCell(column - 2, row + 1)) ||
								old_board_->at(PosCell(column - 2, row + 1)) == Piece::FREE)
								if (row + 1 <= 3)
									Count++;
							if (IsActivePlayerPiece(PosCell(column - 2, row - 1)) ||
								old_board_->at(PosCell(column - 2, row - 1)) == Piece::FREE)
								if (row - 1 >= 0)
									Count++;
						}
				std::cout << Count << " ";
				//std::cout << CountMoves(column, row) << " ";
			}
			else std::cout << "0 ";

			if(column == 3)
			std::cout << "\n";
		}

	}
	
	
	//ChangePlayer();


	std::cout << "===== End Of Showing Board =====\n";

}

void GUI::Bonol::DeleteCurrentPlayerPieces()
{
	Board& old_state = *old_board_;
	Board& update = *update_board_;

	for (CoordCell row = 0; row < kBoardSize; ++row)
		for (CoordCell column = 0; column < kBoardSize; ++column)
			if (IsActivePlayerPiece(PosCell(column, row)))
				old_board_->at(PosCell(column - 2, row - 1)) = Piece::FREE;

}