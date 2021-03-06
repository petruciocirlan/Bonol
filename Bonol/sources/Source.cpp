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

#include "../headers/GUI.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ INT nCmdShow)
{
#ifdef _DEBUG
	AllocConsole();
#pragma warning(push)
#pragma warning(disable : 4996 6031)
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#pragma warning(pop)
#endif

	const GUI::Dimensions kWindowDimensions(800, 600);
	GUI game(kWindowDimensions, hInstance, nCmdShow);

    return 0;
}
