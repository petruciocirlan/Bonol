/*
	File: GUI.h
	Written by:
		Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
		Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
	CS1105: Practica - Introducere in programare
	Faculty of Computer Science, UAIC, WINTER 2019

	ABOUT THIS FILE:
	Interface of the GUI of the Bonol (L game).
*/

#pragma once

#include "Bonol.h"

/// interface
#include <d2d1.h>
#pragma comment(lib, "d2d1")

class Bonol::GUI
{
private:
	INT	width_, height_;
	INT	table_width_, cell_width_;

	WNDCLASS		wc_;
	HWND			hwnd_;
	const Bonol&	kGameState;
	Position		center_, origin_;

	ID2D1Factory*			pFactory_;
	ID2D1HwndRenderTarget*	pRenderTarget_;
	//ID2D1SolidColorBrush*	pBrush_;

	template <class T> static void SafeRelease(T** ppT);
	void    CalculateLayout();
	HRESULT CreateGraphicsResources();
	void    DiscardGraphicsResources();
	void    OnPaint();
	void    Resize();

	void	CreateInterface(const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow);
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void	UpdateDimensions(const RECT& rc);

	void	DrawLine	(const Position from, const Position to)							const;
	void	DrawSquare	(const Position origin, const INT width, const D2D1_COLOR_F color)	const;
	void	DrawCell	(const Position pos)												const;
	void	DrawTable()	const;

public:
	GUI(const Bonol* game, const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow);
};

template<class T>
inline void Bonol::GUI::SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
