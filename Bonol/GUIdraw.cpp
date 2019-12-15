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

void GUI::DrawLine(const PosGUI from, const PosGUI to) const
{
    //line(from.x, from.y, to.x, to.y);
}

void GUI::DrawRect(const Rect rc, const Color color) const
{
    Pen pen(Color::Black, 5.0f);
    graphics_->DrawRectangle(&pen, rc);

    SolidBrush brush(color);
    graphics_->FillRectangle(&brush, rc);
}

void GUI::DrawBackground() const
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

void GUI::CalculateLayout()
{
    RECT window;
    GetClientRect(hwnd_, &window);
    width_  = window.right - window.left;
    height_ = window.bottom - window.top;

    table_width_ = min(min(width_, height_) - 50, 400);
    cell_width_  = table_width_ / Bonol::kBoardSize;

    kGameState->InvalidateTable();
}

void GUI::OnMoveMouse(const PosGUI mouse_pos)
{
    if (is_selecting_ && IsInsideTable(mouse_pos))
    {
        Bonol::PosCell hovered_cell = kGameState->GetCellFromGUI(mouse_pos);
        if (kGameState->GetCellPiece(hovered_cell) == Bonol::Piece::FREE ||
            kGameState->GetCellPiece(hovered_cell) == kGameState->GetActivePlayer())
        {
            kGameState->UpdateCell(hovered_cell, kGameState->GetActivePlayerSelected());

            InvalidateRect(hwnd_, 0, TRUE);
        }
    }
}

void GUI::OnLeftClickPress(const PosGUI mouse_pos)
{
    SetCapture(hwnd_);
    is_mouse_down_ = true;

    if (IsInsideTable(mouse_pos))
    {
        Bonol::PosCell clicked_cell = kGameState->GetCellFromGUI(mouse_pos);
        if (kGameState->IsFreeForActivePlayer(clicked_cell))
        {
            kGameState->InitiateUpdate();
            kGameState->UpdateCell(clicked_cell, kGameState->GetActivePlayerSelected());

            is_selecting_ = true;

            InvalidateRect(hwnd_, 0, TRUE);
        }
    }
}

void GUI::OnLeftClickRelease(const PosGUI mouse_pos)
{
    if (is_selecting_)
    {
        kGameState->ValidateMove();
        kGameState->InvalidateTable();

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

    kGameState->DrawTable();

    delete graphics_;
    EndPaint(hwnd_, &ps);
}

void GUI::Resize()
{
    CalculateLayout();

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd_, &ps);
    graphics_ = new Graphics(hdc);

    DrawBackground();
    kGameState->DrawTable();

    delete graphics_;
    EndPaint(hwnd_, &ps);
}
