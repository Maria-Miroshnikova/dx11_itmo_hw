#include "GridComponent.h"
#include "../Game.h"

using namespace DirectX;
using namespace SimpleMath;

GridComponent::GridComponent(Game* g, float cellSize, int lineCount)
	: BaseRenderComponent(g)
{
	constexpr Vector4 color = Vector4(0.15f, 0.15f, 0.15f, 1.0f);
	constexpr Vector4 boldColor = Vector4(0.3f, 0.3f, 0.3f, 1.0f);
	
	int k = 0;
	for (int i = 0 - lineCount / 2; i < lineCount / 2; ++i)
	{
		points_.push_back(Vertex( {Vector4(static_cast<float>(i), 0.0f, - cellSize * lineCount / 2, 1.0f), i % 5 == 0 ? boldColor : color} ) );
		indices_.push_back(k++);
		points_.push_back(Vertex( {Vector4(static_cast<float>(i), 0.0f, cellSize * lineCount / 2, 1.0f), i % 5 == 0 ? boldColor : color} ) );
		indices_.push_back(k++);
	}
	for (int i = 0 - lineCount / 2; i < lineCount / 2; ++i)
	{
		points_.push_back(Vertex( {Vector4(- cellSize * lineCount / 2, 0.0f, static_cast<float>(i), 1.0f), i % 5 == 0 ? boldColor : color} ) );
		indices_.push_back(k++);
		points_.push_back(Vertex( {Vector4(cellSize * lineCount / 2, 0.0f, static_cast<float>(i), 1.0f), i % 5 == 0 ? boldColor : color} ) );
		indices_.push_back(k++);
	}
	
	colorModePs_ = true;
	topologyType_ = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
}
