#include "KatamariBall.h"
#include "../Davork/Game.h"
#include "../KatamariGame.h"
#include "KatamariFurnitureComponent.h"
#include "KatamariBallOutline.h"

using namespace DirectX;
using namespace SimpleMath;

void KatamariBall::UpdateSize(float absorbedSize)
{
    float tmp = sqrtf(gameSize * gameSize + absorbedSize * absorbedSize);
    collision.Radius = tmp;
    position.y = tmp;
    rotationMaxSpeed = 0.1f / (tmp * tmp);
    if (rotationMaxSpeed < 0.005f)
        rotationMaxSpeed = 0.005f;
    moveMaxSpeed = 8.0f * sqrtf(tmp);
    rotationDrag = 0.1f + 0.06f / sqrtf(tmp);
   // outline->UpdateRadius(tmp);
    SetScale(Vector3(tmp, tmp, tmp));
    gameSize = tmp;
}

KatamariBall::KatamariBall(Game* game) : SphereComponent(game, 1.0f, 32, 32, L"Textures/boll_gradient.dds"),
                                         outline(new KatamariBallOutline(game, 1.0f, 16, 16, L"Textures/boll_gradient.dds")), rotationDrag(0.14f), rotationMaxSpeed(0.1f), moveMaxSpeed(8.0f), moveDrag(5.0f), savedRot(Quaternion::Identity),
                                         velocity(Vector3::Zero), collision(position, 1.0f), gameSize(1.0f)
{
    kGame = dynamic_cast<KatamariGame*>(game);
}

KatamariBall::~KatamariBall()
{
    delete outline;
}

void KatamariBall::Initialize()
{
    outline->Initialize();
    SphereComponent::Initialize();
}

void KatamariBall::Draw()
{
    outline->Draw();
    SphereComponent::Draw();
}

void KatamariBall::Update()
{
    collision.Center = position;
    
    for (auto furn : kGame->furniture)
    {
        if (collision.Intersects(furn->collision) && !furn->isPickedUp && gameSize > furn->gameSize)
        {
            furn->isPickedUp = true;
            furn->kb = this;
            furn->initRelPos = furn->GetPosition() - GetPosition();
            rotation.Inverse(furn->invKbRot);
            UpdateSize(furn->gameSize);
        }
    }
    
    savedRot.RotateTowards(Quaternion::Identity, rotationDrag * game->DeltaTime());
    rotation *= savedRot;
    velocity *= 1.0f - moveDrag * game->DeltaTime();

    outline->SetRotation(rotation);

    outline->Update();
    SphereComponent::Update();

    position += velocity * game->DeltaTime();
    outline->SetPosition(position);


    const float gravity = -30.0f;           // скорость падения
    const float jumpStrength = 15.0f;       // высота

    float minY = collision.Radius;

    if (isJumping) {
        ySpeed += gravity * game->deltaTime_;         
        position.y += ySpeed * game->deltaTime_;      

        if (position.y <= minY) {                     // проверка на падение
            position.y =  minY;
            ySpeed = 0;
            isJumping = false;
        }

        outline->SetPosition(position);
    }
}

void KatamariBall::Reload()
{
    outline->Reload();
    SphereComponent::Reload();
}

void KatamariBall::DestroyResources()
{
    outline->DestroyResources();
    SphereComponent::DestroyResources();
}

void KatamariBall::SetDirection(Vector3 dir)
{
    Vector3 tmp = Vector3(dir.x, 0.0f, dir.z);
    tmp.Normalize();
    Quaternion q = Quaternion::CreateFromAxisAngle(tmp.Cross(Vector3::Up), -rotationMaxSpeed);
    float f = Quaternion::Angle(Quaternion::Identity, savedRot) / 0.1f;
    savedRot *= Quaternion::Lerp(q, Quaternion::Identity, f);
    velocity = tmp * moveMaxSpeed;
}

void KatamariBall::SetPosition(Vector3 p)
{
    outline->SetPosition(p);
    SphereComponent::SetPosition(p);
}
