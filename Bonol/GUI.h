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

//#include <stdafx.h>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

class Bonol::GUI
{
private:
	using CoordGUI = INT;
	struct PosGUI;

	CoordGUI width_, height_;
	CoordGUI table_width_, cell_width_;
	PosGUI* mouse_;

	const Bonol& kGameState;
	HWND hwnd_;
	Graphics* graphics_;

	template<class Interface>
	static inline void SafeRelease(Interface** ppInterfaceToRelease);

	inline void CalculateLayout();
	void OnMoveMouse(INT pixelX, INT pixelY);
	void OnPaint();
	void Resize();

	PosGUI GetTableCenter() const;
	PosGUI GetTableOrigin() const;
	BOOL IsInsideTable(const PosGUI pos) const;

	void DrawLine(const PosGUI from, const PosGUI to) const;
	void DrawSquare(const PosGUI origin, const INT width, const Color color) const;
	void DrawCell(const PosCell pos) const;
	void DrawTable() const;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void SetWindowDataInfo(HWND hwnd, LPARAM lParam, GUI*& game_interface);
	void CreateInterface(const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow);
	void RunMessageLoop();

public:
	GUI(const Bonol* game, const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow);
};

struct Bonol::GUI::PosGUI
{
	CoordGUI x, y;

	PosGUI() : x(0), y(0) {};
	PosGUI(CoordGUI x_pos, CoordGUI y_pos) : x(x_pos), y(y_pos) {};
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
