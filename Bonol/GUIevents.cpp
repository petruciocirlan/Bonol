/*
    File: GUIevents.cpp
    Written by:
        Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
        Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
    CS1105: Practica - Introducere in programare
    Faculty of Computer Science, UAIC, WINTER 2019

    ABOUT THIS FILE:
    React to the window-client events.
*/

#include "GUI.h"

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
