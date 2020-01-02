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
#include <stack>
#include <cassert>
#include <iostream>
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
	struct Padding;
	struct TextBox;
	class Bonol;

public:
	using Dimensions = PointGUI;

private:
	const Color kBackgroundColor = Color::White;
	const Color kTextColor = Color::Black;

	enum class Screen
	{
		MENU, GAME//, SETTINGS
	};
	Screen current_screen_;

	Rect table_, window_;
	INT cell_size_;
	//PosGUI* mouse_;

	static const INT kTextBoxesGameCount = 5;
	union
	{
		struct { TextBox *text_boxes_game_[kTextBoxesGameCount]; };
		struct { TextBox *current_player_, *skip_button_, *reset_button_, *undo_button_, * menu_button_; };
	};

	static const INT kTextBoxesMenuCount = 2;
	union
	{
		struct { TextBox *text_boxes_menu_[kTextBoxesMenuCount]; };
		struct { TextBox *title_, *play_button_; };
	};

	bool is_mouse_down_, is_selecting_, is_moving_block_;
	bool repaint_background_;

	std::stack < Bonol > *game_history_;
	Graphics *graphics_;
	Bonol *game_state_;
	HWND hwnd_;

	/// events
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void OnMouseMove(const PointGUI mouse_pos);
	void OnMouseMoveMenu(const PointGUI mouse_pos);
	void OnMouseMoveGame(const PointGUI mouse_pos);

	void OnLeftClickPress(const PointGUI mouse_pos);
	void OnLeftClickPressMenu(const PointGUI mouse_pos);
	void OnLeftClickPressGame(const PointGUI mouse_pos);

	void OnLeftClickRelease(const PointGUI mouse_pos);
	void OnLeftClickReleaseMenu(const PointGUI mouse_pos);
	void OnLeftClickReleaseGame(const PointGUI mouse_pos);

	void OnPaint();
	void Resize();

	/// drawing
	void DrawLine(const PointGUI from, const PointGUI to) const;
	void DrawRect(const Rect rc, const Color color, const FLOAT width) const;
	void FillRect(const Rect rc, const Color color) const;

	void DrawCell(const PointGUI pos) const;
	void DrawTextBox(TextBox &text_box);
	void DrawTextBoxesMenu();
	void DrawTextBoxesGame();
	void DrawBackground() const;
	void DrawForeground();

	/// logic
	Rect MakeRect(PointGUI origin, PointGUI dimensions) const;
	Rect InflateRect(const Rect rect, Padding padding) const;
	PointGUI GetTableCenter() const;
	PointGUI GetTableOrigin() const;
	BOOL IsInsideTable(const PointGUI pos) const;

	void CalculateTextBoxPosition(TextBox& box);
	void CalculateCurrentPlayerText();
	void CalculateLayout();
	void CalculateLayoutMenu();
	void CalculateLayoutGame();
	void InvalidateTextBoxes();

	void CreateGame();
	void DestroyGame();
	void EndMovingBlockTurn();

	void CreateMenu();
	void DestroyMenu();

	static void SetWindowDataInfo(HWND hwnd, LPARAM lParam, GUI *&game_interface);
	void RunMessageLoop();

public:
	GUI(const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow);
};

struct GUI::PointGUI
{
	CoordGUI x, y;

	PointGUI() : x(0), y(0) {};
	PointGUI(CoordGUI x_pos, CoordGUI y_pos) : x(x_pos), y(y_pos) {};
};

struct GUI::Padding
{
	INT top, right, bottom, left;

	Padding() :
		top(0), right(0), bottom(0), left(0) {};
	Padding(INT padding) :
		top(padding), right(padding), bottom(padding), left(padding) {};
	Padding(INT vertical, INT horizontal) :
		top(vertical), right(horizontal), bottom(vertical), left(horizontal) {};
	Padding(INT top_pad, INT right_pad, INT bottom_pad, INT left_pad) :
		top(top_pad), right(right_pad), bottom(bottom_pad), left(left_pad) {};
};

struct GUI::TextBox
{
	std::basic_string<TCHAR> text;
	Font *font;
	Brush *color;
	PointGUI center;

	Rect rect;
	Padding padding;
	bool updated, visible;

	//TextBox() : updated(true), visible(true) {};
	TextBox(const std::basic_string<TCHAR> TEXT, Font *FONT, Brush *COLOR, Padding PADDING = Padding())
		: text(TEXT), font(FONT), color(COLOR), updated(true), visible(true), padding(PADDING) {};
	~TextBox();
	/// TODO(@petru): suggestion - attach click events to struct
};

#include "Bonol.h"