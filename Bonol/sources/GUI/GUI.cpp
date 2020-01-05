/*
    File: GUI.cpp
    Written by:
        Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
        Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
    CS1105: Practica - Introducere in programare
    Faculty of Computer Science, UAIC, WINTER 2019

    ABOUT THIS FILE:
    Logistics, running of the window.
*/

#include "../../headers/GUI.h"

Rect GUI::MakeRect(PointGUI origin, PointGUI dimensions) const
{
    return Rect(origin.x, origin.y, dimensions.x, dimensions.y);
}

Rect GUI::InflateRect(const Rect rect, Padding padding) const
{
    return Rect(
        rect.X - padding.left,
        rect.Y - padding.top,
        rect.Width + padding.left + padding.right - 1,
        rect.Height + padding.top + padding.bottom - 1
    );
}

GUI::PointGUI GUI::GetTableCenter() const
{
    return PointGUI(window_.Width / 2, window_.Height / 2);
}

GUI::PointGUI GUI::GetTableOrigin() const
{
    return PointGUI(table_.X, table_.Y);
}

BOOL GUI::IsInsideTable(const PointGUI pos) const
{
    return table_.Contains(pos.x, pos.y);
}

void GUI::CalculateTextBoxPosition(TextBox& box)
{
    RectF bounds;
    Font *font = box.normal_font;
    if (box.is_hover && box.highlighted_font != NULL)
    {
        font = box.highlighted_font;
    }
    graphics_->MeasureString(box.text.data(), -1, font, PointF(0, 0), &bounds);
    PointGUI text_box_dimensions((INT)bounds.Width, (INT)bounds.Height);
    PointGUI text_box_origin(box.center.x - (INT)bounds.Width / 2, box.center.y - (INT)bounds.Height / 2);
    box.rect = MakeRect(text_box_origin, text_box_dimensions);
}

void GUI::CalculateCurrentPlayerText()
{
    std::basic_string<TCHAR> current_player = game_state_->GetActivePlayerName();
    if (game_state_->Over())
    {
        std::basic_string<TCHAR> new_text = current_player + TEXT(" has won!");

        if (current_player_->text != new_text)
        {
            current_player_->updated = true;
            current_player_->text = new_text;

            delete current_player_->normal_color;
            current_player_->normal_color = new SolidBrush(Color::Black);
        }
    }
    else
    {
        std::basic_string<TCHAR> new_text = TEXT("Your turn, ") + current_player + TEXT("!");

        if (current_player_->text != new_text)
        {
            current_player_->updated = true;
            current_player_->text = new_text;
            delete current_player_->normal_color;
            if (current_player == TEXT("RED"))
            {
                current_player_->normal_color = new SolidBrush(Color::Firebrick);
            }
            else
            {
                current_player_->normal_color = new SolidBrush(Color::RoyalBlue);
            }
        }
    }
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
    switch (current_screen_)
    {
    case Screen::MENU: CalculateLayoutMenu(); break;
    case Screen::GAME: CalculateLayoutGame(); break;
    }
}

void GUI::CalculateLayoutMenu()
{
    title_->center = PointGUI(window_.Width / 2, window_.Height / 2 - 200);
    play_player_button_->center = PointGUI(window_.Width / 2, window_.Height / 2 - 50);
    play_computer_button_->center = PointGUI(window_.Width / 2, window_.Height / 2 + 50);
}

void GUI::CalculateLayoutGame()
{
    INT table_size = 400;
    table_ = Rect(
        (window_.Width - table_size) / 2,
        (window_.Height - table_size) / 2,
        table_size,
        table_size
    );
    cell_size_ = table_.Width / Bonol::kBoardSize;

    current_player_->center = PointGUI(window_.Width / 2, table_.Y - 60);
    skip_button_->center = PointGUI(window_.Width / 2, table_.Y + table_.Height + 20);
    reset_button_->center = PointGUI(window_.Width / 2 + 40, table_.Y - 20);
    undo_button_->center = PointGUI(window_.Width / 2 - 40, table_.Y - 20);
    menu_button_->center = PointGUI(table_.X + 40, table_.Y + table_.Height + 20);

    if (skip_button_->visible != game_state_->turn_move_block_)
    {
        skip_button_->visible = game_state_->turn_move_block_;
        skip_button_->updated = true;
    }

    if (undo_button_->visible != (game_history_->size() > 1))
    {
        undo_button_->visible = game_history_->size() > 1;
        undo_button_->updated = true;
    }

    CalculateCurrentPlayerText();

    /// TODO(@petru): suggestion - specify how many times you can undo ( UNDO(#) | where # is a number)

    //InvalidateTextBoxes();
    game_state_->InvalidateTable();
}

void GUI::InvalidateTextBoxes()
{
    switch (current_screen_)
    {
    case Screen::MENU:
    {
        for (INT counter = 0; counter < kTextBoxesMenuCount; ++counter)
            text_boxes_menu_[counter]->updated = true;
        break;
    }
    case Screen::GAME:
    {
        for (INT counter = 0; counter < kTextBoxesGameCount; ++counter)
            text_boxes_game_[counter]->updated = true;
        break;
    }
    }
}

/// Screen create/delete and screen-specific methods

