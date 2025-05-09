#include "KatamariGame.h"

#include "Davork/GameComponents/DebugAxisAnchorComponent.h"
#include "Davork/GameComponents/GridComponent.h"
#include "GameComponents/KatamariFurnitureComponent.h"
#include "Davork/GameComponents/QuadComponent.h"
#include "GameComponents/KatamariBall.h"

using namespace DirectX;
using namespace SimpleMath;

float rand_pos() {
    //return static_cast<float>(rand()) / RAND_MAX * 100.0f - 80.0f;
    // % размер площади максимальной (плоскость имеет размер 100)
    // - половина
    return (rand() % 50 - 25);
}

KatamariGame::KatamariGame() : Game(L"Katamari Game", 800, 800)
{
    srand(static_cast<unsigned>(time(nullptr)));

    ball = new KatamariBall(this);
    ball->SetPosition(Vector3(0.0f, 1.0f, 0.0f));
    components_.push_back(ball);

    //QuadComponent* quad = new QuadComponent(this, L"Textures/wood.dds");
    QuadComponent* quad = new QuadComponent(this, L"Textures/gradient.dds");
    quad->SetRotation(Quaternion::CreateFromAxisAngle(Vector3::Right, XM_PI / 2.0f));
    quad->SetScale(Vector3::One * 100.0f);
    components_.push_back(quad);

    for (int i = 0; i < 10; ++i)
    {
        KatamariFurnitureComponent* juice = new KatamariFurnitureComponent(this, "Models/Juice_Bottle.obj", L"Textures/juice.dds", 1.5f, Vector3(0.0f, 0.8f, 0.0f));

        juice->SetPosition(Vector3(rand_pos(), 0.0f, rand_pos()));
        juice->SetScale(Vector3(0.18f, 0.18f, 0.18f));
        juice->collision.Radius = 0.8f;
        components_.push_back(juice);
        furniture.push_back(juice);
    }

    for (int i = 0; i < 10; ++i)
    {
        KatamariFurnitureComponent* shell_small = new KatamariFurnitureComponent(this, "Models/shell_1.obj", L"Textures/shell_2.dds", 0.4f, Vector3(0.0f, 0.5f, 0.0f));

        shell_small->SetScale(Vector3(0.01f, 0.01f, 0.01f));
        shell_small->SetPosition(Vector3(rand_pos(), 0.0f, rand_pos()));
        shell_small->collision.Radius = 0.1f;
        components_.push_back(shell_small);
        furniture.push_back(shell_small);
    }

    for (int i = 0; i < 10; ++i)
    {
        KatamariFurnitureComponent* skull = new KatamariFurnitureComponent(this, "Models/Skull.obj", L"Textures/Skull.dds", 1.2f, Vector3(0.0f, 1.0f, 0.0f));

        skull->SetScale(Vector3(0.002f, 0.002f, 0.002f));
        skull->SetPosition(Vector3(rand_pos(), 0.0f, rand_pos()));
        skull->collision.Radius = 0.2f;
        components_.push_back(skull);
        furniture.push_back(skull);
    }

    
    /*GridComponent* grid = new GridComponent(this, 1.0f, 100);
    grid->SetPosition(Vector3(0.0f, -0.001f, 0.0f));
    Components.push_back(grid);

    DebugAxisAnchorComponent* debugAnchor = new DebugAxisAnchorComponent(this);
    Components.push_back(debugAnchor);*/

    orbitCameraController = new OrbitCameraController(this, GetCamera(), ball);
    orbitCameraController->isLMBActivated = true;
    
    // Camera controller binding
    inputDevice_->MouseMove.AddRaw(orbitCameraController, &OrbitCameraController::OnMouseMove);



    // создание световых мешей и мб света
    // их еще надо апдейтить! для этого сунем их в компонентсы
    float katamari_ball_radius = ball->collision.Radius;

    // light 1
    Satellite satInfo;
    satInfo.OrbitOrientationForward = DirectX::SimpleMath::Vector3::Up;
    satInfo.OrbitOrientationUp = DirectX::SimpleMath::Vector3::Left;
    satInfo.orbitRadius = katamari_ball_radius + offset_form_ball + light_mesh_radius;
    satInfo.orbitSpeed = 2.0f;
    satInfo.relativePos = satInfo.OrbitOrientationForward * satInfo.orbitRadius;
    satellitesInfo.push_back(satInfo);

    SphereComponent* lightMesh = new SphereComponent(this, light_mesh_radius, 8, 8, L"Textures/boll_gradient.dds");
    auto pos = ball->position + satInfo.relativePos;
    lightMesh->SetPosition(pos);
    lightMesh->is_light = true;
    lightMesh->light_color = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
    lightsMeshesVector.push_back(lightMesh);
    components_.push_back(lightMesh);

    
    PointLight* pLight1 = new PointLight(this);
    pLight1->SetColor(DirectX::SimpleMath::Vector4(lightMesh->light_color.x, lightMesh->light_color.y, lightMesh->light_color.z, 1.0f));
    pLight1->SetPosition(lightMesh->position);
   // pLight1->SetPosition(DirectX::SimpleMath::Vector3(0.0f, 10.0f, 0.0f));
    pointLightsVector.push_back(pLight1);
    LightInfo lightInfo;
    lightsInfoVector.push_back(lightInfo);

    // light 2

    Satellite satInfo2;
    satInfo2.OrbitOrientationForward = DirectX::SimpleMath::Vector3::Right;
    satInfo2.OrbitOrientationUp = DirectX::SimpleMath::Vector3::Up;
    satInfo2.orbitRadius = katamari_ball_radius + offset_form_ball + light_mesh_radius;
    satInfo2.orbitSpeed = 2.0f;
    satInfo2.relativePos = satInfo2.OrbitOrientationForward * satInfo2.orbitRadius;
    satellitesInfo.push_back(satInfo2);

    SphereComponent* lightMesh2 = new SphereComponent(this, light_mesh_radius, 8, 8, L"Textures/boll_gradient.dds");
    pos = ball->position + satInfo2.relativePos;
    lightMesh2->SetPosition(pos);
    lightMesh2->is_light = true;
    lightMesh2->light_color = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 1.0f);
    lightsMeshesVector.push_back(lightMesh2);
    components_.push_back(lightMesh2);

    PointLight* pLight2 = new PointLight(this);
    pLight2->SetColor(DirectX::SimpleMath::Vector4(lightMesh2->light_color.x, lightMesh2->light_color.y, lightMesh2->light_color.z, 1.0f));
    pLight2->SetPosition(lightMesh2->position);
    pointLightsVector.push_back(pLight2);
    lightsInfoVector.push_back(lightInfo);
}

