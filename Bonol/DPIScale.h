/*
    File: DPIScale.cpp
    Written by:
        Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
        Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
    CS1105: Practica - Introducere in programare
    Faculty of Computer Science, UAIC, WINTER 2019

    ABOUT THIS FILE:
    Windows uses pixels to compute onscreen position and distance.
    Direct2D uses DPI.
    This class scales pixels to DPI.
*/

#pragma once

#include <WinUser.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

class DPIScale
{
    static FLOAT scaleX;
    static FLOAT scaleY;

public:
    static void Initialize(ID2D1Factory* pFactory)
    {
        FLOAT dpiX, dpiY;
        pFactory->GetDesktopDpi(&dpiX, &dpiY);
        scaleX = dpiX / 96.0f;
        scaleY = dpiY / 96.0f;
    }

    template <typename T>
    static D2D1_POINT_2F PixelsToDips(T x, T y)
    {
        return D2D1::Point2F(static_cast<FLOAT>(x) / scaleX, static_cast<FLOAT>(y) / scaleY);
    }

    template <typename T>
    static D2D1_POINT_2F DipsToPixels(T x, T y)
    {
        return D2D1::Point2F(static_cast<INT>((x) * scaleX), static_cast<INT>((y) * scaleY));
    }
};

float DPIScale::scaleX = 1.0f;
float DPIScale::scaleY = 1.0f;