#pragma once
#include "CircleComponent.h"

class PingPongGame;

class BallComponent :
    public CircleComponent
{
private:
    PingPongGame* game;
    void setStartDirection();

public:
    float Speed;
    DirectX::SimpleMath::Vector2 Direction;

    BallComponent(Game* g);
    void Update() override;
};