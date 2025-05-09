#include "DebugAxisAnchorComponent.h"
#include "../Game.h"

using namespace DirectX;
using namespace SimpleMath;

DebugAxisAnchorComponent::DebugAxisAnchorComponent(Game* g)
    : BaseRenderComponent(g)
{
    points_.push_back( { Vector4(0.0f, 0.0f, 0.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) } );
    points_.push_back( { Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) } );

    points_.push_back( { Vector4(0.0f, 0.0f, 0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) } );
    points_.push_back( { Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) } );

    points_.push_back( { Vector4(0.0f, 0.0f, 0.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) } );
    points_.push_back( { Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) } );

    indices_ = { 0,1, 2,3, 4,5 };
    
    colorModePs_ = true;
    topologyType_ = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
}
