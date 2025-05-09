#include "FPSCameraController.h"
#include "Game.h"

using namespace DirectX;
using namespace SimpleMath;

FPSCameraController::FPSCameraController(Game* g, Camera* c) : game(g), camera(c), yaw(0.0f), pitch(0.0f), speed(0.2f),
    sensitivityX(0.004f), sensitivityY(0.004f), isLMBActivated(true)
{
}

void FPSCameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
    if (game->GetInputDevice()->IsKeyDown(Keys::LeftButton) || !isLMBActivated)
    {
        yaw -= sensitivityX * game->GetInputDevice()->MouseOffset.x;
        while (yaw < -XM_2PI)
            yaw += XM_2PI;
        while (yaw > XM_2PI)
            yaw -= XM_2PI;
        pitch -= sensitivityY * game->GetInputDevice()->MouseOffset.y;
        while (pitch < -XM_2PI)
            pitch += XM_2PI;
        while (pitch > XM_2PI)
            pitch -= XM_2PI;
        camera->Up = XMVector4Transform(Vector3::Up, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
        camera->Target = game->GetCamera()->Position;
        camera->Target += XMVector4Transform(Vector3::Forward, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
    }
}

void FPSCameraController::Update()
{
    if (game->GetInputDevice()->IsKeyDown(Keys::W))
    {
        Vector3 tmp = XMVector4Transform(Vector3::Forward, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
        tmp.Normalize();
        camera->Position += speed * tmp;
    }
    if (game->GetInputDevice()->IsKeyDown(Keys::S))
    {
        Vector3 tmp = XMVector4Transform(Vector3::Backward, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
        tmp.Normalize();
        camera->Position += speed * tmp;
    }
    if (game->GetInputDevice()->IsKeyDown(Keys::A))
    {
        Vector3 tmp = XMVector4Transform(Vector3::Left, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
        tmp.Normalize();
        camera->Position += speed * tmp;
    }
    if (game->GetInputDevice()->IsKeyDown(Keys::D))
    {
        Vector3 tmp = XMVector4Transform(Vector3::Right, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
        tmp.Normalize();
        camera->Position += speed * tmp;
    }
    if (game->GetInputDevice()->IsKeyDown(Keys::E))
    {
        camera->Position += speed * Vector3::Up;
    }
    if (game->GetInputDevice()->IsKeyDown(Keys::Z))
    {
        camera->Position += speed * Vector3::Down;
    }
    camera->Up = XMVector4Transform(Vector3::Up, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
    camera->Target = game->GetCamera()->Position;
    camera->Target += XMVector4Transform(Vector3::Forward, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
}

Vector3 FPSCameraController::GetForward() const
{
    return Vector3::Transform(Vector3::Forward, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
}

Vector3 FPSCameraController::GetUp() const
{
    return Vector3::Transform(Vector3::Up, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
}
