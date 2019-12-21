/*
    File: GUIdraw.cpp
    Written by:
        Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
        Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
    CS1105: Practica - Introducere in programare
    Faculty of Computer Science, UAIC, WINTER 2019

    ABOUT THIS FILE:
    Draw the Bonol game state to screen.
    (using GDI+ Win32 API)
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

void GUI::DrawCurrentPlayer() const
{
    TextBox& box = *current_player_;
    FillRect(box.rect, kBackgroundColor);

    std::basic_string<TCHAR> drawString = TEXT("Your turn, ");
    if (game_state_->GetActivePlayer() == Bonol::Piece::RED)
    {
        drawString += TEXT("RED!");
    }
    else
    {
        drawString += TEXT("BLUE!");
    }

    Font drawFont(TEXT("Arial"), 16);
    PointF drawOrigin((FLOAT)box.x, (FLOAT)box.y);
    SolidBrush drawBrush(Color::White);
    graphics_->DrawString(drawString.data(), -1, &drawFont, drawOrigin, &drawBrush);
}

void GUI::DrawSkipButton() const
{
    TextBox& box = *skip_button_;
    FillRect(box.rect, kBackgroundColor);

    std::basic_string<TCHAR> drawString = TEXT("SKIP?");
    Font drawFont(TEXT("Arial"), 16);
    PointF drawOrigin((FLOAT)box.x, (FLOAT)box.y);
    SolidBrush drawBrush(Color::White);
    graphics_->DrawString(drawString.data(), -1, &drawFont, drawOrigin, &drawBrush);
}

void GUI::DrawTextBoxes()
{
    if (current_player_->updated)
    {
        DrawCurrentPlayer();
        current_player_->updated = false;
    }
    if (skip_button_->updated)
    {
        DrawSkipButton();
        skip_button_->updated = false;
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
    game_state_->DrawTable();
    DrawTextBoxes();
}

void GUI::CalculateLayout()
{
    RECT window;
    GetClientRect(hwnd_, &window);
    window_ = Rect(
        window.left,
        window.top,
        window.right - window.left,
        window.bottom - window.top
    );

    INT table_size = 400;
    table_ = Rect(
        (window_.Width - table_size) / 2,
        (window_.Height - table_size) / 2,
        table_size,
        table_size
    );
    cell_size_  = table_.Width / Bonol::kBoardSize;

    PointGUI text_box_dimensions, text_box_origin;

    text_box_dimensions = PointGUI(225, 35);
    text_box_origin = PointGUI((window_.Width - text_box_dimensions.x) / 2, table_.Y - 50);
    current_player_->rect = MakeRect(text_box_origin, text_box_dimensions);

    text_box_dimensions = PointGUI(85, 35);
    text_box_origin = PointGUI((window_.Width - text_box_dimensions.x) / 2, table_.Y + table_.Height + 10);
    skip_button_->rect = MakeRect(text_box_origin, text_box_dimensions);

    game_state_->InvalidateTable();
}

void GUI::InvalidateTextBoxes()
{
    current_player_->updated = true;
    skip_button_->updated = true;
}

void GUI::OnMoveMouse(const PointGUI mouse_pos)
{
    if (is_selecting_ && IsInsideTable(mouse_pos))
    {
        Bonol::PosCell hovered_cell = game_state_->GetCellFromGUI(mouse_pos);
        if (game_state_->IsFreeForActivePlayer(hovered_cell))
        {
            game_state_->UpdateCell(hovered_cell, game_state_->GetActivePlayerSelected());

            InvalidateRect(hwnd_, 0, TRUE);
        }
    }
}

void GUI::OnLeftClickPress(const PointGUI mouse_pos)
{
    SetCapture(hwnd_);
    is_mouse_down_ = true;

    if (IsInsideTable(mouse_pos))
    {
        Bonol::PosCell clicked_cell = game_state_->GetCellFromGUI(mouse_pos);
        if (game_state_->IsFreeForActivePlayer(clicked_cell))
        {
            game_state_->InitiateUpdate();
            game_state_->UpdateCell(clicked_cell, game_state_->GetActivePlayerSelected());

            is_selecting_ = true;

            InvalidateRect(hwnd_, 0, TRUE);
        }
    }
}

void GUI::OnLeftClickRelease(const PointGUI mouse_pos)
{
    if (is_selecting_)
    {
        game_state_->ValidateMove();
        game_state_->InvalidateTable();

        InvalidateRect(hwnd_, 0, TRUE);
        is_selecting_ = false;
    }

    is_mouse_down_ = false;
    ReleaseCapture();
}

void GUI::OnPaint()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd_, &ps);
    graphics_ = new Graphics(hdc);

    DrawForeground();

    delete graphics_;
    EndPaint(hwnd_, &ps);
}

void GUI::Resize()
{
    CalculateLayout();
    InvalidateTextBoxes();

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd_, &ps);
    graphics_ = new Graphics(hdc);

    DrawBackground();
    DrawForeground();

    delete graphics_;
    EndPaint(hwnd_, &ps);
}
