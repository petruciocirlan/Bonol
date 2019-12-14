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

void Bonol::GUI::DrawSquare(const PosGUI pos, const INT width, const Color color) const
{
    Rect rc(
        pos.x,
        pos.y,
        width,
        width);

    //Pen pen(Color::Black, 5.0f);
    //graphics_->DrawRectangle(&pen, rc);

    SolidBrush brush(color);
    graphics_->FillRectangle(&brush, rc);
}

void Bonol::GUI::DrawCell(const PosCell cell) const
{
    Piece cell_piece;
    if (new_board_state_->at(cell) != Piece::UNUSED)
    {
        cell_piece = new_board_state_->at(cell);
    }
    else
    {
        cell_piece = kGameState.GetCellPiece(cell);
    }
    PosGUI origin = GetTableOrigin();
    PosGUI pos = PosGUI(origin.x + cell.x * cell_width_,
                        origin.y + cell.y * cell_width_);
    switch (cell_piece)
    {
    case Piece::FREE:
    {
        DrawSquare(pos, cell_width_, Color::White);
        break;
    }
    case Piece::RED:
    {
        DrawSquare(pos, cell_width_, Color::PaleVioletRed);
        break;
    }
    case Piece::BLUE:
    {
        DrawSquare(pos, cell_width_, Color::DodgerBlue);
        break;
    }
    case Piece::BLOCKED:
    {
        DrawSquare(pos, cell_width_, Color::LightSlateGray);
        break;
    }
    case Piece::RED_SELECTED:
    {
        DrawSquare(pos, cell_width_, Color::Red);
        break;
    }
    case Piece::BLUE_SELECTED:
    {
        DrawSquare(pos, cell_width_, Color::Blue);
        break;
    }
    case Piece::BLOCKED_SELECTED:
    {
        DrawSquare(pos, cell_width_, Color::Green);
        break;
    }
    }
}

void Bonol::GUI::DrawTable() const
{
    for (CellCoord line = 0; line < kBoardSize; ++line)
        for (CellCoord column = 0; column < kBoardSize; ++column)
            DrawCell(PosCell(column, line));
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
    SolidBrush brush(Color::MediumPurple);
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
}

void Bonol::GUI::OnMoveMouse(const PosGUI mouse_pos)
{
    if (is_selecting_ && IsInsideTable(mouse_pos))
    {
        PosCell hovered_cell = GetCellFromGUI(mouse_pos);
        if (kGameState.GetCellPiece(hovered_cell) == Piece::FREE || kGameState.IsActivePlayerPiece(hovered_cell))
        {
            new_board_state_->at(hovered_cell) = kGameState.GetActivePlayerSelectedPiece();
            
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
        if (kGameState.GetCellPiece(clicked_cell) == Piece::FREE || kGameState.IsActivePlayerPiece(clicked_cell))
        {
            new_board_state_->Clear();
            new_board_state_->at(clicked_cell) = kGameState.GetActivePlayerSelectedPiece();

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
