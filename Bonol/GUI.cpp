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
        game_interface->CalculateLayout();
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
        game_interface->OnMoveMouse(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        
        return 0;
    }
    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
        lpMMI->ptMinTrackSize.x = 480;
        lpMMI->ptMinTrackSize.y = 480;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Bonol::GUI::PosGUI Bonol::GUI::GetTableCenter() const
{
    return PosGUI(width_ / 2, height_ / 2);
}

Bonol::GUI::PosGUI Bonol::GUI::GetTableOrigin() const
{
    return PosGUI((width_ - table_width_) / 2, (height_ - table_width_) / 2);
}

BOOL Bonol::GUI::IsInsideTable(const PosGUI pos) const
{
    PosGUI top_left = GetTableOrigin();
    PosGUI bottom_right = PosGUI(top_left.x + table_width_, top_left.y + table_width_);
    if (pos.x < top_left.x || pos.y < top_left.y)
    {
        return FALSE;
    }
    if (pos.x > bottom_right.x || pos.y > bottom_right.y)
    {
        return FALSE;
    }
    return TRUE;
}

/// BELOW: Window setup and message loop

void Bonol::GUI::SetWindowDataInfo(HWND hwnd, LPARAM lParam, GUI*& game_interface)
{
    CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
    game_interface = reinterpret_cast<Bonol::GUI*>(pCreate->lpCreateParams);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)game_interface);
}

void Bonol::GUI::CreateInterface(const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow)
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

    hwnd_ = CreateWindow(
        TEXT("BonolWindowClassName"),   // window class name
        TEXT("Bonol"),  // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        window_dimensions.x,      // initial x size
        window_dimensions.y,      // initial y size
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        hInstance,                // program instance handle
        this);                    // creation parameters

    ShowWindow(hwnd_, nCmdShow);
    UpdateWindow(hwnd_);
}

void Bonol::GUI::RunMessageLoop()
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

Bonol::GUI::GUI(const Bonol* game, const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow)
    : kGameState(*game)
{
    CreateInterface(window_dimensions, hInstance, nCmdShow);

    RunMessageLoop();
}
