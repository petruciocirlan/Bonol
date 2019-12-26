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

#include "GUI.h"

Rect GUI::MakeRect(PointGUI origin, PointGUI dimensions) const
{
    return Rect(origin.x, origin.y, dimensions.x, dimensions.y);
}

Rect GUI::InflateRect(const Rect rect, INT padding) const
{
    /// TODO(@petru): test this
    return Rect(
        rect.X - padding,
        rect.Y - padding,
        rect.Width + 2 * padding - 1,
        rect.Height + 2 * padding - 1
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
    graphics_->MeasureString(box.text.data(), -1, box.font, PointF(0, 0), &bounds);
    PointGUI text_box_dimensions((INT)bounds.Width, (INT)bounds.Height);
    PointGUI text_box_origin(box.center.x - (INT)bounds.Width / 2, box.center.y - (INT)bounds.Height / 2);
    box.rect = MakeRect(text_box_origin, text_box_dimensions);
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
    play_button_->center = PointGUI(window_.Width / 2, window_.Height / 2);
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
    reset_button_->center = PointGUI(window_.Width / 2 - 40, table_.Y - 20);
    menu_button_->center = PointGUI(window_.Width / 2 + 40, table_.Y - 20);

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

/// BELOW: Window setup and message loop

void GUI::CreateGame()
{
    game_state_ = new Bonol(*this);
    
    current_player_ = new TextBox(
        TEXT("Your turn, RED!"),
        new Font(TEXT("Arial"), 16),
        new SolidBrush(kTextColor)
    );
    skip_button_ = new TextBox(
        TEXT("SKIP?"),
        new Font(TEXT("Arial"), 16),
        new SolidBrush(kTextColor)
    );
    reset_button_ = new TextBox(
        TEXT("RESET"),
        new Font(TEXT("Arial"), 10),
        new SolidBrush(kTextColor)
    );
    menu_button_ = new TextBox(
        TEXT("MENU"),
        new Font(TEXT("Arial"), 10),
        new SolidBrush(kTextColor)
    );

    CalculateLayout();

    turn_move_piece_ = true;
    turn_move_block_ = false;

    is_mouse_down_ = false;
    is_selecting_ = false;
    is_moving_block_ = false;

    skip_button_->visible = false;
    repaint_background_ = true;
}

void GUI::DestroyGame()
{
    delete game_state_;
    delete current_player_, skip_button_, reset_button_, menu_button_;
}

void GUI::CreateMenu()
{
    title_ = new TextBox(
        TEXT("BONOL GAME"),
        new Font(TEXT("Arial"), 32),
        new SolidBrush(kTextColor)
    );
    play_button_ = new TextBox(
        TEXT("Play!"),
        new Font(TEXT("Arial"), 24),
        new SolidBrush(kTextColor)
    );

    repaint_background_ = true;
}

void GUI::DestroyMenu()
{
    delete title_, play_button_;
}

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

    RunMessageLoop();

    GdiplusShutdown(gdiplusToken);
}

GUI::TextBox::~TextBox()
{
    delete font, color;
}
