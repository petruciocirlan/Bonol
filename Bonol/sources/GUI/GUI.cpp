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

/// Screen create/delete and screen-specific methods

void GUI::CreateGame()
{
    game_state_ = new Bonol(*this);
    
    current_player_ = new TextBox(
        TEXT(""),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 20),
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

    InvalidateTextBoxes();

    is_mouse_down_ = false;
    is_selecting_ = false;
    is_moving_block_ = false;

    selected_cells_count_ = 0;

    force_repaint_background_ = true;

    //game_state_->ChangePlayer();
    //game_state_->FindPcMove(TEXT("HARD"));

    game_history_ = new std::stack < Bonol >;
    game_history_->push(Bonol(*this, *game_state_));
}

void GUI::DestroyGame()
{
    for (INT counter = 0; counter < kTextBoxesGameCount; ++counter)
    {
        delete text_boxes_game_[counter];
        text_boxes_game_[counter] = NULL;
    }

    while (!game_history_->empty())
    {
        //delete game_history_->top();
        game_history_->pop();
    }

    delete game_history_; game_history_ = NULL;
    delete game_state_; game_state_ = NULL;
}

void GUI::EndMovingBlockTurn()
{
    game_state_->MovePieceTurn() = true;
    game_state_->MoveBlockTurn() = false;
    game_state_->ChangePlayer();
    game_state_->DeHighlightBlockedPieces();

    if (game_state_->CheckOver())
    {
        game_state_->MovePieceTurn() = false;

        game_state_->ChangePlayer();
        CalculateCurrentPlayerText();
    }
    else if (current_mode_ != VersusMode::PLAYER)
    {
        if (current_mode_ == VersusMode::COMPUTER_EASY)
        {
            game_state_->FindPcMove(TEXT("EASY"));
        }
        else // if (current_mode_ == VersusMode::COMPUTER_HARD)
        {
            game_state_->FindPcMove(TEXT("HARD"));
        }
        if (game_state_->CheckOver())
        {
            game_state_->MovePieceTurn() = false;

            game_state_->ChangePlayer();
            CalculateCurrentPlayerText();
        }
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
        new SolidBrush(Color::DodgerBlue),
        new Font(TEXT("Arial"), 28)
    );
    play_computer_button_ = new TextBox(
        TEXT(">Single Player<"),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 24),
        Padding(),
        new SolidBrush(Color::PaleVioletRed),
        new Font(TEXT("Arial"), 28)
    );
    easy_computer_button_ = new TextBox(
        TEXT(">Easy<"),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 24),
        Padding(),
        new SolidBrush(Color::DodgerBlue),
        new Font(TEXT("Arial"), 28)
    );
    hard_computer_button_ = new TextBox(
        TEXT(">Hard<"),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 24),
        Padding(),
        new SolidBrush(Color::PaleVioletRed),
        new Font(TEXT("Arial"), 28)
    );

    easy_computer_button_->visible = false;
    hard_computer_button_->visible = false;

    InvalidateTextBoxes();

    player_1_name_ = TEXT(""), player_2_name_ = TEXT("");
    force_repaint_background_ = true;
}

void GUI::DestroyMenu()
{
    for (INT counter = 0; counter < kTextBoxesMenuCount; ++counter)
    {
        delete text_boxes_menu_[counter];
        text_boxes_menu_[counter] = NULL;
    }
}

void GUI::CreateNameSelect(String default_name)
{
    choose_name_ = new TextBox(
        TEXT("Type in your name"),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 24)
    );
    select_button_ = new TextBox(
        TEXT("Enter"),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 20),
        Padding(),
        new SolidBrush(kTextHover),
        new Font(TEXT("Arial"), 24)
    );

    Color name_color = Color::PaleVioletRed;
    if (current_mode_ == VersusMode::PLAYER && player_1_name_ != TEXT(""))
    {
        name_color = Color::DodgerBlue;
    }
    name_type_ = new TextBox(
        default_name,
        new SolidBrush(name_color),
        new Font(TEXT("Arial"), 20),
        Padding(5)
    );
    
    InvalidateTextBoxes();

    force_repaint_background_ = true;
}

void GUI::DestroyNameSelect()
{
    for (INT counter = 0; counter < kTextBoxesNameCount; ++counter)
    {
        delete text_boxes_name_[counter];
        text_boxes_name_[counter] = NULL;
    }
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

void GUI::Initialize()
{
    current_language_ = Language::ENGLISH;
    current_screen_ = Screen::MENU;

    music_toggle_ = new TextBox(
        TEXT("STOP MUSIC"),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 10),
        Padding(),
        new SolidBrush(kTextHover),
        new Font(TEXT("Arial"), 14)
    );
    is_playing_music_ = true;

    HINSTANCE hInstance = GetModuleHandle(NULL);
    if (!PlaySound(MAKEINTRESOURCE(BACKGROUND_MUSIC), hInstance, SND_RESOURCE | SND_ASYNC | SND_LOOP | SND_NODEFAULT))
    {
        // something went wrong
        PlaySound(NULL, 0, 0);
        is_playing_music_ = false;
        music_toggle_->text = TEXT("PLAY MUSIC");
    }
}

void GUI::Destroy()
{
    switch (current_screen_)
    {
    case Screen::MENU: DestroyMenu(); break;
    case Screen::GAME: DestroyGame(); break;
    }

    delete music_toggle_; music_toggle_ = NULL;

    PlaySound(NULL, 0, 0);
}

GUI::GUI(const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow)
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

GUI::TextBox::TextBox(const String TEXT,
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
        delete highlighted_color; highlighted_color = NULL;
    }
    if (highlighted_font != NULL)
    {
        delete highlighted_font; highlighted_font = NULL;
    }
    delete normal_font; normal_font = NULL;
    delete normal_color; normal_color = NULL;
}
