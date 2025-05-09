#pragma once
#include <windows.h>
#include <WinUser.h>

class Game;

class DisplayWin32
{
private:
	Game* game;
public:
	int ClientHeight;
	int ClientWidth;
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASSEX wc;
	DisplayWin32(LPCWSTR applicationName, HINSTANCE hInst, int screenWidth, int screenHeight, Game* g);
};
