#pragma once

#include "SimpleMath.h"

class Game;

class GameComponent
{
protected:
	Game* game;
public:	DirectX::SimpleMath::Vector3 position;
protected:
	DirectX::SimpleMath::Quaternion rotation;
	DirectX::SimpleMath::Vector3 scale;
	virtual void SetPosition(DirectX::SimpleMath::Vector3 p) { position = p; }
	virtual void SetRotation(DirectX::SimpleMath::Quaternion q) { rotation = q; }
	virtual void SetScale(DirectX::SimpleMath::Vector3 s) { scale = s; }
public:
	GameComponent(Game* g);
	virtual ~GameComponent() = default;
	virtual DirectX::SimpleMath::Vector3 GetPosition() const { return position; }
	virtual DirectX::SimpleMath::Quaternion GetRotation() const { return rotation; }
	virtual DirectX::SimpleMath::Vector3 GetScale() const { return scale; }
	virtual void DestroyResources() = 0;
	virtual void PrepareFrame() = 0; // Shadow passes and stuff
	virtual void Draw() = 0;
	virtual void Initialize() = 0;
	virtual void Reload() = 0;
	virtual void Update() = 0;
};
