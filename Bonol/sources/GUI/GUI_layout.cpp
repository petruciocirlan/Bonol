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

Rect GUI::MakeRect(PointGUI origin, PointGUI dimensions) const
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
    PointGUI text_box_dimensions((INT)bounds.Width, (INT)bounds.Height);
    PointGUI text_box_origin(box.center.x - (INT)bounds.Width / 2, box.center.y - (INT)bounds.Height / 2);
    box.rect = MakeRect(text_box_origin, text_box_dimensions);
}

void GUI::CalculateCurrentPlayerText()
{
    String current_player = game_state_->GetActivePlayerName();
    if (game_state_->Over())
    {
        String new_text = current_player + TEXT(" has won!");

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
        String new_text = TEXT("Your turn, ") + current_player + TEXT("!");

        if (current_player_->text != new_text)
        {
            current_player_->updated = true;
            current_player_->text = new_text;
            delete current_player_->normal_color;
            if (current_player == TEXT("RED"))
            {
                current_player_->normal_color = new SolidBrush(Color::PaleVioletRed);
            }
            else
            {
                current_player_->normal_color = new SolidBrush(Color::DodgerBlue);
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
    }
}

void GUI::CalculateLayoutMenu()
{
    title_->center = PointGUI(window_.Width / 2, window_.Height / 2 - 200);
    play_player_button_->center = PointGUI(window_.Width / 2, window_.Height / 2 - 50);
    play_computer_button_->center = PointGUI(window_.Width / 2, window_.Height / 2 + 50);
    easy_computer_button_->center = PointGUI(window_.Width / 2 - 100, window_.Height / 2 + 125);
    hard_computer_button_->center = PointGUI(window_.Width / 2 + 100, window_.Height / 2 + 125);
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
    }
}
