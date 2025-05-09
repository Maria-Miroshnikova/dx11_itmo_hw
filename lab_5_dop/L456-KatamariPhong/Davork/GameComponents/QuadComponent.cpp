#include "QuadComponent.h"
#include "../Game.h"

using namespace DirectX;
using namespace SimpleMath;

CD3D11_RASTERIZER_DESC QuadComponent::CreateRasterizerStateDesc()
{
	CD3D11_RASTERIZER_DESC desc = BaseRenderComponent::CreateRasterizerStateDesc();
	desc.CullMode = D3D11_CULL_NONE;
	return desc;
}

QuadComponent::QuadComponent(Game* g, const wchar_t* diffuseTextureName) : BaseRenderComponent(g)
{
	textureFileName_ = diffuseTextureName;
	
	//points_.push_back( { Vector4(0.5f, 0.5f, 0.0f, 1.0f),	Vector4(8.0f, 8.0f, 0.0f, 0.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f) } );
	//points_.push_back( { Vector4(-0.5f, -0.5f, 0.0f, 1.0f),Vector4(0.0f, 0.0f, 0.0f, 0.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f) } );
	//points_.push_back( { Vector4(0.5f, -0.5f, 0.0f, 1.0f), Vector4(0.0f, 8.0f, 0.0f, 0.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f) } );
	//points_.push_back( { Vector4(-0.5f, 0.5f, 0.0f, 1.0f), Vector4(8.0f, 0.0f, 0.0f, 0.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f) } );

	points_.push_back({ Vector4(0.5f, 0.5f, 0.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 0.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f) });
	points_.push_back({ Vector4(-0.5f, -0.5f, 0.0f, 1.0f),Vector4(0.0f, 1.0f, 0.0f, 0.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f) });
	points_.push_back({ Vector4(0.5f, -0.5f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 0.0f, 0.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f) });
	points_.push_back({ Vector4(-0.5f, 0.5f, 0.0f, 1.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f) });


	indices_ = { 0,1,2, 1,0,3 };
	//indices_ = { 0,1,2, 0,2,3 };
}
