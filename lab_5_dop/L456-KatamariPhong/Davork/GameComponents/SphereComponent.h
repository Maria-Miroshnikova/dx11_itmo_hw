#pragma once

#include "BaseRenderComponent.h"

class Game;

class SphereComponent : public BaseRenderComponent
{
public:
    explicit SphereComponent(Game* game, float radius, int sliceCount, int stackCount, const wchar_t* diffuseTextureName);
    
    void SetPosition(DirectX::SimpleMath::Vector3 p) override { position = p; }
    void SetRotation(DirectX::SimpleMath::Quaternion q) override { rotation = q; }
    void SetScale(DirectX::SimpleMath::Vector3 s) override { scale = s; }
};
