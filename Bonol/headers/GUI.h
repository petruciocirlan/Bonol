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

/// utility
#include <string>
#include <stack>
#include <cassert>
#include <iostream>

/// windows
//#include <stdafx.h>
#include <windows.h>
#include <windowsx.h>

/// graphics
//#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;

/// audio
#include "../resources/resource.h"
#include <mmsystem.h>
#pragma comment (lib,"Winmm.lib")

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
	using String = std::basic_string<TCHAR>;

private:
	const Color kBackgroundColor = Color::White;
	const Color kTextColor = Color::Black;
	const Color kTextHover = Color::DeepPink;

	enum class Language
	{
		ENGLISH//, ROMANIAN
	};
	Language current_language_;

	enum class Screen
	{
		MENU, GAME//, SETTINGS
	};
	Screen current_screen_;

	enum class VersusMode
	{
		PLAYER, COMPUTER_EASY, COMPUTER_HARD
	};
	VersusMode current_mode_;

	/// ANY SCREEN (GLOBAL)
	HWND hwnd_;
	Rect window_;
	Graphics* graphics_;

	bool is_mouse_down_;
	bool force_repaint_background_;
	bool is_playing_music_;

	static const INT kTextBoxesGlobalCount = 1;
	union
	{
		struct { TextBox* text_boxes_global_[kTextBoxesGlobalCount]; };
		struct { TextBox* music_toggle_; };
	};

	/// MENU SCREEN
	static const INT kTextBoxesMenuCount = 5;
	union
	{
		struct { TextBox* text_boxes_menu_[kTextBoxesMenuCount]; };
		struct {
			TextBox* title_,
				* play_player_button_, * play_computer_button_,
				* easy_computer_button_, * hard_computer_button_;
		};
	};

	/// GAME SCREEN
	Bonol* game_state_;
	std::stack < Bonol >* game_history_;

	Rect table_;
	INT cell_size_, selected_cells_count_;

	bool is_selecting_, is_moving_block_;

	static const INT kTextBoxesGameCount = 5;
	union
	{
		struct { TextBox* text_boxes_game_[kTextBoxesGameCount]; };
		struct { TextBox* current_player_, * skip_button_, * reset_button_, * undo_button_, * menu_button_; };
	};

	//String* language_text_menu_[kTextBoxesMenuCount];


	/// events
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void OnMouseMove(const PointGUI mouse_pos);
	void OnMouseMoveGlobal(const PointGUI mouse_pos);
	void OnMouseMoveMenu(const PointGUI mouse_pos);
	void OnMouseMoveGame(const PointGUI mouse_pos);

	void OnLeftClickPress(const PointGUI mouse_pos);
	bool OnLeftClickPressGlobal(const PointGUI mouse_pos);
	void OnLeftClickPressMenu(const PointGUI mouse_pos);
	void OnLeftClickPressGame(const PointGUI mouse_pos);

	void OnLeftClickRelease(const PointGUI mouse_pos);
	void OnLeftClickReleaseGlobal(const PointGUI mouse_pos);
	void OnLeftClickReleaseMenu(const PointGUI mouse_pos);
	void OnLeftClickReleaseGame(const PointGUI mouse_pos);

	void OnPaint();
	void Resize();

	/// drawing
	//void DrawLine(const PointGUI from, const PointGUI to) const;
	void DrawRect(const Rect rc, const Color color, const FLOAT width) const;
	void FillRect(const Rect rc, const Color color) const;
	void DrawCircle(const Rect rc, const Color color, const FLOAT width) const;
	void FillCircle(const Rect rc, const Color color) const;

	void DrawCell(const PointGUI pos) const;

	void DrawTextBox(TextBox& text_box);
	void DrawTextBoxes();
	void DrawTextBoxesGlobal();
	void DrawTextBoxesMenu();
	void DrawTextBoxesGame();

	void DrawBackground() const;
	void DrawForeground();

	/// layout compute
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

	/// Screen create/delete and screen-specific methods

	void CreateGame();
	void DestroyGame();
	void EndMovingBlockTurn();

	void CreateMenu();
	void DestroyMenu();

	/// Window setup and message loop

	static void SetWindowDataInfo(HWND hwnd, LPARAM lParam, GUI*& game_interface);
	void RunMessageLoop();
	void Initialize();
	void Destroy();

public:
	GUI(const Dimensions window_dimensions, HINSTANCE hInstance, INT nCmdShow);
};

struct GUI::PointGUI
{
	CoordGUI x, y;

	PointGUI() : x(0), y(0) {};
	PointGUI(CoordGUI X, CoordGUI Y) : x(X), y(Y) {};
};

struct GUI::Padding
{
	INT top, right, bottom, left;

	Padding()
		: top(0), right(0), bottom(0), left(0) {};
	Padding(INT PADDING_ALL)
		: top(PADDING_ALL), right(PADDING_ALL), bottom(PADDING_ALL), left(PADDING_ALL) {};
	Padding(INT PADDING_VERTICAL, INT PADDING_HORIZONTAL)
		: top(PADDING_VERTICAL), right(PADDING_HORIZONTAL), bottom(PADDING_VERTICAL), left(PADDING_HORIZONTAL) {};
	Padding(INT PADDING_TOP, INT PADDING_RIGHT, INT PADDING_BOTTOM, INT PADDING_LEFT)
		: top(PADDING_TOP), right(PADDING_RIGHT), bottom(PADDING_BOTTOM), left(PADDING_LEFT) {};
};

struct GUI::TextBox
{
	String text;
	Font * normal_font, * highlighted_font;
	Brush * normal_color, * highlighted_color;
	PointGUI center;

	Rect rect;
	Padding padding;
	bool updated, visible, is_hover, was_hover;

	//TextBox() : updated(true), visible(true) {};
	TextBox(const String TEXT,
		    Brush* NORMAL_COLOR, Font* NORMAL_FONT, Padding PADDING = Padding(),
		    Brush* HIGHLIGHTED_COLOR = NULL, Font* HIGHLIGHTED_FONT = NULL);
	~TextBox();
	/// TODO(@petru): suggestion - attach click events to struct
};

#include "Bonol.h"