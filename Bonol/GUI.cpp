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


LRESULT Bonol::GUI::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
        game_interface->DiscardGraphicsResources();
        SafeRelease(&game_interface->pFactory_);
        PostQuitMessage(0);
        return 0;
    }
    case WM_PAINT:
    {
        game_interface->OnPaint();
        ValidateRect(hwnd, NULL);
        return 0;
    }
    case WM_SIZE:
    {
        game_interface->Resize();
        return 0;
    }
    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
        lpMMI->ptMinTrackSize.x = 400;
        lpMMI->ptMinTrackSize.y = 400;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Bonol::GUI::GUIPos Bonol::GUI::GetTableCenter() const
{
    return GUIPos(width_ / 2, height_ / 2);
}

Bonol::GUI::GUIPos Bonol::GUI::GetTableOrigin() const
{
    return GUIPos((width_ - table_width_) / 2, (height_ - table_width_) / 2);
}

void Bonol::GUI::DrawTable() const
{
    for (CellCoord line = 0; line < kBoardSize; ++line)
        for (CellCoord column = 0; column < kBoardSize; ++column)
            DrawCell(CellPos(column, line));
}

/// BELOW: Window setup and message loop

void Bonol::GUI::SetWindowDataInfo(HWND hwnd, LPARAM lParam, GUI*& game_interface)
{
    CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
    game_interface = reinterpret_cast<Bonol::GUI*>(pCreate->lpCreateParams);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)game_interface);

    if (FAILED(D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED, &game_interface->pFactory_)))
    {
        // Fail CreateWindowEx.
        /// TODO(petru): throw something
        return;
    }

    RECT rc;
    GetClientRect(hwnd, &rc);
}

void Bonol::GUI::CreateInterface(const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Bonol Window Class";

    wc_.lpfnWndProc = WindowProc;
    wc_.hInstance = hInstance;
    wc_.lpszClassName = CLASS_NAME;

    RegisterClass(&wc_);

    // Create the window.
    hwnd_ = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Bonol",                       // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT,
        window_dimensions.x, window_dimensions.y,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        this        // Additional application data
    );

    if (hwnd_ == NULL)
    {
        /// TODO(petru): throw error
        return;
    }

    ShowWindow(hwnd_, nCmdShow);
}

void Bonol::GUI::RunMessageLoop()
{
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

Bonol::GUI::GUI(const Bonol* game, const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow)
    : kGameState(*game), wc_({}), pFactory_(NULL), pRenderTarget_(NULL)//, pBrush_(NULL)
{
    CreateInterface(window_dimensions, hInstance, nCmdShow);

    RunMessageLoop();
}
