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
    static Bonol::PosCell selected_blocked_piece(-1, -1);

    SetCapture(hwnd_);
    is_mouse_down_ = true;

    if (IsInsideTable(mouse_pos))
    {
        Bonol::PosCell clicked_cell = game_state_->GetCellFromGUI(mouse_pos);
        Bonol::Piece clicked_piece = game_state_->GetCellPiece(clicked_cell);
        if (turn_move_piece_ && game_state_->IsFreeForActivePlayer(clicked_cell))
        {
            game_state_->InitiateUpdate();
            game_state_->UpdateCell(clicked_cell, game_state_->GetActivePlayerSelected());
            is_selecting_ = true;

            InvalidateRect(hwnd_, 0, TRUE);
        }
        else if (turn_move_block_ && !is_moving_block_ && clicked_piece == Bonol::Piece::BLOCKED)
        {   /// TODO(@petru): check for BLOCKED_HIGHLIGHTED instead
            game_state_->InitiateUpdate();
            game_state_->SetCellPiece(clicked_cell, Bonol::Piece::BLOCKED_SELECTED);
            is_moving_block_ = true;

            selected_blocked_piece = clicked_cell;
            InvalidateRect(hwnd_, 0, TRUE);
        }
        else if (is_moving_block_ && clicked_piece == Bonol::Piece::BLOCKED_SELECTED)
        {
            assert(is_moving_block_ == true);
            game_state_->SetCellPiece(clicked_cell, Bonol::Piece::BLOCKED);
            is_moving_block_ = false;

            selected_blocked_piece = Bonol::PosCell(-1, -1);
            InvalidateRect(hwnd_, 0, TRUE);
        }
        else if (is_moving_block_ && clicked_piece == Bonol::Piece::FREE)
        {
            is_moving_block_ = false;
            game_state_->SetCellPiece(clicked_cell, Bonol::Piece::BLOCKED);

            assert(!(selected_blocked_piece == Bonol::PosCell(-1, -1)));
            game_state_->SetCellPiece(selected_blocked_piece, Bonol::Piece::FREE);

            selected_blocked_piece = Bonol::PosCell(-1, -1);

            show_skip_ = false;
            skip_button_->updated = true;

            turn_move_piece_ = true;
            turn_move_block_ = false;
            game_state_->ChangePlayer();

            InvalidateRect(hwnd_, 0, TRUE);
        }
    }
    else if (turn_move_block_ && skip_button_->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
    {
        show_skip_ = false;
        skip_button_->updated = true;

        turn_move_piece_ = true;
        is_moving_block_ = false;
        game_state_->ChangePlayer();

        if (is_moving_block_)
        {
            game_state_->SetCellPiece(selected_blocked_piece, Bonol::Piece::BLOCKED);
            selected_blocked_piece = Bonol::PosCell(-1, -1);
        }

        InvalidateRect(hwnd_, 0, TRUE);
    }
    else if (reset_button_->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
    {
        FreePointers();
        Initialize();

        InvalidateRect(hwnd_, 0, TRUE);
    }
}

void GUI::OnLeftClickRelease(const PointGUI mouse_pos)
{
    if (is_selecting_)
    {
        game_state_->ValidateMove();
        game_state_->InvalidateTable();

        show_skip_ = true;
        skip_button_->updated = true;

        turn_move_piece_ = false;
        turn_move_block_ = true;

        is_selecting_ = false;
        InvalidateRect(hwnd_, 0, TRUE);
    }

    is_mouse_down_ = false;
    ReleaseCapture();

    if (game_state_->Over())
    {
        /// TODO(@petru): add "player has won" popup
        // InvalidateRect(hwnd_, 0, TRUE);
    }
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
