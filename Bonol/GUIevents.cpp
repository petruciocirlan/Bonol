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
        else if (turn_move_block_ && !is_moving_block_ && clicked_piece == Bonol::Piece::BLOCKED_HIGHLIGHTED)
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
            game_state_->SetCellPiece(clicked_cell, Bonol::Piece::BLOCKED_HIGHLIGHTED);
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
            game_state_->DeHighlightBlockedPieces();

            InvalidateRect(hwnd_, 0, TRUE);
        }
    }
    else if (turn_move_block_ && skip_button_->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
    {
        show_skip_ = false;
        skip_button_->updated = true;

        turn_move_piece_ = true;
        turn_move_block_ = false;
        is_moving_block_ = false;
        game_state_->ChangePlayer();
        game_state_->DeHighlightBlockedPieces();

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
        game_state_->HighlightBlockedPieces();

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

    Bitmap buffer(window_.Width, window_.Height);
    graphics_ = new Graphics(&buffer);
    Graphics graphics(hdc);

    DrawForeground();

    graphics.DrawImage(&buffer, 0, 0);

    delete graphics_;
    EndPaint(hwnd_, &ps);
}

void GUI::Resize()
{
    CalculateLayout();
    InvalidateTextBoxes();

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd_, &ps);

    Bitmap buffer(window_.Width, window_.Height);
    graphics_ = new Graphics(&buffer);
    Graphics graphics(hdc);

    DrawBackground();
    DrawForeground();

    graphics.DrawImage(&buffer, 0, 0);

    delete graphics_;
    EndPaint(hwnd_, &ps);
}

LRESULT GUI::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    GUI* game_interface = NULL;

    if (uMsg != WM_CREATE)
    {
        game_interface = (GUI*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    switch (uMsg)
    {
    case WM_CREATE:
    {
        SetWindowDataInfo(hwnd, lParam, game_interface);
        game_interface->Initialize();
        return 0;
    }
    case WM_CLOSE:
    {
        if (MessageBox(hwnd, L"Really quit?", L"Bonol", MB_OKCANCEL) == IDOK)
        {
            DestroyWindow(hwnd);
        }
        return 0;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_PAINT:
    {
        game_interface->OnPaint();
        return 0;
    }
    case WM_SIZE:
    {
        game_interface->Resize();
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        PointGUI mouse_pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        game_interface->OnMoveMouse(mouse_pos);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        PointGUI mouse_pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        game_interface->OnLeftClickPress(mouse_pos);
        return 0;
    }
    case WM_LBUTTONUP:
    {
        PointGUI mouse_pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        game_interface->OnLeftClickRelease(mouse_pos);
        return 0;
    }
    case WM_KEYDOWN:
    {
        if (GetKeyState(VK_ESCAPE) & 0x8000)
        {
            if (MessageBox(hwnd, L"Really quit?", L"Bonol", MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(hwnd);
            }
            return 0;
        }
        break;
    }
    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
        lpMMI->ptMinTrackSize.x = 480;
        lpMMI->ptMinTrackSize.y = 600;
        return 0;
    }
    case WM_ERASEBKGND:
        return 1;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
