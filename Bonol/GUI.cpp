/*
    File: GUI.cpp
    Written by:
        Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
        Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
    CS1105: Practica - Introducere in programare
    Faculty of Computer Science, UAIC, WINTER 2019

    ABOUT THIS FILE:
    Implementation of the GUI of the Bonol (L game).
*/

#include "GUI.h"

void Bonol::GUI::DrawLine(const Position from, const Position to) const
{
    line(from.x, from.y, to.x, to.y);
}

void Bonol::GUI::DrawSquare(const Position origin, const int width) const
{
    Position upper_left_corner  = Position(origin.x, origin.y);
    Position upper_right_corner = Position(origin.x + width, origin.y);
    Position lower_left_corner  = Position(origin.x, origin.y + width);
    Position lower_right_corner = Position(origin.x + width, origin.y + width);

    DrawLine(upper_left_corner, upper_right_corner);
    DrawLine(upper_right_corner, lower_right_corner);
    DrawLine(lower_right_corner, lower_left_corner);
    DrawLine(lower_left_corner, upper_left_corner);
}

void Bonol::GUI::GetPolyPointsFromCell(Position pos, Position PolyPoints[4]) const
{
    int cell_left = origin_.x + pos.x * cell_width_;
    int cell_top = origin_.y + pos.y * cell_width_;

    PolyPoints[0] = Position(cell_left, cell_top);
    PolyPoints[1] = Position(cell_left + cell_width_, cell_top);
    PolyPoints[2] = Position(cell_left + cell_width_, cell_top + cell_width_);
    PolyPoints[3] = Position(cell_left, cell_top + cell_width_);
}

void Bonol::GUI::DrawCell(const Position pos) const
{
    int numpoints = 4;
    Position polypoints[4];
    GetPolyPointsFromCell(pos, polypoints);

    auto cell_piece = game_state_.GetCell(pos);
    switch (cell_piece)
    {
    case PIECE_FREE:
    {
        //setcolor(getbkcolor());
        setfillstyle(SOLID_FILL, getbkcolor());
        break;
    }
    case PIECE_RED:
    {
        //setcolor(RED);
        setfillstyle(SOLID_FILL, LIGHTRED);
        break;
    }
    case PIECE_BLUE:
    {
        //setcolor(BLUE);
        setfillstyle(SOLID_FILL, LIGHTBLUE);
        break;
    }
    case PIECE_BLOCKED:
    {
        //setcolor(WHITE);
        setfillstyle(SOLID_FILL, WHITE);
        break;
    }
    }

    setcolor(WHITE);
    fillpoly(numpoints, (int*)polypoints);
}

Bonol::GUI::GUI(const Bonol* game, const Dimensions dim) : game_state_(*game)
{
    width_ = dim.x, height_ = dim.y;
    left_ = (getmaxwidth() - width_) / 2;
    top_ = (getmaxheight() - height_) / 2;
    initwindow(width_, height_, "BONOL", left_, top_);

    table_width_ = 400;
    cell_width_ = table_width_ / kBoardSize;

    center_ = Position(width_ / 2, height_ / 2);
    origin_ = Position((width_ - table_width_) / 2, (height_ - table_width_) / 2);

    setbkcolor(BLACK);
}

void Bonol::GUI::DrawScreen() const
{
    for (int line = 0; line < kBoardSize; ++line)
        for (int column = 0; column < kBoardSize; ++column)
                DrawCell(Position(column, line));

    DrawSquare(origin_, table_width_);
}