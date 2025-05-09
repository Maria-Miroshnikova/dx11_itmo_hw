#pragma once

#include "BaseRenderComponent.h"

class QuadComponent :
    public BaseRenderComponent
{
protected:
    CD3D11_RASTERIZER_DESC CreateRasterizerStateDesc() override;
public:
	explicit QuadComponent(Game* g, const wchar_t* diffuseTextureName);
    void SetPosition(DirectX::SimpleMath::Vector3 p) override { position = p; }
    void SetRotation(DirectX::SimpleMath::Quaternion q) override { rotation = q; }
    void SetScale(DirectX::SimpleMath::Vector3 s) override { scale = s; }
};

