#pragma once
#include <set>

class Game;

class InputDevice {

private:
	Game* game;
	std::set<int> keys;

public:
	int MousePosX = 0;
	int MousePosY = 0;
	bool MouseMove;

	InputDevice(Game* game);

	void AddPressedKey(int key);
	void RemovePressed(int key);

	bool IsKeyDown(int key);

protected:
	void OnKeyDown();
	void OnMouseMove();
};