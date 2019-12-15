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

#include <string>
//#include <stdafx.h>
#include <windows.h>
#include <windowsx.h>
//#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

class GUI
{
private:
	using CoordGUI = INT;
	struct PointGUI;
	class Bonol;

public:
	using Dimensions = PointGUI;

private:
	Rect table_, window_, skip_button_, current_player_text_box_;
	INT cell_size_;
	//PosGUI* mouse_;

	bool is_mouse_down_, is_selecting_, is_moving_block_;

	Graphics* graphics_;
	Bonol* game_state_;
	HWND hwnd_;

	inline void CalculateLayout();
	void OnMoveMouse(const PointGUI mouse_pos);
	void OnLeftClickPress(const PointGUI mouse_pos);
	void OnLeftClickRelease(const PointGUI mouse_pos);
	void OnPaint();
	void Resize();

	PointGUI GetTableCenter() const;
	PointGUI GetTableOrigin() const;
	BOOL IsInsideTable(const PointGUI pos) const;

	void DrawLine(const PointGUI from, const PointGUI to) const;
	void DrawRect(const Rect rc, const Color color, const FLOAT width) const;
	void FillRect(const Rect rc, const Color color) const;
	void DrawBackground() const;
	void DrawCurrentPlayerTextBox() const;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void SetWindowDataInfo(HWND hwnd, LPARAM lParam, GUI*& game_interface);
	void RunMessageLoop();
	void Initialize();

public:
	GUI(const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow);
};

struct GUI::PointGUI
{
	CoordGUI x, y;

	PointGUI() : x(0), y(0) {};
	PointGUI(CoordGUI x_pos, CoordGUI y_pos) : x(x_pos), y(y_pos) {};
};

#include "Bonol.h"