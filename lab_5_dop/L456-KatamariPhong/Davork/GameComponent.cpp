#include "GameComponent.h"
#include "Game.h"

using namespace DirectX;
using namespace SimpleMath;

GameComponent::GameComponent(Game* g) : game(g), position(Vector3::Zero), rotation(Quaternion::Identity), scale(Vector3::One)
{
}
