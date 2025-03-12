#pragma once

class Game;

class GameComponent {
	
public:
	Game* game;
	GameComponent(Game* game);
	virtual ~GameComponent();

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void DestroyResources() = 0;
	virtual void Reload() = 0;

};