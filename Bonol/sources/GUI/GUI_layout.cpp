/*
    File: GUI_layout.cpp
    Written by:
        Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
        Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
    CS1105: Practica - Introducere in programare
    Faculty of Computer Science, UAIC, WINTER 2019

    ABOUT THIS FILE:
    Layout computing.
*/

#include "../../headers/GUI.h"

Rect GUI::MakeRect(PointGUI origin, Dimensions dimensions) const
{
    return Rect(origin.x, origin.y, dimensions.x, dimensions.y);
}

Rect GUI::InflateRect(const Rect rect, Padding padding) const
{
    return Rect(
        rect.X - padding.left,
        rect.Y - padding.top,
        rect.Width + padding.left + padding.right,
        rect.Height + padding.top + padding.bottom
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
    Font* font = box.normal_font;
    if (box.is_hover && box.highlighted_font != NULL)
    {
        font = box.highlighted_font;
    }
    graphics_->MeasureString(box.text.data(), -1, font, PointF(0, 0), &bounds);
    Dimensions text_box_dimensions((INT)bounds.Width, (INT)bounds.Height);
    PointGUI text_box_origin(box.center.x - (INT)bounds.Width / 2, box.center.y - (INT)bounds.Height / 2);
    box.rect = MakeRect(text_box_origin, text_box_dimensions);
}

void GUI::CalculateCurrentPlayerText()
{
    //String current_player = game_state_->GetActivePlayerName();
    String player_name = player_1_name_;
    Color player_color = player_1_color_;
    if (game_state_->GetActivePlayer() == Bonol::Piece::BLUE)
    {
        player_name = player_2_name_;
        player_color = player_2_color_;
    }

    if (game_state_->IsOver())
    {
        String new_text = player_name + TEXT(" has won!");

        if (current_player_->text != new_text)
        {
            current_player_->updated = true;
            current_player_->text = new_text;

            delete current_player_->normal_color;
            current_player_->normal_color = new SolidBrush(ChangeColorBrightness(player_color, 0.6f));
        }
    }
    else
    {
        String new_text = TEXT("Your turn, ") + player_name + TEXT("!");
        if (current_mode_ != VersusMode::PLAYER && game_state_->GetActivePlayer() == Bonol::Piece::BLUE)
        {
            new_text = TEXT("It's the COMPUTER's turn...");
        }

        if (current_player_->text != new_text)
        {
            current_player_->updated = true;
            current_player_->text = new_text;
            delete current_player_->normal_color;
            if (player_name == player_1_name_)
            {
                current_player_->normal_color = new SolidBrush(player_1_color_);
            }
            else
            {
                current_player_->normal_color = new SolidBrush(player_2_color_);
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

    music_toggle_->center = PointGUI(window_.Width / 2, window_.Height / 2 + 260);

    switch (current_screen_)
    {
    case Screen::MENU: CalculateLayoutMenu(); break;
    case Screen::GAME: CalculateLayoutGame(); break;
    case Screen::NAME_SELECT: CalculateLayoutNameSelect(); break;
    case Screen::LEADERBOARD: CalculateLayoutLeaderboard(); break;
    }
}

void GUI::CalculateLayoutMenu()
{
    title_->center = PointGUI(window_.Width / 2, window_.Height / 2 - 200);
    leaderboard_button_->center = PointGUI(window_.Width / 2, window_.Height / 2 - 100);
    play_player_button_->center = PointGUI(window_.Width / 2, window_.Height / 2);
    play_computer_button_->center = PointGUI(window_.Width / 2, window_.Height / 2 + 100);
    easy_computer_button_->center = PointGUI(window_.Width / 2 - 100, window_.Height / 2 + 175);
    hard_computer_button_->center = PointGUI(window_.Width / 2 + 100, window_.Height / 2 + 175);
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
    reset_button_->center = PointGUI(window_.Width / 2 + cell_size_ / 2, table_.Y - 20);
    undo_button_->center = PointGUI(window_.Width / 2 - cell_size_ / 2, table_.Y - 20);
    menu_button_->center = PointGUI(table_.X + cell_size_ / 2, table_.Y + table_.Height + 20);

    if (skip_button_->visible != game_state_->MoveBlockTurn())
    {
        skip_button_->visible = game_state_->MoveBlockTurn();
        skip_button_->updated = true;
    }

    if (undo_button_->visible != (game_history_->size() > 1))
    {
        undo_button_->visible = game_history_->size() > 1;
        undo_button_->updated = true;
    }

    if (reset_button_->visible != (game_history_->size() > 1))
    {
        reset_button_->visible = game_history_->size() > 1;
        reset_button_->updated = true;
    }

    CalculateCurrentPlayerText();

    /// TODO(@petru): suggestion - specify how many times you can undo ( UNDO(#) | where # is a number)

    //InvalidateBoxes();
    game_state_->InvalidateTable();
}

void GUI::CalculateLayoutNameSelect()
{
    choose_name_->center = PointGUI(window_.Width / 2, window_.Height / 2 - 180);
    name_type_->center = PointGUI(window_.Width / 2, window_.Height / 2 - 90);
    select_button_->center = PointGUI(window_.Width / 2, window_.Height / 2 + 120);

    red_box_->center = PointGUI(window_.Width / 2 - 80, window_.Height / 2);
    green_box_->center = PointGUI(window_.Width / 2, window_.Height / 2);
    blue_box_->center = PointGUI(window_.Width / 2 + 80, window_.Height / 2);
}

void GUI::CalculateLayoutLeaderboard()
{
    leaderboard_->center = PointGUI(window_.Width / 2, window_.Height / 2 - 50);
    back_button_->center = PointGUI(window_.Width / 2, window_.Height / 2 + 220);
}

void GUI::InvalidateBoxes()
{
    for (INT counter = 0; counter < kTextBoxesGlobalCount; ++counter)
        text_boxes_global_[counter]->updated = true;

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
    case Screen::NAME_SELECT:
    {
        for (INT counter = 0; counter < kTextBoxesNameSelectCount; ++counter)
            text_boxes_name_select_[counter]->updated = true;
        for (INT counter = 0; counter < kColorBoxesCount; ++counter)
            color_boxes_[counter]->updated = true;
        break;
    }
    case Screen::LEADERBOARD:
    {
        for (INT counter = 0; counter < kTextBoxesLeaderboardCount; ++counter)
            text_boxes_leaderboard_[counter]->updated = true;
        break;
    }
    }
}
