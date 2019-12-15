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

LRESULT GUI::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
        game_interface->Initialize();
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
        PointGUI mouse_pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        game_interface->OnMoveMouse(mouse_pos);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        PointGUI mouse_pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        game_interface->OnLeftClickPress(mouse_pos);
        return 0;
    }
    case WM_LBUTTONUP:
    {
        PointGUI mouse_pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        game_interface->OnLeftClickRelease(mouse_pos);
        return 0;
    }
    case WM_KEYDOWN:
    {
        if (GetKeyState(VK_ESCAPE) & 0x8000)
        {
            if (MessageBox(hwnd, L"Really quit?", L"Bonol", MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(hwnd);
            }
            return 0;
        }
        break;
    }
    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
        lpMMI->ptMinTrackSize.x = 480;
        lpMMI->ptMinTrackSize.y = 600;
        return 0;
    }
    case WM_ERASEBKGND:
        return 1;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
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

/// BELOW: Window setup and message loop

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
    game_state_ = new Bonol(*this);
    CalculateLayout();
    is_mouse_down_ = false;
    is_selecting_ = false;
    is_moving_block_ = false;
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
