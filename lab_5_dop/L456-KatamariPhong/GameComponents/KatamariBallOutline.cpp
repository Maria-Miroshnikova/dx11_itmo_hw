#include "KatamariBallOutline.h"

CD3D11_RASTERIZER_DESC KatamariBallOutline::CreateRasterizerStateDesc()
{
    CD3D11_RASTERIZER_DESC tmp = SphereComponent::CreateRasterizerStateDesc();
    tmp.FillMode = D3D11_FILL_WIREFRAME;
    return tmp;
}

KatamariBallOutline::KatamariBallOutline(Game* g, float radius, int sliceCount, int stackCount, const wchar_t* diffuseTextureName) :
    SphereComponent(g, radius, sliceCount, stackCount, diffuseTextureName), initialRadius(radius)
{
}

void KatamariBallOutline::UpdateRadius(float radius)
{
    SetScale(DirectX::SimpleMath::Vector3::One * (radius / initialRadius));
}
