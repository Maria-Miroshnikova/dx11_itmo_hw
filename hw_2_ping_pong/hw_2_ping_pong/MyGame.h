#pragma once
#include "Game.h"

class MyGame : public Game
{
protected:
	void Draw() override;
public:
	MyGame();
	~MyGame();
};