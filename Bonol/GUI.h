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

#ifndef UNICODE
#define UNICODE
#endif

//#include <stdafx.h>
#include <windows.h>
#include <windowsx.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

class GUI
{
private:
	using CoordGUI = INT;
	struct PosGUI;
	class Bonol;

public:
	using Dimensions = PosGUI;

private:
	CoordGUI table_width_, cell_width_;
	CoordGUI width_, height_;
	//PosGUI* mouse_;

	bool is_mouse_down_, is_selecting_;

	Graphics* graphics_;
	Bonol* kGameState;
	HWND hwnd_;

	inline void CalculateLayout();
	void OnMoveMouse(const PosGUI mouse_pos);
	void OnLeftClickPress(const PosGUI mouse_pos);
	void OnLeftClickRelease(const PosGUI mouse_pos);
	void OnPaint();
	void Resize();

	PosGUI GetTableCenter() const;
	PosGUI GetTableOrigin() const;
	BOOL IsInsideTable(const PosGUI pos) const;

	void DrawLine(const PosGUI from, const PosGUI to) const;
	void DrawRect(const Rect rc, const Color color) const;
	void DrawBackground() const;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void SetWindowDataInfo(HWND hwnd, LPARAM lParam, GUI*& game_interface);
	void CreateInterface(const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow);
	void RunMessageLoop();
	void Initialize();

public:
	GUI(const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow);
};

struct GUI::PosGUI
{
	CoordGUI x, y;

	PosGUI() : x(0), y(0) {};
	PosGUI(CoordGUI x_pos, CoordGUI y_pos) : x(x_pos), y(y_pos) {};
};

#include "Bonol.h"