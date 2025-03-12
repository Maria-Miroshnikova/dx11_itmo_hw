#include "PingPongGame.h"
#include "BallComponent.h"
#include "PlatformComponent.h"
#include "SimpleMath.h"
#include "Keys.h"

#include <iostream>

using namespace DirectX::SimpleMath;

PingPongGame::PingPongGame() : Game(L"MyGame", 800, 800)
{
	//Components.push_back(new TriangleComponent(this));
	//Components.push_back(new CircleComponent(this));
	ball = new BallComponent(this);
	platformLeft = new PlatformComponent(this, true);
	platformRight = new PlatformComponent(this, false);

	platformLeft->SetPosition(Vector2(-0.8f, 0.0f));
	platformRight->SetPosition(Vector2(0.8f, 0.0f));

	// сдвинуть платформы
	Components.push_back(ball);
	Components.push_back(platformLeft);
	Components.push_back(platformRight);

	//Components.push_back(new RectangleComponent(this));
}

PingPongGame::~PingPongGame()
{
}

void PingPongGame::Draw()
{

	float color[] = { 0.1f, 0.1f, 0.1f, 1.0f };

	Context->ClearRenderTargetView(RenderView, color);

	for (auto c : Components)
	{
		c->Draw();
	}
}

void PingPongGame::addScore(bool toLeft) {
	if (toLeft)
		leftPlayerScore++;
	else
		rightPlayerScore++;

	std::cout << leftPlayerScore << " / " << rightPlayerScore << std::endl;
}

void PingPongGame::Update() {
	
	// изменение скорости шарика ?
	//ball->Speed += 0.1;

	// управление ракетками в з/в от нажатия на стрелок
	//std::cout << InputDev->IsKeyDown(Keys::W) << std::endl;
	
	// левая платформа
	if (InputDev->IsKeyDown(Keys::W)) {
		if (platformLeft->GetPosition().y < 1.0f)
			platformLeft->SetPosition(Vector2(platformLeft->GetPosition().x, platformLeft->GetPosition().y + DeltaTime * platformLeft->Speed));
		//std::cout << "Pushed keys: " << InputDev->keys->size() << std::endl;
		InputDev->RemovePressedKey(Keys::W);
	}
	if (InputDev->IsKeyDown(Keys::S)) {
		if (platformLeft->GetPosition().y > -1.0f)
			platformLeft->SetPosition(Vector2(platformLeft->GetPosition().x, platformLeft->GetPosition().y - DeltaTime * platformLeft->Speed));
		//std::cout << "Pushed keys: " << InputDev->keys->size() << std::endl ;
		InputDev->RemovePressedKey(Keys::S);
	}

	// правая платформа
	if (InputDev->IsKeyDown(Keys::Up)) {
		if (platformRight->GetPosition().y < 1.0f)
			platformRight->SetPosition(Vector2(platformRight->GetPosition().x, platformRight->GetPosition().y + DeltaTime * platformRight->Speed));
		//std::cout << "Pushed keys: " << InputDev->keys->size() << std::endl;
		InputDev->RemovePressedKey(Keys::Up);
	}
	if (InputDev->IsKeyDown(Keys::Down)) {
		if (platformRight->GetPosition().y > -1.0f)
			platformRight->SetPosition(Vector2(platformRight->GetPosition().x, platformRight->GetPosition().y - DeltaTime * platformRight->Speed));
		//std::cout << "Pushed keys: " << InputDev->keys->size() << std::endl;
		InputDev->RemovePressedKey(Keys::Down);
	}

	// обновление всех компонентов
	Game::Update();
}