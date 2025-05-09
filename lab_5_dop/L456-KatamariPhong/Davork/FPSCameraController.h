#pragma once
#include "InputDevice.h"
#include "SimpleMath.h"

class Game;
class Camera;

class FPSCameraController
{
protected:
    Game* game;
    Camera* camera;
    float yaw;
    float pitch;
    float speed;
    float sensitivityX;
    float sensitivityY;
    bool isLMBActivated;
public:
    FPSCameraController(Game* g, Camera* c);
    void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);
    void Update();
    DirectX::SimpleMath::Vector3 GetForward() const;
    DirectX::SimpleMath::Vector3 GetUp() const;
    float GetYaw() const { return yaw; }
    float GetPitch() const { return pitch; }
    float GetSpeed() const { return speed; }
    void SetSpeed(float s) { speed = s; }
    void SetSensitivity(float sX, float sY) { sensitivityX = sX; sensitivityY = sY; }
    void SetSensitivityX(float sX) { sensitivityX = sX; }
    void SetSensitivityY(float sY) { sensitivityY = sY; }
    float GetSensitivityX() const { return sensitivityX; }
    float GetSensitivityY() const { return sensitivityX; }
};
