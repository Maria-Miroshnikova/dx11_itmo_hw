#include "MyGame.h"
#include "TriangleComponent.h"
#include "CircleComponent.h"
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

MyGame::MyGame() : Game(L"MyGame", 800, 800)
{
	//Components.push_back(new TriangleComponent(this));
	//Components.push_back(new CircleComponent(this));
	int circle_count = 4;
	float x_0 = -1.0f;
	float density = 0.1;
	float y_0 = 1.0f - density * 4;

	for (int i = 0; i < circle_count; ++i) {
		Components.push_back(new CircleComponent(this, x_0, y_0 - i * density * 4, density - 0.02, (i + 1) * 4));
	}
}

MyGame::~MyGame()
{
}

void MyGame::Draw()
{
	// заменяем черное окно на мигающее красное
	float color[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	//float color[] = { TotalTime, 0.1f, 0.1f, 1.0f };
//	float color[] = { sin(2 * M_PI * TotalTime), 0.1f, 0.1f, 1.0f};
	//std::cout << color[0] <<std::endl;
	Context->ClearRenderTargetView(RenderView, color);

	for (auto c : Components)
	{
		c->Draw();
	}
}