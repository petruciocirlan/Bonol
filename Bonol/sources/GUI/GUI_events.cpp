/*
    File: GUI_events.cpp
    Written by:
        Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
        Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
    CS1105: Practica - Introducere in programare
    Faculty of Computer Science, UAIC, WINTER 2019

    ABOUT THIS FILE:
    Event handling.
*/

#include "../../headers/GUI.h"

/// mouse move

void GUI::OnMouseMove(const PointGUI mouse_pos)
{
    OnMouseMoveGlobal(mouse_pos);

    switch (current_screen_)
    {
    case Screen::MENU: OnMouseMoveMenu(mouse_pos); break;
    case Screen::GAME: OnMouseMoveGame(mouse_pos); break;
    case Screen::NAME_SELECT: OnMouseMoveName(mouse_pos); break;
    }
}

void GUI::OnMouseMoveGlobal(const PointGUI mouse_pos)
{
    bool updated = false;
    for (INT counter = 0; counter < kTextBoxesGlobalCount; ++counter)
    {
        if (text_boxes_global_[counter]->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
        {
            text_boxes_global_[counter]->is_hover = true;
            text_boxes_global_[counter]->was_hover = true;
            updated = true;
        }
        else if (text_boxes_global_[counter]->is_hover)
        {
            text_boxes_global_[counter]->is_hover = false;
            updated = true;
        }
    }

    if (updated)
    {
        InvalidateTextBoxes();
        InvalidateRect(hwnd_, 0, TRUE);
    }
}

void GUI::OnMouseMoveMenu(const PointGUI mouse_pos)
{
    bool updated = false;
    for (INT counter = 0; counter < kTextBoxesMenuCount; ++counter)
    {
        if (text_boxes_menu_[counter]->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
        {
            text_boxes_menu_[counter]->is_hover = true;
            text_boxes_menu_[counter]->was_hover = true;
            updated = true;
        }
        else if (text_boxes_menu_[counter]->is_hover)
        {
            text_boxes_menu_[counter]->is_hover = false;
            updated = true;
        }
    }

    if (updated)
    {
        InvalidateTextBoxes();
        InvalidateRect(hwnd_, 0, TRUE);
    }
}

void GUI::OnMouseMoveGame(const PointGUI mouse_pos)
{
    if (is_selecting_ && IsInsideTable(mouse_pos) && selected_cells_count_ < 4)
    {
        Bonol::PosCell hovered_cell = game_state_->GetCellFromGUI(mouse_pos);
        if (game_state_->IsFreeForActivePlayer(hovered_cell) && !game_state_->IsSelected(hovered_cell))
        {
            game_state_->UpdateCell(hovered_cell, game_state_->GetActivePlayerSelected());
            if (game_state_->IsUpdateSelectionConnected())
            {
                ++selected_cells_count_;
            }
            else
            {
                game_state_->RemoveUpdateCell(hovered_cell);
            }

            //std::cout << "Selected cells count: " << selected_cells_count_ << '\n';

            InvalidateRect(hwnd_, 0, TRUE);
        }
    }
    else
    {
        bool updated = false;
        for (INT counter = 0; counter < kTextBoxesGameCount; ++counter)
        {
            if (text_boxes_game_[counter]->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
            {
                text_boxes_game_[counter]->is_hover = true;
                text_boxes_game_[counter]->was_hover = true;
                updated = true;
            }
            else if (text_boxes_game_[counter]->is_hover)
            {
                text_boxes_game_[counter]->is_hover = false;
                updated = true;
            }
        }

        if (updated)
        {
            InvalidateTextBoxes();
            InvalidateRect(hwnd_, 0, TRUE);
        }
    }
}

void GUI::OnMouseMoveName(const PointGUI mouse_pos)
{
    bool updated = false;
    for (INT counter = 0; counter < kTextBoxesNameCount; ++counter)
    {
        if (text_boxes_name_[counter]->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
        {
            text_boxes_name_[counter]->is_hover = true;
            text_boxes_name_[counter]->was_hover = true;
            updated = true;
        }
        else if (text_boxes_name_[counter]->is_hover)
        {
            text_boxes_name_[counter]->is_hover = false;
            updated = true;
        }
    }

    if (updated)
    {
        InvalidateTextBoxes();
        InvalidateRect(hwnd_, 0, TRUE);
    }
}

/// left click press

void GUI::OnLeftClickPress(const PointGUI mouse_pos)
{
    SetCapture(hwnd_);
    is_mouse_down_ = true;

    if (!OnLeftClickPressGlobal(mouse_pos))
    {
        switch (current_screen_)
        {
        case Screen::GAME: OnLeftClickPressGame(mouse_pos); break;
        case Screen::MENU: OnLeftClickPressMenu(mouse_pos); break;
        case Screen::NAME_SELECT: OnLeftClickPressName(mouse_pos); break;
        }
    }
}

bool GUI::OnLeftClickPressGlobal(const PointGUI mouse_pos)
{
    if (music_toggle_->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
    {
        if (is_playing_music_)
        {
            PlaySound(NULL, 0, 0);
            is_playing_music_ = false;
            music_toggle_->text = TEXT("PLAY MUSIC");
        }
        else
        {
            HINSTANCE hInstance = GetModuleHandle(NULL);
            if (!PlaySound(MAKEINTRESOURCE(BACKGROUND_MUSIC), hInstance, SND_RESOURCE | SND_ASYNC | SND_LOOP | SND_NODEFAULT))
            {
                // something went wrong
                // PlaySound(NULL, 0, 0);
                PlaySound(NULL, 0, 0);
                is_playing_music_ = false;
                music_toggle_->text = TEXT("PLAY MUSIC");
            }
            else
            {
                is_playing_music_ = true;
                music_toggle_->text = TEXT("STOP MUSIC");
            }
        }
        music_toggle_->updated = true;

        InvalidateRect(hwnd_, NULL, TRUE);

        return true;
    }

    return false;
}

void GUI::OnLeftClickPressMenu(const PointGUI mouse_pos)
{
    if (title_->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
    {
        std::cout << "TITLE\n";
    }
    else if (play_player_button_->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
    {
        current_screen_ = Screen::NAME_SELECT;
        current_mode_ = VersusMode::PLAYER;

        DestroyMenu();
        CreateNameSelect(TEXT("PLAYER1"));

        InvalidateRect(hwnd_, 0, TRUE);
    }
    else if (play_computer_button_->visible && play_computer_button_->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
    {
        play_computer_button_->text = TEXT("Choose difficulty");

        delete play_computer_button_->highlighted_color;
        play_computer_button_->highlighted_color = NULL;

        delete play_computer_button_->highlighted_font;
        play_computer_button_->highlighted_font = NULL;

        play_computer_button_->updated = true;

        easy_computer_button_->visible = true;
        easy_computer_button_->updated = true;

        hard_computer_button_->visible = true;
        hard_computer_button_->updated = true;

        InvalidateRect(hwnd_, 0, TRUE);
    }
    else if (easy_computer_button_->visible && easy_computer_button_->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
    {
        current_screen_ = Screen::NAME_SELECT;
        current_mode_ = VersusMode::COMPUTER_EASY;

        DestroyMenu();
        CreateNameSelect(TEXT("PLAYER"));
        player_2_ = TEXT("COMPUTER");

        InvalidateRect(hwnd_, 0, TRUE);
    }
    else if (hard_computer_button_->visible && hard_computer_button_->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
    {
        current_screen_ = Screen::NAME_SELECT;
        current_mode_ = VersusMode::COMPUTER_HARD;

        DestroyMenu();
        CreateNameSelect(TEXT("PLAYER"));
        player_2_ = TEXT("COMPUTER");

        InvalidateRect(hwnd_, 0, TRUE);
    }
}

void GUI::OnLeftClickPressGame(const PointGUI mouse_pos)
{
    static Bonol::PosCell selected_blocked_piece(-1, -1);

    if (IsInsideTable(mouse_pos))
    {
        Bonol::PosCell clicked_cell = game_state_->GetCellFromGUI(mouse_pos);
        Bonol::Piece clicked_piece = game_state_->GetCellPiece(clicked_cell);
        if (game_state_->turn_move_piece_ && game_state_->IsFreeForActivePlayer(clicked_cell))
        {
            game_state_->ClearUpdate();
            game_state_->UpdateCell(clicked_cell, game_state_->GetActivePlayerSelected());
            is_selecting_ = true;

            selected_cells_count_ = 1;

            InvalidateRect(hwnd_, 0, TRUE);
        }
        else if (game_state_->turn_move_block_ && !is_moving_block_ && clicked_piece == Bonol::Piece::BLOCKED_HIGHLIGHTED)
        {
            game_state_->ClearUpdate();
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

            EndMovingBlockTurn();

            InvalidateRect(hwnd_, 0, TRUE);
        }
    }
    else if (game_state_->turn_move_block_ && skip_button_->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
    {
        if (is_moving_block_)
        {
            game_state_->SetCellPiece(selected_blocked_piece, Bonol::Piece::BLOCKED);
            selected_blocked_piece = Bonol::PosCell(-1, -1);
            is_moving_block_ = false;
        }

        EndMovingBlockTurn();

        InvalidateRect(hwnd_, 0, TRUE);
    }
    else if (reset_button_->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
    {
        //DestroyGame();
        //CreateGame();

        if (game_history_->size() > 1)
        {
            delete game_state_;
            while (game_history_->size() > 1)
            {
                game_history_->pop();
            }
            game_state_ = new Bonol(*this, game_history_->top());

            is_moving_block_ = false;

            InvalidateTextBoxes();
            game_state_->InvalidateTable();

            InvalidateRect(hwnd_, 0, TRUE);
        }
    }
    else if (undo_button_->visible && undo_button_->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
    {
        if (game_history_->size() > 1)
        {
            delete game_state_;
            game_history_->pop();
            game_state_ = new Bonol(*this, game_history_->top());

            is_moving_block_ = false;

            InvalidateTextBoxes();
            game_state_->InvalidateTable();

            InvalidateRect(hwnd_, 0, TRUE);
        }
    }
    else if (menu_button_->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
    {
        current_screen_ = Screen::MENU;

        DestroyGame();
        CreateMenu();

        InvalidateRect(hwnd_, 0, TRUE);
    }
}

void GUI::OnLeftClickPressName(const PointGUI mouse_pos)
{
    if (select_button_->rect.Contains(Point(mouse_pos.x, mouse_pos.y)))
    {
        String player_name = name_type_->text;

        if (player_1_ == TEXT(""))
        {
            if (player_name == TEXT(""))
            {
                player_name = TEXT("PLAYER1");
            }
            player_1_ = player_name;

            DestroyNameSelect();

            if (current_mode_ == VersusMode::PLAYER)
            {
                CreateNameSelect(TEXT("PLAYER2"));
            }
            else
            {
                current_screen_ = Screen::GAME;
                CreateGame();
            }

            InvalidateRect(hwnd_, 0, TRUE);
        }
        else if (current_mode_ == VersusMode::PLAYER)
        {
            if (player_name == TEXT(""))
            {
                player_name = TEXT("PLAYER2");
            }
            player_2_ = player_name;

            current_screen_ = Screen::GAME;

            DestroyNameSelect();
            CreateGame();

            InvalidateRect(hwnd_, 0, TRUE);
        }
        else
        {
            std::cout << "UNEXPECTED!\n";
        }
    }
}

/// left click release

void GUI::OnLeftClickRelease(const PointGUI mouse_pos)
{
    OnLeftClickReleaseGlobal(mouse_pos);

    switch (current_screen_)
    {
    case Screen::GAME: OnLeftClickReleaseGame(mouse_pos); break;
    case Screen::MENU: OnLeftClickReleaseMenu(mouse_pos); break;
    case Screen::NAME_SELECT: OnLeftClickReleaseName(mouse_pos); break;
    }

    is_mouse_down_ = false;
    ReleaseCapture();
}

void GUI::OnLeftClickReleaseGlobal(const PointGUI mouse_pos)
{
    /// TODO(@petru): global - left click release
}

void GUI::OnLeftClickReleaseMenu(const PointGUI mouse_pos)
{
    /// TODO(@petru): menu - left click release
}

void GUI::OnLeftClickReleaseGame(const PointGUI mouse_pos)
{
    if (is_selecting_)
    {
        game_state_->InvalidateTable();
        if (game_state_->ValidateMove())
        {
            game_state_->ApplyMove();

            game_state_->turn_move_piece_ = false;
            game_state_->turn_move_block_ = true;
            game_state_->HighlightBlockedPieces();

            game_history_->push(Bonol(*this, *game_state_));
        }
        game_state_->ClearUpdate();

        is_selecting_ = false;
        selected_cells_count_ = 0;

        InvalidateRect(hwnd_, 0, TRUE);
    }
}

void GUI::OnLeftClickReleaseName(const PointGUI mouse_pos)
{
    /// TODO(@petru): name select - left click release
}

void GUI::TypeName(TCHAR character)
{
    if (iswalnum(character))
    {
        if (name_type_->text.size() < 8)
        {
            name_type_->text += character;
            name_type_->updated = true;

            InvalidateRect(hwnd_, 0, TRUE);
        }
    }
    else if (character == L'\b')
    {
        if (!name_type_->text.empty())
        {
            name_type_->text.pop_back();
            name_type_->updated = true;

            InvalidateRect(hwnd_, 0, TRUE);
        }
    }
}

/// paint screen

void GUI::OnPaint()
{
    CalculateLayout();

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd_, &ps);

    Bitmap buffer(window_.Width, window_.Height);
    graphics_ = new Graphics(&buffer);
    Graphics graphics(hdc);

    graphics_->SetSmoothingMode(SmoothingModeAntiAlias);
    graphics_->SetTextRenderingHint(TextRenderingHintAntiAlias);

    if (force_repaint_background_)
    {
        DrawBackground();
        force_repaint_background_ = false;
    }
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

    graphics_->SetSmoothingMode(SmoothingModeAntiAlias);
    graphics_->SetTextRenderingHint(TextRenderingHintAntiAlias);

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
        switch (game_interface->current_screen_)
        {
        case Screen::MENU: game_interface->CreateMenu(); break;
        case Screen::GAME: game_interface->CreateGame(); break;
        }
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
        game_interface->Destroy();
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
        game_interface->OnMouseMove(mouse_pos);
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
    case WM_CHAR:
    {
        if (game_interface->current_screen_ == Screen::NAME_SELECT)
        {
            game_interface->TypeName((TCHAR)wParam);
        }
        break;
    }
    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
        lpMMI->ptMinTrackSize.x = 480;
        lpMMI->ptMinTrackSize.y = 640;
        return 0;
    }
    case WM_ERASEBKGND:
        return 1;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
