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

void Bonol::GUI::DrawLine(const PosGUI from, const PosGUI to) const
{
    //line(from.x, from.y, to.x, to.y);
}

void Bonol::GUI::DrawRect(const Rect rc, const Color color) const
{
    Pen pen(Color::Black, 5.0f);
    graphics_->DrawRectangle(&pen, rc);

    SolidBrush brush(color);
    graphics_->FillRectangle(&brush, rc);
}

void Bonol::GUI::DrawCell(const PosCell cell) const
{
    const INT cool_padding = 5;
    PosGUI origin = GetTableOrigin();
    PosGUI pos = PosGUI(origin.x + cell.x * cell_width_,
        origin.y + cell.y * cell_width_);
    Piece cell_piece;
    Rect rc(
        pos.x + cool_padding,
        pos.y + cool_padding,
        cell_width_ - 2 * cool_padding,
        cell_width_ - 2 * cool_padding);

    if (new_board_state_->at(cell) != Piece::UNUSED)
    {
        cell_piece = new_board_state_->at(cell);
    }
    else
    {
        cell_piece = kGameState.GetCellPiece(cell);
    }

    switch (cell_piece)
    {
    case Piece::FREE:
    {
        DrawRect(rc, Color::White);
        break;
    }
    case Piece::RED:
    {
        DrawRect(rc, Color::PaleVioletRed);
        break;
    }
    case Piece::BLUE:
    {
        DrawRect(rc, Color::DodgerBlue);
        break;
    }
    case Piece::BLOCKED:
    {
        DrawRect(rc, Color::LightSlateGray);
        break;
    }
    case Piece::RED_SELECTED:
    {
        DrawRect(rc, Color::Firebrick);
        break;
    }
    case Piece::BLUE_SELECTED:
    {
        DrawRect(rc, Color::RoyalBlue);
        break;
    }
    case Piece::BLOCKED_SELECTED:
    {
        DrawRect(rc, Color::Green);
        break;
    }
    }
}

void Bonol::GUI::DrawTable()
{
    for (CellCoord line = 0; line < kBoardSize; ++line)
        for (CellCoord column = 0; column < kBoardSize; ++column)
            if (has_cell_updated_[column][line])
            {
                DrawCell(PosCell(column, line));
                has_cell_updated_[column][line] = false;
            }
}

void Bonol::GUI::DrawBackground() const
{
    RECT client_rect;
    GetClientRect(hwnd_, &client_rect);

    Rect rc(
        client_rect.left,
        client_rect.top,
        client_rect.right - client_rect.left,
        client_rect.bottom - client_rect.top
    );

    LinearGradientBrush brush(
        Point(client_rect.left, client_rect.top),
        Point(client_rect.right, client_rect.right),
        Color::RoyalBlue,
        Color::Firebrick
    );
    graphics_->FillRectangle(&brush, rc);
}

void Bonol::GUI::CalculateLayout()
{
    RECT window;
    GetClientRect(hwnd_, &window);
    width_  = window.right - window.left;
    height_ = window.bottom - window.top;

    table_width_ = min(min(width_, height_) - 50, 400);
    cell_width_  = table_width_ / kBoardSize;

    memset(has_cell_updated_, true, sizeof(has_cell_updated_));
}

void Bonol::GUI::OnMoveMouse(const PosGUI mouse_pos)
{
    if (is_selecting_ && IsInsideTable(mouse_pos))
    {
        PosCell hovered_cell = GetCellFromGUI(mouse_pos);
        if (kGameState.GetCellPiece(hovered_cell) == Piece::FREE || kGameState.IsActivePlayerPiece(hovered_cell))
        {
            new_board_state_->at(hovered_cell) = kGameState.GetActivePlayerSelectedPiece();
            has_cell_updated_[hovered_cell.x][hovered_cell.y] = true;

            InvalidateRect(hwnd_, 0, TRUE);
        }
    }
}

void Bonol::GUI::OnLeftClickPress(const PosGUI mouse_pos)
{
    SetCapture(hwnd_);
    is_mouse_down_ = true;

    if (IsInsideTable(mouse_pos))
    {
        PosCell clicked_cell = GetCellFromGUI(mouse_pos);
        if (kGameState.IsFreeForActivePlayer(clicked_cell))
        {
            new_board_state_->Clear();
            new_board_state_->at(clicked_cell) = kGameState.GetActivePlayerSelectedPiece();
            has_cell_updated_[clicked_cell.x][clicked_cell.y] = true;

            is_selecting_ = true;

            InvalidateRect(hwnd_, 0, TRUE);
        }
    }
}

void Bonol::GUI::OnLeftClickRelease(const PosGUI mouse_pos)
{
    if (is_selecting_)
    {
        kGameState.ValidateMove(*new_board_state_);
        new_board_state_->Clear();
        memset(has_cell_updated_, true, sizeof(has_cell_updated_));

        InvalidateRect(hwnd_, 0, TRUE);
        is_selecting_ = false;
    }

    is_mouse_down_ = false;
    ReleaseCapture();
}

void Bonol::GUI::OnPaint()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd_, &ps);
    graphics_ = new Graphics(hdc);

    DrawTable();

    delete graphics_;
    EndPaint(hwnd_, &ps);
}

void Bonol::GUI::Resize()
{
    CalculateLayout();

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd_, &ps);
    graphics_ = new Graphics(hdc);

    DrawBackground();
    DrawTable();

    delete graphics_;
    EndPaint(hwnd_, &ps);
}