void GUI::CreateGame()
{
    game_state_ = new Bonol(*this);
    
    current_player_ = new TextBox(
        TEXT(""),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 16),
        Padding(0, 50)
    );
    skip_button_ = new TextBox(
        TEXT("SKIP?"),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 16),
        Padding(),
        new SolidBrush(kTextHover),
        new Font(TEXT("Arial"), 20)
    );
    reset_button_ = new TextBox(
        TEXT("RESET"),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 10),
        Padding(),
        new SolidBrush(kTextHover),
        new Font(TEXT("Arial"), 14)
    );
    undo_button_ = new TextBox(
        TEXT("UNDO"),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 10),
        Padding(),
        new SolidBrush(kTextHover),
        new Font(TEXT("Arial"), 14)
    );
    menu_button_ = new TextBox(
        TEXT("MENU"),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 10),
        Padding(),
        new SolidBrush(kTextHover),
        new Font(TEXT("Arial"), 14)
    );
    /// TODO(@petru): add possible moves count

    is_mouse_down_ = false;
    is_selecting_ = false;
    is_moving_block_ = false;

    repaint_background_ = true;

    game_history_ = new std::stack < Bonol >;
    game_history_->push(Bonol(*this, *game_state_));
}

void GUI::DestroyGame()
{
    delete current_player_, skip_button_, reset_button_, undo_button_, menu_button_;
    while (!game_history_->empty())
    {
        //delete game_history_->top();
        game_history_->pop();
    }
    delete game_history_;
    delete game_state_;
}

void GUI::EndMovingBlockTurn()
{
    game_state_->turn_move_piece_ = true;
    game_state_->turn_move_block_ = false;
    game_state_->ChangePlayer();
    game_state_->DeHighlightBlockedPieces();

    if (game_state_->Over())
    {
        game_state_->turn_move_piece_ = false;

        game_state_->ChangePlayer();
        CalculateCurrentPlayerText();
    }

    game_history_->push(Bonol(*this, *game_state_));
}

void GUI::CreateMenu()
{
    title_ = new TextBox(
        TEXT("BONOL GAME"),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 32),
        Padding(),
        new SolidBrush(kTextColor)/*,
        new Font(TEXT("Comic Sans MS"), 40)*/
    );
    play_player_button_ = new TextBox(
        TEXT(">Multiplayer<"),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 24),
        Padding(),
        new SolidBrush(Color::RoyalBlue),
        new Font(TEXT("Arial"), 28)
    );
    play_computer_button_ = new TextBox(
        TEXT(">Single Player<"),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 24),
        Padding(),
        new SolidBrush(Color::Firebrick),
        new Font(TEXT("Arial"), 28)
    );

    repaint_background_ = true;
}

void GUI::DestroyMenu()
{
    delete title_, play_player_button_, play_computer_button_;
}

/// Window setup and message loop

void GUI::SetWindowDataInfo(HWND hwnd, LPARAM lParam, GUI*& game_interface)
{
    CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
    game_interface = reinterpret_cast<GUI*>(pCreate->lpCreateParams);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)game_interface);
}

void GUI::RunMessageLoop()
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

GUI::GUI(const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow)
    : current_screen_(Screen::MENU)
{
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WindowProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("BonolWindowClassName");

    RegisterClass(&wndClass);

    INT window_position_x = (GetSystemMetrics(SM_CXSCREEN) - window_dimensions.x) / 2;
    INT window_position_y = (GetSystemMetrics(SM_CYSCREEN) - window_dimensions.y) / 2;

    hwnd_ = CreateWindow(
        TEXT("BonolWindowClassName"),   // window class name
        TEXT("Bonol"),                  // window caption
        WS_OVERLAPPEDWINDOW,            // window style
        window_position_x,              // initial x position
        window_position_y,              // initial y position
        window_dimensions.x,            // initial x size
        window_dimensions.y,            // initial y size
        NULL,                           // parent window handle
        NULL,                           // window menu handle
        hInstance,                      // program instance handle
        this);                          // creation parameters

    ShowWindow(hwnd_, nCmdShow);
    UpdateWindow(hwnd_);

    if (!PlaySound(MAKEINTRESOURCE(BACKGROUND_MUSIC), hInstance, SND_RESOURCE | SND_ASYNC | SND_LOOP | SND_NODEFAULT))
    {
        PlaySound(NULL, 0, 0);
    }
    
    RunMessageLoop();

    GdiplusShutdown(gdiplusToken);
}

GUI::TextBox::TextBox(const std::basic_string<TCHAR> TEXT,
                      Brush* NORMAL_COLOR, Font* NORMAL_FONT, Padding PADDING,
                      Brush* HIGHLIGHTED_COLOR, Font* HIGHLIGHTED_FONT)
    : text(TEXT), normal_color(NORMAL_COLOR), normal_font(NORMAL_FONT), padding(PADDING),
      highlighted_color(HIGHLIGHTED_COLOR), highlighted_font(HIGHLIGHTED_FONT),
      updated(true), visible(true), is_hover(false), was_hover(false)
{
    /// TODO(@petru): caution - highlighted_color, highlighted_font can be NULL
}

GUI::TextBox::~TextBox()
{
    if (highlighted_color != NULL)
    {
        delete highlighted_color;
    }
    if (highlighted_font != NULL)
    {
        delete highlighted_font;
    }
    delete normal_font, normal_color;
}
