#pragma once

#include "BaseRenderComponent.h"

class Game;

class DebugAxisAnchorComponent : public BaseRenderComponent
{
public:
    DebugAxisAnchorComponent(Game* game);
    ~DebugAxisAnchorComponent() override = default;
    void SetPosition(DirectX::SimpleMath::Vector3 p) override { position = p; }
    void SetScale(DirectX::SimpleMath::Vector3 s) override { scale = s; }
};
