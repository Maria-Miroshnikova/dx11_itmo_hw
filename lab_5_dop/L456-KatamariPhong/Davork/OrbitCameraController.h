#pragma once

#include "GameComponent.h"
#include "InputDevice.h"
#include "SimpleMath.h"

class GameComponent;
class Game;
class Camera;

class OrbitCameraController
{
protected:
    Game* game;
    Camera* camera;
    GameComponent* target;
    DirectX::SimpleMath::Quaternion rotation;
    float radius;
    float sensitivityX;
    float sensitivityY;
public:
    bool isLMBActivated;
    OrbitCameraController(Game* g, Camera* c, GameComponent* t);
    void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);
    void Update();
    DirectX::SimpleMath::Vector3 GetForward() const;
    DirectX::SimpleMath::Vector3 GetUp() const;
    void SetSensitivity(float sX, float sY) { sensitivityX = sX; sensitivityY = sY; }
    void SetSensitivityX(float sX) { sensitivityX = sX; }
    void SetSensitivityY(float sY) { sensitivityY = sY; }
    float GetSensitivityX() const { return sensitivityX; }
    float GetSensitivityY() const { return sensitivityX; }
};
