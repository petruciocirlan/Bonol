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

#include "Bonol.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ INT nCmdShow)
{
    const auto kWindowDimensions = Bonol::Dimensions(800, 600);
    Bonol game(kWindowDimensions, hInstance, nCmdShow);

    return 0;
}
