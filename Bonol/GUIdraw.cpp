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

    Pen pen(Color::Black, 5.0f);
    graphics_->DrawRectangle(&pen, rc);

    SolidBrush brush(color);
    graphics_->FillRectangle(&brush, rc);
}

void Bonol::GUI::DrawCell(const PosCell cell) const
{
    auto cell_piece = kGameState.GetCellPiece(cell);
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
        DrawSquare(pos, cell_width_, Color::Red);
        break;
    }
    case Piece::BLUE:
    {
        DrawSquare(pos, cell_width_, Color::Blue);
        break;
    }
    case Piece::BLOCKED:
    {
        DrawSquare(pos, cell_width_, Color::Black);
        break;
    }
    case Piece::RED_SELECTED:
    {
        DrawSquare(pos, cell_width_, Color::DarkRed);
        break;
    }
    case Piece::BLUE_SELECTED:
    {
        DrawSquare(pos, cell_width_, Color::DarkBlue);
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

void Bonol::GUI::CalculateLayout()
{
    RECT window;
    GetClientRect(hwnd_, &window);
    width_  = window.right - window.left;
    height_ = window.bottom - window.top;

    table_width_ = min(min(width_, height_) - 50, 400);
    cell_width_  = table_width_ / kBoardSize;
}

void Bonol::GUI::OnMoveMouse(INT pixelX, INT pixelY)
{
    /*
    if (mouse_ == NULL)
    {
        mouse_ = new PosGUI();
        *mouse_ = GetTableCenter();
    }
    else
    {
        if (IsInsideTable(*mouse_) == FALSE)
        {
            SetCursorPos(mouse_->x, mouse_->y);
        }
        else
        {
            (*mouse_) = PosGUI(pixelX, pixelY);
        }
    }
    */
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
    OnPaint();
}
