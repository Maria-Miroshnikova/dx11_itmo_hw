#pragma once

#include <vector>
#include "../GameComponent.h"
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

#include "../Vertex.h"

class Game;

class BaseRenderComponent : public GameComponent
{
public:
    struct CbDataPerObject
    {
        DirectX::SimpleMath::Matrix WorldViewProj;
        DirectX::SimpleMath::Matrix World;
        DirectX::SimpleMath::Matrix WorldView;
        DirectX::SimpleMath::Matrix InvTrWorldView;

        DirectX::SimpleMath::Vector4 emissionParams; // w = is_light, xyz = emissionColor

    };
    struct CbDataCascade
    {
        DirectX::SimpleMath::Matrix ViewProj[5];
        DirectX::SimpleMath::Vector4 Distance;
    };
protected:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> layout_;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastState_;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> shadowRastState_;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer_;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer_;
    std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> constBuffers_{};

    Microsoft::WRL::ComPtr<ID3D11Buffer> isDisplaicedBuffer_;

public:  bool is_light = false;
      DirectX::SimpleMath::Vector3 light_color;

protected:

    UINT strides_[1];
    UINT offsets_[1];
    std::vector<Vertex> points_{};
    std::vector<UINT> indices_{};
    bool passThroughVs_;
    bool colorModePs_;
    D3D_PRIMITIVE_TOPOLOGY topologyType_;
    virtual CD3D11_RASTERIZER_DESC CreateRasterizerStateDesc();
    const wchar_t* textureFileName_;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState_;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> depthSamplerState_;
    bool isShadowCasting_ = false; // будут ли рисоваться тени ОТ объектов на пол
public:
    explicit BaseRenderComponent(Game* game);
    ~BaseRenderComponent() override = default;
    void DestroyResources() override;
    void PrepareFrame() override;
    void Draw() override;
    void Initialize() override;
    void Reload() override;
    void Update() override;
};
