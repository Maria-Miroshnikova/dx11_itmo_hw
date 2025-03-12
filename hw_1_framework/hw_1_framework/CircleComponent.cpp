#include "CircleComponent.h"
#include "Game.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

CircleComponent::CircleComponent(Game* game) : GameComponent(game) {
	strides[0] = 32;
	offsets[0] = 0;
}

/*CircleComponent::CircleComponent(Game* game, int vertCount_) : GameComponent(game), vertCount(vertCount_) {
	strides[0] = 32;
	offsets[0] = 0;
}*/

CircleComponent::CircleComponent(Game* game, float center_pos_x, float center_pos_y, float rad, float amplituda_ = 1) : 
	GameComponent(game), radius(rad), x(center_pos_x), y(center_pos_y), amplituda(amplituda_) {
	strides[0] = 32;
	offsets[0] = 0;
}

CircleComponent::~CircleComponent() {

}

void CircleComponent::Initialize() {
	//------------------------------------
	// 	// stage 4
	ID3DBlob* errorVertexCode = nullptr;
	auto res = D3DCompileFromFile(L"./Shaders/CircleShiftShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexShaderByteCode,
		&errorVertexCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(game->Display->hWnd, L"CircleShiftShader.hlsl", L"Missing Shader File", MB_OK);
		}

		return;
	}

	// это цвет зеленого прямоугольника. Чтобы половина окрасилась в него, надо этот макрос передать в следующий компайл шейдер
	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"./Shaders/CircleShiftShader.hlsl",
		nullptr,//Shader_Macros /*macros*/, 
		nullptr /*include*/,
		"PSMain", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelShaderByteCode,
		&errorPixelCode);

	game->Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader);

	game->Device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader);

	///--------------------------------
	// stage 5

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};

	game->Device->CreateInputLayout(
		inputElements,
		2,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&layout);

	//---------------------------------
	// stage 6

	// (координаты, цвет) - парами

	const int max_ver = (vertCount + 1) * 2;
	DirectX::XMFLOAT4 points[max_ver] = {};
	DirectX::XMFLOAT4 center = DirectX::XMFLOAT4(x, y, 0.0f, 1.0f);
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	points[0] = center;
	points[1] = color;
	float x_offset;
	float y_offset;
	int k = 0;

	for (int i = 2; i < max_ver; i += 2) {
		x_offset = radius * cos(2 * M_PI * k / vertCount);
		y_offset = radius * sin(2 * M_PI * k / vertCount);
		points[i] = DirectX::XMFLOAT4(center.x + x_offset, center.y + y_offset, center.z, center.w);
		points[i + 1] = color;
		k += 1;
	}

	for (int i = 0; i < max_ver; i += 2) {
		std::cout << points[i].x << " " << points[i].y << " " << std::endl;
	}

	//---------------------------------
	// stage 7

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	game->Device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);

	int indeces[vertCount * 3] = {};
	k = 1;
	for (int i = 0; i < ((vertCount - 1) * 3); i += 3) {
		indeces[i] = 0;
		indeces[i + 1] = k;
		indeces[i + 2] = k + 1;
		k += 1;
	}
	indeces[vertCount * 3 - 3] = 0;
	indeces[vertCount * 3 - 3 + 1] = vertCount;
	indeces[vertCount * 3 - 3 + 2] = 1;
	std::cout << "index: " << std::endl;
	for (int i = 0; i < (vertCount) * 3; i++) {
		std::cout << indeces[i] << " ";
	}
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indeces;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	game->Device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = 0;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4);

	game->Device->CreateBuffer(&constBufDesc, nullptr, &constBuffer);

	//-------------------------------------
	// stage 10

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = game->Device->CreateRasterizerState(&rastDesc, &rastState);
}

void CircleComponent::Draw() {
	game->Context->RSSetState(rastState);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(game->Display->ClientWidth);
	viewport.Height = static_cast<float>(game->Display->ClientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	game->Context->RSSetViewports(1, &viewport);

	// -----------------------------------------------------------------
	// stage 8

	game->Context->IASetInputLayout(layout);
	game->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->Context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	game->Context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);

	// -----------------------------------------------------------------
	// stage 9

	game->Context->VSSetShader(vertexShader, nullptr, 0);
	game->Context->VSSetConstantBuffers(0, 1, &constBuffer);
	game->Context->PSSetShader(pixelShader, nullptr, 0);

	// -------------------------------------------------------------------
	game->Context->DrawIndexed(vertCount * 3, 0, 0);
}

// undefined
void CircleComponent::Update() {
	
	// linear
//	offset.x += 0.01;
	
	// sinx
	//offset.x += 0.01; //game->TotalTime;
	//offset.y = sin((offset.x) * 8) / 2;

	// sinx
	//pseudotime += 0.01;
	//offset.x = pseudotime - 2 * floor(pseudotime / 2);
	//offset.y = sin(pseudotime * 8) / 2;
	
	// sin x с разной скоростью и амплитудой
	pseudotime += amplituda / 200;
	offset.x = pseudotime - 2 * floor(pseudotime / 2);
	offset.y = sin(pseudotime * amplituda) / 8;

	//if (offset.x > 2.0f) {
	//	offset.x = 0.0f;
	//}
	game->Context->UpdateSubresource(constBuffer, 0, 0, &offset, 0, 0);
}



// undefined
void CircleComponent::Reload() {

}

void CircleComponent::DestroyResources() {
	layout->Release();
	pixelShader->Release();
	pixelShaderByteCode->Release();
	rastState->Release();
	vertexShader->Release();
	vertexShaderByteCode->Release();
	vertexBuffer->Release();
	indexBuffer->Release();
}