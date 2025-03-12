#pragma once
#include "Game.h"
#include "BallComponent.h"
#include "PlatformComponent.h"

class PingPongGame : public Game
{
protected:
	void Draw() override;
public:

	BallComponent* ball;
	PlatformComponent* platformLeft;
	PlatformComponent* platformRight;

	int leftPlayerScore = 0;
	int rightPlayerScore = 0;

	PingPongGame();
	~PingPongGame();

	void addScore(bool toLeft);

protected:
	void Update() override;
};