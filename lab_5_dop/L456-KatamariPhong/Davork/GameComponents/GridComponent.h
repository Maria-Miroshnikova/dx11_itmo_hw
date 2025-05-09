#pragma once

#include "BaseRenderComponent.h"

class Game;

class GridComponent : public BaseRenderComponent
{
public:
    GridComponent(Game* game, float cellSize, int lineCount);
    ~GridComponent() override = default;
    void SetPosition(DirectX::SimpleMath::Vector3 p) override { position = p; }
    void SetRotation(DirectX::SimpleMath::Quaternion q) override { rotation = q; }
    void SetScale(DirectX::SimpleMath::Vector3 s) override { scale = s; }
};
