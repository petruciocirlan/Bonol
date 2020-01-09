/*
    File: GUI_draw.cpp
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

void GUI::DrawCircle(const Rect rc, const Color color, const FLOAT width) const
{
	Pen pen(color, width);
	graphics_->DrawEllipse(&pen, rc);
}

void GUI::FillCircle(const Rect rc, const Color color) const
{
	SolidBrush brush(color);
	graphics_->FillEllipse(&brush, rc);
}

void GUI::DrawCell(const PointGUI pos) const
{
	//const INT cool_padding = 5;
	Rect rc(
		pos.x,// + cool_padding,
		pos.y,// + cool_padding,
		cell_size_,// - 2 * cool_padding,
		cell_size_// - 2 * cool_padding
	);

	FillRect(rc, Color::White);

	Rect free_cell = InflateRect(rc, Padding(-5));
	Rect full_cell = InflateRect(rc, Padding(-10));
	
	Bonol::Piece cell_piece = game_state_->GetUpdateCellPiece(game_state_->GetCellFromGUI(pos));

	switch (cell_piece)
	{
	case Bonol::Piece::FREE:
	{
		FillRect(free_cell, Color::White);
		DrawRect(free_cell, Color::Black, 1.0f);
		break;
	}
	case Bonol::Piece::RED:
	{
		FillRect(full_cell, Color::PaleVioletRed);
		DrawRect(full_cell, Color::Black, 1.0f);
		break;
	}
	case Bonol::Piece::BLUE:
	{
		FillRect(full_cell, Color::DodgerBlue);
		DrawRect(full_cell, Color::Black, 1.0f);
		break;
	}
	case Bonol::Piece::BLOCKED:
	{
		FillCircle(full_cell, Color::DarkGray);
		DrawCircle(full_cell, Color::Black, 1.0f);
		break;
	}
	case Bonol::Piece::RED_SELECTED:
	{
		FillRect(full_cell, Color::Firebrick);
		DrawRect(full_cell, Color::Black, 1.0f);
		break;
	}
	case Bonol::Piece::BLUE_SELECTED:
	{
		FillRect(full_cell, Color::RoyalBlue);
		DrawRect(full_cell, Color::Black, 1.0f);
		break;
	}
	case Bonol::Piece::BLOCKED_SELECTED:
	{
		FillCircle(full_cell, Color::Yellow);
		DrawCircle(full_cell, Color::Black, 1.0f);
		DrawCircle(InflateRect(full_cell, Padding(3)), Color::Black, 1.0f);
		break;
	}
	case Bonol::Piece::BLOCKED_HIGHLIGHTED:
	{
		FillCircle(full_cell, Color::LightSlateGray);
		DrawCircle(full_cell, Color::Black, 1.0f);
		DrawCircle(InflateRect(full_cell, Padding(3)), Color::Black, 1.0f);
		break;
	}
	}
}

void GUI::DrawTextBox(TextBox& box)
{
	if (box.was_hover && !box.is_hover && box.visible)
	{
		FillRect(box.rect, kBackgroundColor);
		box.was_hover = false;
	}

	FillRect(InflateRect(box.rect, box.padding), kBackgroundColor);
    CalculateTextBoxPosition(box);
    FillRect(InflateRect(box.rect, box.padding), kBackgroundColor);

    if (box.visible)
    {
		Brush *brush = box.normal_color;
		Font* font = box.normal_font;
		if (box.is_hover)
		{
			if (box.highlighted_color != NULL)
			{
				brush = box.highlighted_color;
			}
			if (box.highlighted_font != NULL)
			{
				font = box.highlighted_font;
			}
		}

		PointF drawOrigin((REAL)box.rect.X, (REAL)box.rect.Y);
        graphics_->DrawString(box.text.data(), -1, font, drawOrigin, brush);
    }
}

void GUI::DrawTextBoxes()
{
	DrawTextBoxesGlobal();

	switch (current_screen_)
	{
	case Screen::MENU: DrawTextBoxesMenu(); break;
	case Screen::GAME: DrawTextBoxesGame(); break;
	}
}

void GUI::DrawTextBoxesGlobal()
{
	for (INT counter = 0; counter < kTextBoxesGlobalCount; ++counter)
		if (text_boxes_global_[counter]->updated)
		{
			DrawTextBox(*text_boxes_global_[counter]);
			text_boxes_global_[counter]->updated = false;
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
	DrawTextBoxes();

    switch (current_screen_)
    {
    case Screen::MENU:
    {

        break;
    }
    case Screen::GAME:
    {
        game_state_->DrawTable();

        break;
    }
    }
}
