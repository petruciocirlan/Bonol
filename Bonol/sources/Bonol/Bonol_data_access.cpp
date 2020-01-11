/*
	File: Bonol_data_access.cpp
	Written by:
		Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
		Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
	CS1105: Practica - Introducere in programare
	Faculty of Computer Science, UAIC, WINTER 2019

	ABOUT THIS FILE:
	Data access and conversion.
*/

#include "../../headers/Bonol.h"

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

short GUI::Bonol::GetActivePlayerName() const
{
	if (active_player_piece_ == Piece::RED)
	{
		/// Player RED
		return 1;
	}
	else
	{
		/// Player BLUE
		return 2;
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
