#pragma once

#include "../Davork/GameComponents/SphereComponent.h"

class KatamariBallOutline;
class KatamariGame;

class KatamariBall :  public SphereComponent
{
protected:
    KatamariGame* kGame;
    void UpdateSize(float absorbedSize);
public:
    KatamariBallOutline* outline;
    float rotationDrag;
    float rotationMaxSpeed;
    float moveMaxSpeed;
    float moveDrag;
    DirectX::SimpleMath::Quaternion savedRot;
    DirectX::SimpleMath::Vector3 velocity;
    DirectX::BoundingSphere collision;
    float gameSize;
    KatamariBall(Game* game);
    ~KatamariBall() override;
    void Initialize() override;
    void Draw() override;
    void Update() override;
    void Reload() override;
    void DestroyResources() override;
    void SetDirection(DirectX::SimpleMath::Vector3 dir);
    void SetPosition(DirectX::SimpleMath::Vector3 p) override;

    bool isJumping = false;
    float ySpeed = 0;
};
