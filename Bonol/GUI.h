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

class Bonol::GUI
{
private:
	int				width_, height_;
	int				left_, top_;
	int				table_width_, cell_width_;

	const Bonol&	game_state_;
	Position		center_, origin_;

	void DrawLine				(Position from, Position to)			const;
	void DrawSquare				(Position origin, int width)			const;
	void GetPolyPointsFromCell	(Position pos, Position polypoints[4])	const;
	void DrawCell				(const Position pos)					const;

public:
	GUI(const Bonol* game, const Dimensions window_dimensions);

	void DrawScreen()	const;
};