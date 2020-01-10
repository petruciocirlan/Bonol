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

bool& GUI::Bonol::MovePieceTurn()
{
	return turn_move_piece_;
}

bool& GUI::Bonol::MoveBlockTurn()
{
	return turn_move_block_;
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

void GUI::Bonol::RemoveUpdateCell(const PosCell cell)
{
	update_board_->at(cell) = Piece::UNUSED;
	InvalidateCell(cell);
}

bool GUI::Bonol::IsUpdateSelectionConnected() const
{
	INT areas_count = 0;
	Board parse_board(*update_board_);

	const INT kDirections = 4;
	const INT kRowDirections[kDirections] =   { -1, 0, 1, 0 };
	const INT kColumnDirections[kDirections] = { 0, 1, 0,-1 };

	for (CoordCell row = 0; row < kBoardSize; ++row)
		for (CoordCell column = 0; column < kBoardSize; ++column)
			if (parse_board.at(PosCell(column, row)) != Piece::UNUSED)
			{
				++areas_count;

				std::stack < PosCell > visited;
				visited.push(PosCell(column, row));
				parse_board.at(PosCell(column, row)) = Piece::UNUSED;

				while (!visited.empty())
				{
					PosCell position = visited.top();
					bool has_visited = false;

					for (INT direction = 0; direction < kDirections; ++direction)
					{
						PosCell neighbour = PosCell(position.x + kColumnDirections[direction],
							                        position.y + kRowDirections[direction]);
						if (0 <= neighbour.x && neighbour.x < kBoardSize &&
							0 <= neighbour.y && neighbour.y < kBoardSize)
						{
							if (parse_board.at(neighbour) != Piece::UNUSED)
							{
								visited.push(neighbour);
								parse_board.at(neighbour) = Piece::UNUSED;
								has_visited = true;
								break;
							}
						}
					}

					if (!has_visited)
					{
						visited.pop();
					}
				}
			}

	return (areas_count == 1);
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
	for (CoordCell row = 0; row < kBoardSize; ++row)
		for (CoordCell column = 0; column < kBoardSize; ++column)
			if (has_cell_updated_[row][column])
			{
				interface_->DrawCell(GetGUIFromCell(PosCell(row, column)));
				has_cell_updated_[row][column] = false;
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
