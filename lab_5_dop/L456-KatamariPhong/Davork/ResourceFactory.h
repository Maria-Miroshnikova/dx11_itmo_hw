#pragma once
#include <unordered_map>
#include "Vertex.h"
#include "DDSTextureLoader.h"
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

class Game;

class ResourceFactory
{
public:
    struct TextureInfo
    {
        ID3D11Resource* TextureBuffer;
        ID3D11ShaderResourceView* TextureView;
    };
    struct VertexShaderInfo
    {
        ID3D11VertexShader* Shader;
        ID3DBlob* Bc;
    };
    struct PixelShaderInfo
    {
        ID3D11PixelShader* Shader;
        ID3DBlob* Bc;
    };
    struct GeometryShaderInfo
    {
        ID3D11GeometryShader* Shader;
        ID3DBlob* Bc;
    };
    struct GeometryInfo
    {
        std::vector<Vertex> Points {};
        std::vector<UINT> Indices {};
    };
private:
    static bool isInitialized_;
    static Game* activeGame_;
    static std::unordered_map<std::string, VertexShaderInfo> vShaders_;
    static std::unordered_map<std::string, PixelShaderInfo> pShaders_;
    static std::unordered_map<std::string, GeometryShaderInfo> gShaders_;
    static std::unordered_map<const wchar_t*, TextureInfo> textures_;
    static std::unordered_map<std::string, GeometryInfo> meshes_;
    static void LoadTexture(const wchar_t* name);
    static void LoadMesh(std::string name);
    static void ProcessNode(const std::string name, aiNode* node, const aiScene* scene);
    static void ProcessMesh(const std::string name, aiMesh* mesh, const aiScene* scene);
public:
    ResourceFactory() = delete;
    static void Initialize(Game* game);
    static bool IsInitialized() { return isInitialized_; }
    static void DestroyResources();
    static ID3D11Resource* GetTextureBuffer(const wchar_t* name);
    static ID3D11ShaderResourceView* GetTextureView(const wchar_t* name);
    static ID3D11VertexShader* GetVertexShader(std::string name);
    static ID3D11PixelShader* GetPixelShader(std::string name);
    static ID3D11GeometryShader* GetGeometryShader(std::string name);
    static ID3DBlob* GetVertexShaderBC(std::string name);
    static ID3DBlob* GetPixelShaderBC(std::string name);
    static ID3DBlob* GetGeometryShaderBC(std::string name);
    static const std::vector<Vertex>& GetPoints(std::string name);
    static const std::vector<UINT>& GetIndices(std::string name);
};
