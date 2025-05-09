#pragma once
#include "../Davork/GameComponents/SphereComponent.h"

class KatamariBallOutline : public SphereComponent
{
protected:
    CD3D11_RASTERIZER_DESC CreateRasterizerStateDesc() override;
    float initialRadius;
public:
    KatamariBallOutline(Game* g, float radius, int sliceCount, int stackCount, const wchar_t* diffuseTextureName);
    ~KatamariBallOutline() override = default;
    void UpdateRadius(float radius);
};
