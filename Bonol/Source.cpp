/*
	File: Source.cpp
	Written by:
		Petru Ciocirlan		(petru.ciocirlan@info.uaic.ro)
		Valentin Grigorean	(valentin.grigorean@info.uaic.ro)
	CS1105: Practica - Introducere in programare
	Faculty of Computer Science, UAIC, WINTER 2019

	ABOUT THIS FILE:
	Main source file.
*/

#include <iostream>
#include "Bonol.h"
//using namespace std;

int main()
{
	auto dimensions = Bonol::Dimensions(800, 600);
	Bonol game(dimensions);
	
	getch();

	//while (!game.Over())
	//{
	//	// do move
	//	game.UpdateScreen();
	//}

	/// congratulate winner

	return 0;
}