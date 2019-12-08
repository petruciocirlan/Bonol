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

void Bonol::GUI::UpdateDimensions(const RECT& rc)
{
    width_  = rc.right - rc.left;
    height_ = rc.bottom - rc.top;

    table_width_ = min(width_, height_) / 2;
    cell_width_  = table_width_ / kBoardSize;

    center_ = Position(width_ / 2, height_ / 2);
    origin_ = Position((width_ - table_width_) / 2, (height_ - table_width_) / 2);
}

LRESULT Bonol::GUI::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    GUI* game_interface;

    if (uMsg == WM_CREATE)
    {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        game_interface = reinterpret_cast<Bonol::GUI*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)game_interface);

        if (FAILED(D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, &game_interface->pFactory_)))
        {
            return -1;  // Fail CreateWindowEx.
        }

        RECT rc;
        GetClientRect(hwnd, &rc);
        (*game_interface).UpdateDimensions(rc);

        return 0;
    }
    else
    {
        game_interface = (GUI*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    switch (uMsg)
    {
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
        (*game_interface).DiscardGraphicsResources();
        SafeRelease(&game_interface->pFactory_);
        PostQuitMessage(0);
        return 0;
    }
    case WM_PAINT:
    {
        (*game_interface).OnPaint();
        ValidateRect(hwnd, NULL);
        return 0;
    }
    case WM_SIZE:
    {
        (*game_interface).Resize();
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Bonol::GUI::GUI(const Bonol* game, const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow)
    : kGameState(*game), wc_({}), pFactory_(NULL), pRenderTarget_(NULL)//, pBrush_(NULL)
{
    CreateInterface(window_dimensions, hInstance, nCmdShow);

    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
