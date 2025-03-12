#pragma once
#include "GameComponent.h"
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

class TriangleComponent : public GameComponent {
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

public:
	TriangleComponent(Game* game);
	~TriangleComponent();

	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Update() override;
	void Reload() override;
};