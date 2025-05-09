#include "SphereComponent.h"
#include "../Game.h"

using namespace DirectX;
using namespace SimpleMath;

SphereComponent::SphereComponent(Game* g, float radius, int sliceCount, int stackCount, const wchar_t* diffuseTextureName)
	: BaseRenderComponent(g)
{
	textureFileName_ = diffuseTextureName;
	
	Vertex topPoint( {Vector4(0.0f, radius, 0.0f, 1.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f) } );
	Vertex bottomPoint( {Vector4(0.0f, -radius, 0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f) } );

	points_.push_back(topPoint);

	const float phiStep   = XM_PI / static_cast<float>(stackCount);
	const float thetaStep = XM_2PI / static_cast<float>(sliceCount);
	
	for(int i = 1; i <= stackCount-1; ++i)
	{
		const float phi = static_cast<float>(i) * phiStep;
		
        for(int j = 0; j <= sliceCount; ++j)
		{
			const float theta = static_cast<float>(j) * thetaStep;
			Vertex p;
			p.pos.x = radius * sinf(phi)*cosf(theta);
			p.pos.y = radius * cosf(phi);
			p.pos.z = radius * sinf(phi)*sinf(theta);
        	p.pos.w = 1.0f;
        	p.tex.x = theta / XM_2PI;
        	p.tex.y = phi / XM_PI;
        	p.normal = p.pos;
        	p.normal.w = 0.0f;
        	p.normal.Normalize();
			points_.push_back(p);
		}
	}
	
	points_.push_back(bottomPoint);

    for(int i = 1; i <= sliceCount; ++i)
	{
		indices_.push_back(0);
		indices_.push_back(i + 1);
		indices_.push_back(i);
	}
	
    int baseIndex = 1;
	const int ringVertexCount = sliceCount + 1;
	for(int i = 0; i < stackCount - 2; ++i)
	{
		for(int j = 0; j < sliceCount; ++j)
		{
			indices_.push_back(baseIndex + i*ringVertexCount + j);
			indices_.push_back(baseIndex + i*ringVertexCount + j + 1);
			indices_.push_back(baseIndex + (i+1)*ringVertexCount + j);

			indices_.push_back(baseIndex + (i+1)*ringVertexCount + j);
			indices_.push_back(baseIndex + i*ringVertexCount + j + 1);
			indices_.push_back(baseIndex + (i+1)*ringVertexCount + j + 1);
		}
	}
	
	const int southPoleIndex = points_.size() - 1;
	
	baseIndex = southPoleIndex - ringVertexCount;
	
	for(int i = 0; i < sliceCount; ++i)
	{
		indices_.push_back(southPoleIndex);
		indices_.push_back(baseIndex + i);
		indices_.push_back(baseIndex + i + 1);
	}
}
