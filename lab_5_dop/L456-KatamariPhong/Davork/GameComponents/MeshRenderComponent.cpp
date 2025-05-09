#include "MeshRenderComponent.h"
#include <iostream>
#include "../ResourceFactory.h"

using namespace DirectX;
using namespace SimpleMath;

MeshRenderComponent::MeshRenderComponent(Game* g, const std::string fileNameModel, const wchar_t* fileNameTexture) :
    BaseRenderComponent(g), fNameModel(fileNameModel), isPickedUp(false)
{
    textureFileName_ = fileNameTexture;
}

void MeshRenderComponent::Initialize()
{
    points_ = ResourceFactory::GetPoints(fNameModel);
    indices_ = ResourceFactory::GetIndices(fNameModel);
    
    BaseRenderComponent::Initialize();
}
