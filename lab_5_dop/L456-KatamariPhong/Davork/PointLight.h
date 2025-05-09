#pragma once
#include "SimpleMath.h"
#include <vector>
#include "../Davork/PerSceneCb.h"

class Game;

class PointLight
{
    DirectX::SimpleMath::Vector3 position_;
    DirectX::SimpleMath::Vector4 lightColor_;
    Game* game_;

public:
    PointLight(Game* g);
    
    DirectX::SimpleMath::Vector3 GetPosition() const { return position_; }
    void SetPosition(const DirectX::SimpleMath::Vector3 pos) { position_ = pos; }
    
    DirectX::SimpleMath::Vector4 GetColor() const { return lightColor_; }
    void SetColor(const DirectX::SimpleMath::Vector4 col) { lightColor_ = col; }
};