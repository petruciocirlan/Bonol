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

#include "GUI.h"

void GUI::DrawLine(const PointGUI from, const PointGUI to) const
{
    //line(from.x, from.y, to.x, to.y);
}

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

void GUI::DrawTextBox(TextBox &box)
{
    CalculateTextBoxPosition(box);

    Rect box_padded_LR(box.rect);
    INT padding = 5;
    box_padded_LR.X -= padding;
    box_padded_LR.Width += 2 * padding;
    FillRect(box_padded_LR, kBackgroundColor);

    if (box.visible)
    {
        PointF drawOrigin((REAL)box.rect.X, (REAL)box.rect.Y);
        graphics_->DrawString(box.text.data(), -1, box.font, drawOrigin, box.color);
    }
}

void GUI::DrawTextBoxesMenu()
{
    if (title_->updated)
    {
        DrawTextBox(*title_);
        title_->updated = false;
    }
    if (play_button_->updated)
    {
        DrawTextBox(*play_button_);
        play_button_->updated = false;
    }
}

void GUI::DrawTextBoxesGame()
{
    if (current_player_->updated)
    {
        DrawTextBox(*current_player_);
        current_player_->updated = false;
    }
    if (skip_button_->updated)
    {
        DrawTextBox(*skip_button_);
        skip_button_->updated = false;
    }
    if (reset_button_->updated)
    {
        DrawTextBox(*reset_button_);
        reset_button_->updated = false;
    }
    if (menu_button_->updated)
    {
        DrawTextBox(*menu_button_);
        menu_button_->updated = false;
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
