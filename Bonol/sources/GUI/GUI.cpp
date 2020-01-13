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

    InvalidateBoxes();

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

        String winner = player_1_name_;
        if (game_state_->GetActivePlayer() == Bonol::Piece::BLUE)
        {
            winner = player_2_name_;
        }
        IncreaseLeaderboardScore(winner);

        game_history_->push(Bonol(*this, *game_state_));
    }
    else if (current_mode_ != VersusMode::PLAYER)
    {
        PostMessage(hwnd_, WM_PAINT, 0, 0);
        PostMessage(hwnd_, WM_PC_MOVE, 0, 0);
    }
    else
    {
        game_history_->push(Bonol(*this, *game_state_));
    }

}

void GUI::ComputerTurn()
{
    Sleep(750);
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

        if (game_state_->GetActivePlayer() == Bonol::Piece::RED)
        {
            String winner = player_1_name_;
            IncreaseLeaderboardScore(winner);
        }
    }

    game_history_->push(Bonol(*this, *game_state_));
    InvalidateRect(hwnd_, 0, TRUE);
}

void GUI::IncreaseLeaderboardScore(const String player_name) const
{
    std::map < String, INT > scores;

    std::basic_ifstream<TCHAR> leaderboard_input;
    leaderboard_input.open("leaderboard");

    String head;
    getline(leaderboard_input, head);

    while (!leaderboard_input.eof())
    {
        String name;
        leaderboard_input >> name;
        if (name.size() == 0)
        {
            break;
        }

        INT score;
        leaderboard_input >> score;

        scores[name] = score;
    }

    leaderboard_input.close();
    
    ++scores[player_name];

    std::basic_ofstream<TCHAR> leaderboard_output;
    leaderboard_output.open("leaderboard");

    leaderboard_output << TEXT("PLAYER NAME\tSCORE\n");

    std::priority_queue < std::pair < INT, String > > ranking;
    for (auto it = scores.cbegin(); it != scores.cend(); ++it)
    {
        ranking.push(std::make_pair(it->second, it->first));
    }

    while (!ranking.empty())
    {
        INT score = ranking.top().first;
        String name = ranking.top().second;
        ranking.pop();

        if (name.size() < 8)
        {
            name.append(8 - name.size(), TEXT(' '));
        }

        leaderboard_output << name << TEXT('\t') << score << TEXT('\n');
    }

    leaderboard_output.close();
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
    leaderboard_button_ = new TextBox(
        TEXT(">Leaderboard<"),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 24),
        Padding(),
        new SolidBrush(Color::PaleVioletRed),
        new Font(TEXT("Arial"), 28)
    );
    play_player_button_ = new TextBox(
        TEXT(">Multiplayer<"),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 24),
        Padding(),
        new SolidBrush(Color::LimeGreen),
        new Font(TEXT("Arial"), 28)
    );
    play_computer_button_ = new TextBox(
        TEXT(">Single Player<"),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 24),
        Padding(),
        new SolidBrush(Color::DodgerBlue),
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

    InvalidateBoxes();

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

void GUI::CreateNameSelect(const String default_name)
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

    INT color_box_width = 50;
    red_box_ = new ColorBox(
        Color::PaleVioletRed,
        color_box_width
    );
    green_box_ = new ColorBox(
        Color::LimeGreen,
        color_box_width
    );
    blue_box_ = new ColorBox(
        Color::DodgerBlue,
        color_box_width
    );

    Color name_color;
    if (current_mode_ == VersusMode::PLAYER && player_1_name_ != TEXT(""))
    {
        for (INT counter = 0; counter < kColorBoxesCount; ++counter)
        {
            if (color_boxes_[counter]->color.GetValue() == player_1_color_.GetValue())
            {
                color_boxes_[counter]->disabled = true;
                break;
            }
        }
        for (INT counter = 0; counter < kColorBoxesCount; ++counter)
        {
            if (color_boxes_[counter]->disabled == false)
            {
                color_boxes_[counter]->selected = true;
                name_color = color_boxes_[counter]->color;
                break;
            }
        }
    }
    else
    {
        name_color = color_boxes_[0]->color;
        color_boxes_[0]->selected = true;
    }

    name_type_ = new TextBox(
        default_name,
        new SolidBrush(name_color),
        new Font(TEXT("Arial"), 20),
        Padding(5)
    );
    
    InvalidateBoxes();

    force_repaint_background_ = true;
}

void GUI::DestroyNameSelect()
{
    for (INT counter = 0; counter < kTextBoxesNameSelectCount; ++counter)
    {
        delete text_boxes_name_select_[counter];
        text_boxes_name_select_[counter] = NULL;
    }
}

void GUI::CreateLeaderboard()
{
    String leaderboard_content = GetLeaderboardContent();

    leaderboard_ = new TextBox(
        leaderboard_content,
        new SolidBrush(kTextColor),
        new Font(TEXT("Consolas"), 22)
    );

    back_button_ = new TextBox(
        TEXT("BACK"),
        new SolidBrush(kTextColor),
        new Font(TEXT("Arial"), 14),
        Padding(),
        new SolidBrush(kTextHover),
        new Font(TEXT("Arial"), 18)
    );

    InvalidateBoxes();

    force_repaint_background_ = true;
}

void GUI::DestroyLeaderboard()
{
    for (INT counter = 0; counter < kTextBoxesLeaderboardCount; ++counter)
    {
        delete text_boxes_leaderboard_[counter];
        text_boxes_leaderboard_[counter] = NULL;
    }
}

GUI::String GUI::GetLeaderboardContent() const
{
    std::basic_stringstream<TCHAR> leaderboard_content;

    std::basic_ifstream<TCHAR> leaderboard_input;
    leaderboard_input.open("leaderboard");

    String head;
    getline(leaderboard_input, head);
    if (head.size() == 0)
    {
        leaderboard_input.close();

        std::basic_ofstream<TCHAR> leaderboard_output;
        leaderboard_output.open("leaderboard");
        leaderboard_output << TEXT("PLAYER NAME\tSCORE\n");
        leaderboard_output.close();

        leaderboard_input.open("leaderboard");
        getline(leaderboard_input, head);
    }

    leaderboard_content << head;

    INT player_count = 0;
    while (!leaderboard_input.eof() && player_count < 10)
    {
        String name;
        leaderboard_input >> name;
        if (name.size() == 0)
        {
            break;
        }

        INT score;
        leaderboard_input >> score;

        if (name.size() < 8)
        {
            name += String(8 - name.size(), TEXT('_'));
        }

        leaderboard_content << TEXT('\n') << name << TEXT('\t') << std::setw(5) << std::setfill(TEXT('_')) << score;
        ++player_count;
    }

    while (player_count < 10)
    {
        leaderboard_content << TEXT('\n') << String(8, TEXT('_')) << TEXT("\t_____");
        ++player_count;
    }

    leaderboard_input.close();

    return leaderboard_content.str();
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