void KatamariGame::Update()
{
    // вращаем свет
    for (int i = 0; i < lightsMeshesVector.size(); ++i) {
        auto lightMesh = lightsMeshesVector[i];
        satellitesInfo[i].orbitRadius = ball->collision.Radius + offset_form_ball + light_mesh_radius;
        auto satInfo = satellitesInfo[i];

        DirectX::SimpleMath::Vector3 relPos = XMVector4Transform(satInfo.relativePos, Matrix::CreateFromAxisAngle(satInfo.OrbitOrientationUp, deltaTime_ * satInfo.orbitSpeed));
        relPos.Normalize();
        satellitesInfo[i].relativePos = relPos * satInfo.orbitRadius;
        auto mesh_position = ball->position + satellitesInfo[i].relativePos;
        lightMesh->SetPosition(mesh_position);
        
        
        /*


        // Step 1: Rotate satellite around local orbit axis (like you already do)
        Matrix orbitRotation = Matrix::CreateFromAxisAngle(satellitesInfo[i].OrbitOrientationUp, deltaTime_ * satellitesInfo[i].orbitSpeed);
        Vector3 relPos = XMVector3Transform(satellitesInfo[i].relativePos, orbitRotation);

        // Step 2: Apply the ball's current rotation to the orbit
        //relPos = XMVector3Transform(relPos, ball);  // <- this makes orbit rotate with the ball!
        relPos = XMVector3Rotate(relPos, ball->savedRot);

        // Step 3: Normalize and reapply radius
        relPos.Normalize();
        satellitesInfo[i].relativePos = relPos * (ball->collision.Radius + offset_form_ball + light_mesh_radius);

        // Step 4: Compute final position
        Vector3 mesh_position = ball->position + satellitesInfo[i].relativePos;
        lightsMeshesVector[i]->SetPosition(mesh_position);
        */
        // ? делать ли поворот самих источников света вокруг оси?
        //rotation *= Quaternion::CreateFromAxisAngle(spinAxis, spinSpeed * game->DeltaTime);

        // переписываем положение с лайтмеша в свет
       // auto pointLight = pointLightsVector[i];
       // pointLight->SetPosition(lightMesh->GetPosition());
    }

    orbitCameraController->Update();
    Vector3 dir = Vector3::Zero;
    if (inputDevice_->IsKeyDown(Keys::W))
        dir += orbitCameraController->GetForward();
    if (inputDevice_->IsKeyDown(Keys::S))
        dir -= orbitCameraController->GetForward();
    if (inputDevice_->IsKeyDown(Keys::A))
        dir -= (orbitCameraController->GetForward()).Cross(orbitCameraController->GetUp());
    if (inputDevice_->IsKeyDown(Keys::D))
        dir +=(orbitCameraController->GetForward()).Cross(orbitCameraController->GetUp());
    if (dir.Length() > 0.0f)
        ball->SetDirection(dir);
    if (inputDevice_->IsKeyDown(Keys::Space)) {
        ball->isJumping = true;
        const float jumpStrength = 15.0f;
        ball->ySpeed = jumpStrength;
    }
    Game::Update();
}
