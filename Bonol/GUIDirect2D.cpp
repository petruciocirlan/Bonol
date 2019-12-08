/*
    File: GUIDirect2D.cpp
    Written by:
        Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
        Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
    CS1105: Practica - Introducere in programare
    Faculty of Computer Science, UAIC, WINTER 2019

    ABOUT THIS FILE:
    Methods that use the Direct2D library in the GUI.
*/

#include "GUI.h"

void Bonol::GUI::DrawLine(const Position from, const Position to) const
{
    //line(from.x, from.y, to.x, to.y);
}

void Bonol::GUI::DrawSquare(const Position origin, const INT width, const D2D1_COLOR_F color) const
{
    Position pos(origin_.x + origin.x * cell_width_, origin_.y + origin.y * cell_width_);
    ID2D1SolidColorBrush* pBrush = NULL;
    pRenderTarget_->CreateSolidColorBrush(color, &pBrush);

    if (pBrush != NULL)
    {
        pRenderTarget_->FillRectangle(
            D2D1::RectF(
                (FLOAT)pos.x,
                (FLOAT)pos.y,
                (FLOAT)pos.x + width,
                (FLOAT)pos.y + width
            ),
            pBrush
        );
    }
}

void Bonol::GUI::DrawCell(const Position pos) const
{
    auto cell_piece = kGameState.GetCellPiece(pos);
    switch (cell_piece)
    {
    case Piece::FREE:
    {
        DrawSquare(pos, cell_width_, D2D1::ColorF(D2D1::ColorF::Gray));
        break;
    }
    case Piece::RED:
    {
        DrawSquare(pos, cell_width_, D2D1::ColorF(D2D1::ColorF::Red));
        break;
    }
    case Piece::BLUE:
    {
        DrawSquare(pos, cell_width_, D2D1::ColorF(D2D1::ColorF::Blue));
        break;
    }
    case Piece::BLOCKED:
    {
        DrawSquare(pos, cell_width_, D2D1::ColorF(D2D1::ColorF::Black));
        break;
    }
    case Piece::RED_SELECTED:
    {
        DrawSquare(pos, cell_width_, D2D1::ColorF(D2D1::ColorF::DarkRed));
        break;
    }
    case Piece::BLUE_SELECTED:
    {
        DrawSquare(pos, cell_width_, D2D1::ColorF(D2D1::ColorF::DarkBlue));
        break;
    }
    case Piece::BLOCKED_SELECTED:
    {
        DrawSquare(pos, cell_width_, D2D1::ColorF(D2D1::ColorF::Green));
        break;
    }
    }
}

void Bonol::GUI::DrawTable() const
{
    for (CellCoord line = 0; line < kBoardSize; ++line)
        for (CellCoord column = 0; column < kBoardSize; ++column)
            DrawCell(Position(column, line));
}

void Bonol::GUI::CalculateLayout()
{
    if (pRenderTarget_ != NULL)
    {
        D2D1_SIZE_F size = pRenderTarget_->GetSize();
        const FLOAT x = size.width / 2;
        const FLOAT y = size.height / 2;
        const FLOAT radius = min(x, y);
        //ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
    }
}

HRESULT Bonol::GUI::CreateGraphicsResources()
{
    HRESULT hr = S_OK;
    if (pRenderTarget_ == NULL)
    {
        RECT rc;
        GetClientRect(hwnd_, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        hr = pFactory_->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hwnd_, size),
            &pRenderTarget_);
    }
    return hr;
}

void Bonol::GUI::DiscardGraphicsResources()
{
    SafeRelease(&pRenderTarget_);
    //SafeRelease(&pBrush_);
}

void Bonol::GUI::OnPaint()
{
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(hwnd_, &ps);

        pRenderTarget_->BeginDraw();

        // All painting occurs here, between BeginPaint and EndPaint.
        pRenderTarget_->Clear(D2D1::ColorF(D2D1::ColorF::White));

        DrawTable();

        hr = pRenderTarget_->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources();
        }
        EndPaint(hwnd_, &ps);
    }
}

void Bonol::GUI::Resize()
{
    if (pRenderTarget_ != NULL)
    {
        RECT rc;
        GetClientRect(hwnd_, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        pRenderTarget_->Resize(size);
        CalculateLayout();
        InvalidateRect(hwnd_, NULL, FALSE);
    }
}