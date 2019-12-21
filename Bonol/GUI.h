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
	struct TextBox;
	class Bonol;

public:
	using Dimensions = PointGUI;

private:
	const Color kBackgroundColor = Color::Purple;

	Rect table_, window_;
	TextBox *current_player_, *skip_button_;
	INT cell_size_;
	//PosGUI* mouse_;

	bool is_mouse_down_, is_selecting_, is_moving_block_;
	bool show_skip_;

	Graphics* graphics_;
	Bonol* game_state_;
	HWND hwnd_;

	void CalculateLayout();
	void InvalidateTextBoxes();
	void OnMoveMouse(const PointGUI mouse_pos);
	void OnLeftClickPress(const PointGUI mouse_pos);
	void OnLeftClickRelease(const PointGUI mouse_pos);
	void OnPaint();
	void Resize();

	Rect MakeRect(PointGUI origin, PointGUI dimensions) const;
	PointGUI GetTableCenter() const;
	PointGUI GetTableOrigin() const;
	BOOL IsInsideTable(const PointGUI pos) const;

	void DrawLine(const PointGUI from, const PointGUI to) const;
	void DrawRect(const Rect rc, const Color color, const FLOAT width) const;
	void FillRect(const Rect rc, const Color color) const;

	void DrawCurrentPlayer() const;
	void DrawSkipButton() const;

	void DrawTextBoxes();
	void DrawBackground() const;
	void DrawForeground();

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void SetWindowDataInfo(HWND hwnd, LPARAM lParam, GUI *&game_interface);
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

struct GUI::TextBox
{
	union
	{
		struct { INT x, y, width, height; };
		struct { Rect rect; };
	};
	bool updated;

	TextBox() : rect(Rect()), updated(true) {};
	TextBox(PointGUI top_left, PointGUI dimensions)
		: rect(Rect(top_left.x, top_left.y, dimensions.x, dimensions.y)), updated(true) {};
};

#include "Bonol.h"