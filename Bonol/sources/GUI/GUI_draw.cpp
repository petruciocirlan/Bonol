/*
    File: GUIdraw.cpp
    Written by:
        Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
        Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
    CS1105: Practica - Introducere in programare
    Faculty of Computer Science, UAIC, WINTER 2019

    ABOUT THIS FILE:
    Screen drawing.
    (graphics library: GDI+ Win32 API)
*/

#include "../../headers/GUI.h"

//void GUI::DrawLine(const PointGUI from, const PointGUI to) const
//{
//    //line(from.x, from.y, to.x, to.y);
//}

void GUI::DrawRect(const Rect rc, const Color color, const FLOAT width) const
{
    Pen pen(color, width);
    graphics_->DrawRectangle(&pen, rc);
}

void GUI::FillRect(const Rect rc, const Color color) const
{
    SolidBrush brush(color);
    graphics_->FillRectangle(&brush, rc);
}

void GUI::DrawCell(const PointGUI pos) const
{
	const INT cool_padding = 5;
	Rect rc(
		pos.x + cool_padding,
		pos.y + cool_padding,
		cell_size_ - 2 * cool_padding,
		cell_size_ - 2 * cool_padding);
	
	Bonol::Piece cell_piece = game_state_->GetUpdateCellPiece(game_state_->GetCellFromGUI(pos));

	switch (cell_piece)
	{
	case Bonol::Piece::FREE:
	{
		DrawRect(InflateRect(rc, 1), Color::Black, 1.0f);
		FillRect(rc, Color::White);
		break;
	}
	case Bonol::Piece::RED:
	{
		DrawRect(InflateRect(rc, 1), Color::Black, 1.0f);
		FillRect(rc, Color::PaleVioletRed);
		break;
	}
	case Bonol::Piece::BLUE:
	{
		DrawRect(InflateRect(rc, 1), Color::Black, 1.0f);
		FillRect(rc, Color::DodgerBlue);
		break;
	}
	case Bonol::Piece::BLOCKED:
	{
		DrawRect(InflateRect(rc, 1), Color::Black, 1.0f);
		FillRect(rc, Color::DarkGray);
		break;
	}
	case Bonol::Piece::RED_SELECTED:
	{
		DrawRect(InflateRect(rc, 1), Color::Black, 1.0f);
		FillRect(rc, Color::Firebrick);
		break;
	}
	case Bonol::Piece::BLUE_SELECTED:
	{
		DrawRect(InflateRect(rc, 1), Color::Black, 1.0f);
		FillRect(rc, Color::RoyalBlue);
		break;
	}
	case Bonol::Piece::BLOCKED_SELECTED:
	{
		DrawRect(InflateRect(rc, 1), Color::Black, 1.0f);
		FillRect(rc, Color::Yellow);
		break;
	}
	case Bonol::Piece::BLOCKED_HIGHLIGHTED:
	{
		DrawRect(InflateRect(rc, 1), Color::Black, 1.0f);
		FillRect(rc, Color::LightSlateGray);
		break;
	}
	}
}

void GUI::DrawTextBox(TextBox &box)
{
    CalculateTextBoxPosition(box);
    FillRect(InflateRect(box.rect, box.padding), kBackgroundColor);

    if (box.visible)
    {
		Brush *color;
		if (box.hover && box.highlight != NULL)
		{
			color = box.highlight;
		}
		else
		{
			color = box.color;
		}

		PointF drawOrigin((REAL)box.rect.X, (REAL)box.rect.Y);
        graphics_->DrawString(box.text.data(), -1, box.font, drawOrigin, color);
    }
}

void GUI::DrawTextBoxesMenu()
{
    for (INT counter = 0; counter < kTextBoxesMenuCount; ++counter)
        if (text_boxes_menu_[counter]->updated)
        {
            DrawTextBox(*text_boxes_menu_[counter]);
            text_boxes_menu_[counter]->updated = false;
        }
}

void GUI::DrawTextBoxesGame()
{
    for (INT counter = 0; counter < kTextBoxesGameCount; ++counter)
        if (text_boxes_game_[counter]->updated)
        {
            DrawTextBox(*text_boxes_game_[counter]);
            text_boxes_game_[counter]->updated = false;
        }
}

void GUI::DrawBackground() const
{
    //LinearGradientBrush brush(
    //    Point(window_.X, window_.Y),
    //    Point(window_.X + window_.Width, window_.Y + window_.Height),
    //    Color::RoyalBlue,
    //    Color::Firebrick
    //);
	/// TODO(@petru): add background image?
    SolidBrush brush(kBackgroundColor);
    graphics_->FillRectangle(&brush, window_);
}

void GUI::DrawForeground()
{
    switch (current_screen_)
    {
    case Screen::MENU:
    {
        DrawTextBoxesMenu();

        break;
    }
    case Screen::GAME:
    {
        game_state_->DrawTable();
        DrawTextBoxesGame();

        break;
    }
    }
}
