#pragma once
#include "SimpleMath.h"

struct PerSceneCb
{
    DirectX::SimpleMath::Vector4 LightPos;
    DirectX::SimpleMath::Vector4 LightColor;
    DirectX::SimpleMath::Vector4 AmbientSpecularPowType; // a - ambient light strength, s - specularity, p - falloff power
        // todo: t - 0: directional, 1: point, 2: spot
    DirectX::SimpleMath::Matrix T;
};
