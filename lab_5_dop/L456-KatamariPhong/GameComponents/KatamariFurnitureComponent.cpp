#include "KatamariFurnitureComponent.h"

#include "KatamariBall.h"

using namespace DirectX;
using namespace SimpleMath;

KatamariFurnitureComponent::KatamariFurnitureComponent(Game* g, const std::string fileNameModel, const wchar_t* fileNameTexture, float katSize, Vector3 collOffset) :
    MeshRenderComponent(g, fileNameModel, fileNameTexture), isPickedUp(false), collision(position + collOffset, katSize), originCollisionOffset(collOffset), gameSize(katSize)
{
}

void KatamariFurnitureComponent::Update()
{
    MeshRenderComponent::Update();
    if (isPickedUp)
    {
        if (kb)
        {
            SetPosition(kb->GetPosition() + Vector3::Transform(initRelPos, invKbRot * kb->GetRotation()));
            SetRotation(invKbRot * kb->GetRotation());
        }
    }
}

void KatamariFurnitureComponent::SetPosition(Vector3 p)
{
    collision.Center = p + originCollisionOffset;
    MeshRenderComponent::SetPosition(p);
}
