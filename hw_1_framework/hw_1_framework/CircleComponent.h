#pragma once
#include "GameComponent.h"
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

const int vertCount = 8;

class CircleComponent : public GameComponent {
private:
	ID3D11InputLayout* layout;

	ID3D11PixelShader* pixelShader;
	ID3DBlob* pixelShaderByteCode;
	//int points; // ??

	ID3D11RasterizerState* rastState;

	ID3D11VertexShader* vertexShader;
	ID3DBlob* vertexShaderByteCode;
	//int vertices; // ??

	UINT strides[1];
	UINT offsets[1];
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	float radius = 1;
	float x = -1.0f;
	float y = 0.0f;

	ID3D11Buffer* constBuffer;
	DirectX::XMFLOAT4 offset = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	float pseudotime = 0;
	float amplituda;

public:
	CircleComponent(Game* game);
	//CircleComponent(Game* game, int vertCount_);
	CircleComponent(Game* game, float x, float y, float rad, float amplituda);
	~CircleComponent();

	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Update() override;
	void Reload() override;
};