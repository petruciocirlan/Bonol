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
	using GUICoord = FLOAT;
	struct GUIPos;

	GUICoord width_, height_;
	GUICoord table_width_, cell_width_;

	HWND hwnd_;
	WNDCLASS wc_;
	const Bonol& kGameState;

	ID2D1Factory* pFactory_;
	ID2D1HwndRenderTarget* pRenderTarget_;
	//ID2D1SolidColorBrush*	pBrush_;

	template<class Interface>
	static inline void SafeRelease(Interface** ppInterfaceToRelease);

	inline void CalculateLayout();
	HRESULT CreateGraphicsResources();
	void DiscardGraphicsResources();
	void OnPaint();
	void Resize();

	static void SetWindowDataInfo(HWND hwnd, LPARAM lParam, GUI*& game_interface);
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void CreateInterface(const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow);
	void RunMessageLoop();

	GUIPos GetTableCenter() const;
	GUIPos GetTableOrigin() const;

	void DrawLine(const GUIPos from, const GUIPos to) const;
	void DrawSquare(const GUIPos origin, const FLOAT width, const D2D1_COLOR_F color) const;
	void DrawCell(const CellPos pos) const;
	void DrawTable() const;

public:
	GUI(const Bonol* game, const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow);
};

struct Bonol::GUI::GUIPos
{
	GUICoord x, y;

	GUIPos() : x(0), y(0) {};
	GUIPos(GUICoord x_pos, GUICoord y_pos) : x(x_pos), y(y_pos) {};
};

template<class Interface>
static inline void Bonol::GUI::SafeRelease(Interface** ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}
