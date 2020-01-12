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
	char move[5] = "EASY";
	if(GetActivePlayerName()==2)
	FindPcMove(move);

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

				//Check how many squares are the same w the ones from old_board_
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
			}

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

void GUI::Bonol::FindPcMove(char WhichMove[5])
{
	/// This function will find the best move
	Board& old_state = *old_board_;
	Board& update = *update_board_;
	Board aux_board = old_state;

	struct {
		int LT;
		int LB;
		int RT;
		int RB;
		int TL;
		int TR;
		int BL;
		int BR;
	}AuxMoves[4][4];

	for (CoordCell row = 0; row < kBoardSize; ++row)
		for (CoordCell column = 0; column < kBoardSize; ++column)
		{
			AuxMoves[column][row].LT = -1;
			AuxMoves[column][row].LB = -1;
			AuxMoves[column][row].RT = -1;
			AuxMoves[column][row].RB = -1;
			AuxMoves[column][row].TL = -1;
			AuxMoves[column][row].TR = -1;
			AuxMoves[column][row].BL = -1;
			AuxMoves[column][row].BR = -1;
		}

	std::cout << "\n" << "===== Show Board =====\n";

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
									if (IsActivePlayerPiece(PosCell(column, row)) && IsActivePlayerPiece(PosCell(column, row - 1)) &&
										IsActivePlayerPiece(PosCell(column, row - 2)) && IsActivePlayerPiece(PosCell(column + 1, row - 2)))
									{}
									else
									{
										Count++;

										DeleteCurrentPlayerPieces();

										/// Apply the move
										old_board_->at(PosCell(column, row)) = Piece::BLUE;
										old_board_->at(PosCell(column, row - 1)) = Piece::BLUE;
										old_board_->at(PosCell(column, row - 2)) = Piece::BLUE;
										old_board_->at(PosCell(column + 1, row - 2)) = Piece::BLUE;

										ChangePlayer();

										std::cout << "(TR" << HowManyPossibleMoves() << ")";
										AuxMoves[column][row].TR = HowManyPossibleMoves();

										ChangePlayer();
									}
								}
								old_state = aux_board;

							if (IsActivePlayerPiece(PosCell(column - 1, row - 2)) ||
								old_board_->at(PosCell(column - 1, row - 2)) == Piece::FREE)
								if (column - 1 >= 0)
								{
									if (IsActivePlayerPiece(PosCell(column, row)) && IsActivePlayerPiece(PosCell(column, row - 1)) &&
										IsActivePlayerPiece(PosCell(column, row - 2)) && IsActivePlayerPiece(PosCell(column - 1, row - 2)))
									{}
									else
									{
										Count++;

										DeleteCurrentPlayerPieces();

										/// Apply the move
										old_board_->at(PosCell(column, row)) = Piece::BLUE;
										old_board_->at(PosCell(column, row - 1)) = Piece::BLUE;
										old_board_->at(PosCell(column, row - 2)) = Piece::BLUE;
										old_board_->at(PosCell(column - 1, row - 2)) = Piece::BLUE;

										ChangePlayer();

										std::cout << "(TL" << HowManyPossibleMoves() << ")";
										AuxMoves[column][row].TL = HowManyPossibleMoves();

										ChangePlayer();
									}
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
								{
									if (IsActivePlayerPiece(PosCell(column, row)) && IsActivePlayerPiece(PosCell(column + 1, row)) &&
										IsActivePlayerPiece(PosCell(column + 2, row)) && IsActivePlayerPiece(PosCell(column + 2, row + 1)))
									{}
									else
									{
										Count++;

										DeleteCurrentPlayerPieces();

										/// Apply the move
										old_board_->at(PosCell(column, row)) = Piece::BLUE;
										old_board_->at(PosCell(column + 1, row)) = Piece::BLUE;
										old_board_->at(PosCell(column + 2, row)) = Piece::BLUE;
										old_board_->at(PosCell(column + 2, row + 1)) = Piece::BLUE;

										ChangePlayer();

										std::cout << "(RB" << HowManyPossibleMoves() << ")";
										AuxMoves[column][row].RB = HowManyPossibleMoves();

										ChangePlayer();
									}
									old_state = aux_board;
								}

							if (IsActivePlayerPiece(PosCell(column + 2, row - 1)) ||
								old_board_->at(PosCell(column + 2, row - 1)) == Piece::FREE)
								if (row - 1 >= 0)
								{
									if (IsActivePlayerPiece(PosCell(column, row)) && IsActivePlayerPiece(PosCell(column + 1, row)) &&
										IsActivePlayerPiece(PosCell(column + 2, row)) && IsActivePlayerPiece(PosCell(column + 2, row - 1)))
									{}
									else
									{
										Count++;

										DeleteCurrentPlayerPieces();

										/// Apply the move
										old_board_->at(PosCell(column, row)) = Piece::BLUE;
										old_board_->at(PosCell(column + 1, row)) = Piece::BLUE;
										old_board_->at(PosCell(column + 2, row)) = Piece::BLUE;
										old_board_->at(PosCell(column + 2, row - 1)) = Piece::BLUE;

										ChangePlayer();

										std::cout << "(RT" << HowManyPossibleMoves() << ")";
										AuxMoves[column][row].RT = HowManyPossibleMoves();

										ChangePlayer();
									}
									old_state = aux_board;
								}

						}

				//Check bottom side
				if (row <= 1)
					if (IsActivePlayerPiece(PosCell(column, row + 1)) || old_board_->at(PosCell(column, row + 1)) == Piece::FREE && row + 2 <= 3)
						if (IsActivePlayerPiece(PosCell(column, row + 2)) || old_board_->at(PosCell(column, row + 2)) == Piece::FREE)
						{
							if (IsActivePlayerPiece(PosCell(column + 1, row + 2)) ||
								old_board_->at(PosCell(column + 1, row + 2)) == Piece::FREE)
								if (column + 1 <= 3)
								{
									if (IsActivePlayerPiece(PosCell(column, row)) && IsActivePlayerPiece(PosCell(column, row + 1)) &&
										IsActivePlayerPiece(PosCell(column, row + 2)) && IsActivePlayerPiece(PosCell(column + 1, row + 2)))
									{}
									else
									{
										Count++;

										DeleteCurrentPlayerPieces();

										/// Apply the move
										old_board_->at(PosCell(column, row)) = Piece::BLUE;
										old_board_->at(PosCell(column, row + 1)) = Piece::BLUE;
										old_board_->at(PosCell(column, row + 2)) = Piece::BLUE;
										old_board_->at(PosCell(column + 1, row + 2)) = Piece::BLUE;

										ChangePlayer();

										std::cout << "(BR" << HowManyPossibleMoves() << ")";
										AuxMoves[column][row].BR = HowManyPossibleMoves();

										ChangePlayer();
									}
									old_state = aux_board;
								}

							if (IsActivePlayerPiece(PosCell(column - 1, row + 2)) ||
								old_board_->at(PosCell(column - 1, row + 2)) == Piece::FREE)
								if (column - 1 >= 0)
								{
									if (IsActivePlayerPiece(PosCell(column, row)) && IsActivePlayerPiece(PosCell(column, row + 1)) &&
										IsActivePlayerPiece(PosCell(column, row + 2)) && IsActivePlayerPiece(PosCell(column - 1, row + 2)))
									{}
									else
									{
										Count++;

										DeleteCurrentPlayerPieces();

										/// Apply the move
										old_board_->at(PosCell(column, row)) = Piece::BLUE;
										old_board_->at(PosCell(column, row + 1)) = Piece::BLUE;
										old_board_->at(PosCell(column, row + 2)) = Piece::BLUE;
										old_board_->at(PosCell(column - 1, row + 2)) = Piece::BLUE;

										ChangePlayer();

										std::cout << "(BL" << HowManyPossibleMoves() << ")";
										AuxMoves[column][row].BL = HowManyPossibleMoves();

										ChangePlayer();
									}
									old_state = aux_board;
								}
						}

				//Check left side
				if (column >= 2)
					if (IsActivePlayerPiece(PosCell(column - 1, row)) || old_board_->at(PosCell(column - 1, row)) == Piece::FREE && column - 2 >= 0)
						if (IsActivePlayerPiece(PosCell(column - 2, row)) || old_board_->at(PosCell(column - 2, row)) == Piece::FREE)
						{
							if (IsActivePlayerPiece(PosCell(column - 2, row + 1)) ||
								old_board_->at(PosCell(column - 2, row + 1)) == Piece::FREE)
								if (row + 1 <= 3)
								{
									if (IsActivePlayerPiece(PosCell(column, row)) && IsActivePlayerPiece(PosCell(column - 1, row)) &&
										IsActivePlayerPiece(PosCell(column - 2, row)) && IsActivePlayerPiece(PosCell(column - 2, row + 1)))
									{}
									else
									{
										Count++;

										DeleteCurrentPlayerPieces();

										/// Apply the move
										old_board_->at(PosCell(column, row)) = Piece::BLUE;
										old_board_->at(PosCell(column - 1, row)) = Piece::BLUE;
										old_board_->at(PosCell(column - 2, row)) = Piece::BLUE;
										old_board_->at(PosCell(column - 2, row + 1)) = Piece::BLUE;

										ChangePlayer();

										std::cout << "(LB" << HowManyPossibleMoves() << ")";
										AuxMoves[column][row].LB = HowManyPossibleMoves();

										ChangePlayer();
									}
									old_state = aux_board;
								}

							if (IsActivePlayerPiece(PosCell(column - 2, row - 1)) ||
								old_board_->at(PosCell(column - 2, row - 1)) == Piece::FREE)
								if (row - 1 >= 0)
								{
									if (IsActivePlayerPiece(PosCell(column, row)) && IsActivePlayerPiece(PosCell(column - 1, row)) &&
										IsActivePlayerPiece(PosCell(column - 2, row)) && IsActivePlayerPiece(PosCell(column - 2, row - 1)))
									{}
									else
									{
										Count++;

										DeleteCurrentPlayerPieces();

										/// Apply the move
										old_board_->at(PosCell(column, row)) = Piece::BLUE;
										old_board_->at(PosCell(column - 1, row)) = Piece::BLUE;
										old_board_->at(PosCell(column - 2, row)) = Piece::BLUE;
										old_board_->at(PosCell(column - 2, row - 1)) = Piece::BLUE;

										ChangePlayer();

										std::cout << "(LT" << HowManyPossibleMoves() << ")";
										AuxMoves[column][row].LT = HowManyPossibleMoves();

										ChangePlayer();
									}
									old_state = aux_board;
								}
						}
				std::cout << Count << " ";	
			}
			else std::cout << "0 ";

			if(column == 3)
			std::cout << "\n";
		}

	}
	

	int BestMove = 99;
	int WorstMove = -1;

	for (CoordCell row = 0; row < kBoardSize; ++row)
		for (CoordCell column = 0; column < kBoardSize; ++column)
		{
			if (AuxMoves[column][row].LT > 0)
			{
				BestMove = min(AuxMoves[column][row].LT, BestMove);
				WorstMove = max(AuxMoves[column][row].LT, WorstMove);
			}
			if (AuxMoves[column][row].LB > 0)
			{
				BestMove = min(AuxMoves[column][row].LB, BestMove);
				WorstMove = max(AuxMoves[column][row].LB, WorstMove);
			}
			if (AuxMoves[column][row].RT > 0)
			{
				BestMove = min(AuxMoves[column][row].RT, BestMove);
				WorstMove = max(AuxMoves[column][row].RT, WorstMove);
			}
			if (AuxMoves[column][row].RB > 0)
			{
				BestMove = min(AuxMoves[column][row].RB, BestMove);
				WorstMove = max(AuxMoves[column][row].RB, WorstMove);
			}
			if (AuxMoves[column][row].TL > 0)
			{
				BestMove = min(AuxMoves[column][row].TL, BestMove);
				WorstMove = max(AuxMoves[column][row].TL, WorstMove);
			}
			if (AuxMoves[column][row].TR > 0)
			{
				BestMove = min(AuxMoves[column][row].TR, BestMove);
				WorstMove = max(AuxMoves[column][row].TR, WorstMove);
			}
			if (AuxMoves[column][row].BL > 0)
			{
				BestMove = min(AuxMoves[column][row].BL, BestMove);
				WorstMove = max(AuxMoves[column][row].BL, WorstMove);
			}
			if (AuxMoves[column][row].BR > 0)
			{
				BestMove = min(AuxMoves[column][row].BR, BestMove);
				WorstMove = max(AuxMoves[column][row].BR, WorstMove);
			}
		}

	if (strcmp(WhichMove, "HARD")==0)
	{
		std::cout << "===== End Of Showing Board HARD =====  " << BestMove << "\n";

		bool FoundMove = false;

		for (CoordCell row = 0; row < kBoardSize && FoundMove == false; ++row)
			for (CoordCell column = 0; column < kBoardSize && FoundMove == false; ++column)
			{
				if(AuxMoves[column][row].TR == BestMove && FoundMove == false)
				{
					DeleteCurrentPlayerPieces();
					old_board_->at(PosCell(column, row)) = Piece::BLUE;
					old_board_->at(PosCell(column, row - 1)) = Piece::BLUE;
					old_board_->at(PosCell(column, row - 2)) = Piece::BLUE;
					old_board_->at(PosCell(column + 1, row - 2)) = Piece::BLUE;
					FoundMove = true;
				}
				if(AuxMoves[column][row].TL == BestMove && FoundMove == false)
				{
					DeleteCurrentPlayerPieces();

					old_board_->at(PosCell(column, row)) = Piece::BLUE;
					old_board_->at(PosCell(column, row - 1)) = Piece::BLUE;
					old_board_->at(PosCell(column, row - 2)) = Piece::BLUE;
					old_board_->at(PosCell(column - 1, row - 2)) = Piece::BLUE;
					FoundMove = true;
				}
				if(AuxMoves[column][row].RB == BestMove && FoundMove == false)
				{
					DeleteCurrentPlayerPieces();

					old_board_->at(PosCell(column, row)) = Piece::BLUE;
					old_board_->at(PosCell(column + 1, row)) = Piece::BLUE;
					old_board_->at(PosCell(column + 2, row)) = Piece::BLUE;
					old_board_->at(PosCell(column + 2, row + 1)) = Piece::BLUE;
					FoundMove = true;
				}
				if(AuxMoves[column][row].RT == BestMove && FoundMove == false)
				{
					DeleteCurrentPlayerPieces();

					old_board_->at(PosCell(column, row)) = Piece::BLUE;
					old_board_->at(PosCell(column + 1, row)) = Piece::BLUE;
					old_board_->at(PosCell(column + 2, row)) = Piece::BLUE;
					old_board_->at(PosCell(column + 2, row - 1)) = Piece::BLUE;
					FoundMove = true;
				}
				if(AuxMoves[column][row].BR == BestMove && FoundMove == false)
				{
					DeleteCurrentPlayerPieces();

					old_board_->at(PosCell(column, row)) = Piece::BLUE;
					old_board_->at(PosCell(column, row + 1)) = Piece::BLUE;
					old_board_->at(PosCell(column, row + 2)) = Piece::BLUE;
					old_board_->at(PosCell(column + 1, row + 2)) = Piece::BLUE;
					FoundMove = true;
				}
				if(AuxMoves[column][row].BL == BestMove && FoundMove == false)
				{
					DeleteCurrentPlayerPieces();

					old_board_->at(PosCell(column, row)) = Piece::BLUE;
					old_board_->at(PosCell(column, row + 1)) = Piece::BLUE;
					old_board_->at(PosCell(column, row + 2)) = Piece::BLUE;
					old_board_->at(PosCell(column - 1, row + 2)) = Piece::BLUE;
					FoundMove = true;
				}
				if(AuxMoves[column][row].LB == BestMove && FoundMove == false)
				{
					DeleteCurrentPlayerPieces();

					old_board_->at(PosCell(column, row)) = Piece::BLUE;
					old_board_->at(PosCell(column - 1, row)) = Piece::BLUE;
					old_board_->at(PosCell(column - 2, row)) = Piece::BLUE;
					old_board_->at(PosCell(column - 2, row + 1)) = Piece::BLUE;
					FoundMove = true;
				}
				if (AuxMoves[column][row].LT == BestMove && FoundMove == false)
				{
					DeleteCurrentPlayerPieces();

					old_board_->at(PosCell(column, row)) = Piece::BLUE;
					old_board_->at(PosCell(column - 1, row)) = Piece::BLUE;
					old_board_->at(PosCell(column - 2, row)) = Piece::BLUE;
					old_board_->at(PosCell(column - 2, row - 1)) = Piece::BLUE;
					FoundMove = true;
				}
			}

		std::cout << "\n\n";

		aux_board = old_state;
		int BestRock = 99, BestRockRow=-1, BestRockColumn=-1;
		bool isOver = false;

			for (CoordCell row = 0; row < kBoardSize; ++row)
				for (CoordCell column = 0; column < kBoardSize; ++column)
					if (old_board_->at(PosCell(column, row)) != Piece::BLOCKED && old_board_->at(PosCell(column, row)) == Piece::FREE)
					{
						old_board_->at(PosCell(column, row)) = Piece::BLOCKED;
						ChangePlayer();

						std::cout << HowManyPossibleMoves() << " ";
						if (column == 3) std::cout << "\n";

						if (HowManyPossibleMoves() == 0)
						{
							BestRock = HowManyPossibleMoves();
							BestRockRow = row;
							BestRockColumn = column;
						}
						ChangePlayer();
						old_state = aux_board;
					}
					else
					{
						std::cout << " -1 ";
						if (column == 3) std::cout << "\n";

					}

			std::cout << "=" << BestRock << " = "<< BestRockColumn <<"="<< BestRockRow<< "=\n" ;
			
			if (BestRock == 0)
			{
				DeleteFirstRock();
				old_board_->at(PosCell(BestRockColumn, BestRockRow)) = Piece::BLOCKED;
			}
	
			ChangePlayer();	
	}

	if (strcmp(WhichMove, "EASY") == 0)
	{
		std::cout << "===== End Of Showing Board EASY =====  "<< WorstMove << "\n";

		bool FoundMove = false;

		for (CoordCell row = 0; row < kBoardSize && FoundMove == false; ++row)
			for (CoordCell column = 0; column < kBoardSize && FoundMove == false; ++column)
			{
				if (AuxMoves[column][row].TR == WorstMove && FoundMove == false)
				{
					DeleteCurrentPlayerPieces();
					old_board_->at(PosCell(column, row)) = Piece::BLUE;
					old_board_->at(PosCell(column, row - 1)) = Piece::BLUE;
					old_board_->at(PosCell(column, row - 2)) = Piece::BLUE;
					old_board_->at(PosCell(column + 1, row - 2)) = Piece::BLUE;
					FoundMove = true;
				}
				if (AuxMoves[column][row].TL == WorstMove && FoundMove == false)
				{
					DeleteCurrentPlayerPieces();

					old_board_->at(PosCell(column, row)) = Piece::BLUE;
					old_board_->at(PosCell(column, row - 1)) = Piece::BLUE;
					old_board_->at(PosCell(column, row - 2)) = Piece::BLUE;
					old_board_->at(PosCell(column - 1, row - 2)) = Piece::BLUE;
					FoundMove = true;
				}
				if (AuxMoves[column][row].RB == WorstMove && FoundMove == false)
				{
					DeleteCurrentPlayerPieces();

					old_board_->at(PosCell(column, row)) = Piece::BLUE;
					old_board_->at(PosCell(column + 1, row)) = Piece::BLUE;
					old_board_->at(PosCell(column + 2, row)) = Piece::BLUE;
					old_board_->at(PosCell(column + 2, row + 1)) = Piece::BLUE;
					FoundMove = true;
				}
				if (AuxMoves[column][row].RT == WorstMove && FoundMove == false)
				{
					DeleteCurrentPlayerPieces();

					old_board_->at(PosCell(column, row)) = Piece::BLUE;
					old_board_->at(PosCell(column + 1, row)) = Piece::BLUE;
					old_board_->at(PosCell(column + 2, row)) = Piece::BLUE;
					old_board_->at(PosCell(column + 2, row - 1)) = Piece::BLUE;
					FoundMove = true;
				}
				if (AuxMoves[column][row].BR == WorstMove && FoundMove == false)
				{
					DeleteCurrentPlayerPieces();

					old_board_->at(PosCell(column, row)) = Piece::BLUE;
					old_board_->at(PosCell(column, row + 1)) = Piece::BLUE;
					old_board_->at(PosCell(column, row + 2)) = Piece::BLUE;
					old_board_->at(PosCell(column + 1, row + 2)) = Piece::BLUE;
					FoundMove = true;
				}
				if (AuxMoves[column][row].BL == WorstMove && FoundMove == false)
				{
					DeleteCurrentPlayerPieces();

					old_board_->at(PosCell(column, row)) = Piece::BLUE;
					old_board_->at(PosCell(column, row + 1)) = Piece::BLUE;
					old_board_->at(PosCell(column, row + 2)) = Piece::BLUE;
					old_board_->at(PosCell(column - 1, row + 2)) = Piece::BLUE;
					FoundMove = true;
				}
				if (AuxMoves[column][row].LB == WorstMove && FoundMove == false)
				{
					DeleteCurrentPlayerPieces();

					old_board_->at(PosCell(column, row)) = Piece::BLUE;
					old_board_->at(PosCell(column - 1, row)) = Piece::BLUE;
					old_board_->at(PosCell(column - 2, row)) = Piece::BLUE;
					old_board_->at(PosCell(column - 2, row + 1)) = Piece::BLUE;
					FoundMove = true;
				}
				if (AuxMoves[column][row].LT == WorstMove && FoundMove == false)
				{
					DeleteCurrentPlayerPieces();

					old_board_->at(PosCell(column, row)) = Piece::BLUE;
					old_board_->at(PosCell(column - 1, row)) = Piece::BLUE;
					old_board_->at(PosCell(column - 2, row)) = Piece::BLUE;
					old_board_->at(PosCell(column - 2, row - 1)) = Piece::BLUE;
					FoundMove = true;
				}
			}
		ChangePlayer();
	}
}

void GUI::Bonol::DeleteCurrentPlayerPieces()
{
	Board& old_state = *old_board_;

	for (CoordCell row = 0; row < kBoardSize; ++row)
		for (CoordCell column = 0; column < kBoardSize; ++column)
			if (IsActivePlayerPiece(PosCell(column, row)))
				old_board_->at(PosCell(column, row)) = Piece::FREE;

}

void GUI::Bonol::DeleteFirstRock()
{
	Board& old_state = *old_board_;

	for (CoordCell row = 0; row < kBoardSize; ++row)
		for (CoordCell column = 0; column < kBoardSize; ++column)
			if (old_board_->at(PosCell(column, row)) == Piece::BLOCKED)
			{
				old_board_->at(PosCell(column, row)) = Piece::FREE;
				column = kBoardSize + 1;
				row = kBoardSize + 1;
			}
}