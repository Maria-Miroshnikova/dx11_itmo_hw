#include "ResourceFactory.h"
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#include "Game.h"

using namespace Assimp;

bool ResourceFactory::isInitialized_ = false;
Game* ResourceFactory::activeGame_;
std::unordered_map<std::string, ResourceFactory::VertexShaderInfo> ResourceFactory::vShaders_ {};
std::unordered_map<std::string, ResourceFactory::PixelShaderInfo> ResourceFactory::pShaders_ {};
std::unordered_map<std::string, ResourceFactory::GeometryShaderInfo> ResourceFactory::gShaders_ {};
std::unordered_map<const wchar_t*, ResourceFactory::TextureInfo> ResourceFactory::textures_ {};
std::unordered_map<std::string, ResourceFactory::GeometryInfo> ResourceFactory::meshes_ {};

void ResourceFactory::LoadTexture(const wchar_t* name)
{
    auto res = DirectX::CreateDDSTextureFromFile(activeGame_->GetDevice(), name, &textures_[name].TextureBuffer, &textures_[name].TextureView);
}

void ResourceFactory::LoadMesh(const std::string name)
{

    meshes_.insert({ name, { {}, {} } });

    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(name.c_str(), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

    ProcessNode(name, pScene->mRootNode, pScene);
}

void ResourceFactory::ProcessNode(const std::string name, aiNode* node, const aiScene* scene)
{
    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(name, mesh, scene);
    }
    for (UINT i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(name, node->mChildren[i], scene);
    }
}

void ResourceFactory::ProcessMesh(const std::string name, aiMesh* mesh, const aiScene* scene)
{
    for (UINT i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex point;

        point.pos.x = mesh->mVertices[i].x;
        point.pos.y = mesh->mVertices[i].y;
        point.pos.z = mesh->mVertices[i].z;
        point.pos.w = 1.0f;

        if (mesh->mTextureCoords[0])
        {
            point.tex.x = mesh->mTextureCoords[0][i].x;
            point.tex.y = mesh->mTextureCoords[0][i].y;
        }

        point.normal.x = mesh->mNormals[i].x;
        point.normal.y = mesh->mNormals[i].y;
        point.normal.z = mesh->mNormals[i].z;
        point.normal.w = 0.0f;

        meshes_[name].Points.push_back(point);
    }

    for (UINT i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace face = mesh->mFaces[i];

        for (UINT j = 0; j < face.mNumIndices; j++)
            meshes_[name].Indices.push_back(face.mIndices[j]);
    }
}

void ResourceFactory::Initialize(Game* game)
{
    if (isInitialized_)
        return;

    activeGame_ = game;
    isInitialized_ = true;

    vShaders_.insert({"base", {nullptr, nullptr}});
    vShaders_.insert({"csm", {nullptr, nullptr}});

    gShaders_.insert({ "csm", {nullptr, nullptr} });

    pShaders_.insert({"base", {nullptr, nullptr}});
    
    ID3DBlob* errorVertexCode = nullptr;
    auto res = D3DCompileFromFile(L"./Shaders/Base3d.hlsl",
        nullptr /*macros*/,
        nullptr /*include*/,
        "VSMain",
        "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &(vShaders_["base"].Bc),
        &errorVertexCode);

    if (FAILED(res)) {
        // If the shader failed to compile it should have written something to the error message.
        if (errorVertexCode) {
            const char* compileErrors = static_cast<char*>(errorVertexCode->GetBufferPointer());

            std::cout << compileErrors << std::endl;
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBox(game->GetDisplay()->hWnd, L"Base3d.hlsl", L"Missing Shader File", MB_OK);
        }
        //return;
    }

    errorVertexCode = nullptr;
    res = D3DCompileFromFile(L"./Shaders/Csm.hlsl",
        nullptr /*macros*/,
        nullptr /*include*/,
        "VSMain",
        "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &(vShaders_["csm"].Bc),
        &errorVertexCode);

    if (FAILED(res)) {
        // If the shader failed to compile it should have written something to the error message.
        if (errorVertexCode)
        {
            const char* compileErrors = static_cast<char*>(errorVertexCode->GetBufferPointer());

            std::cout << compileErrors << std::endl;
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBox(game->GetDisplay()->hWnd, L"Csm.hlsl", L"Missing Shader File", MB_OK);
        }
        //return;
    }

    ID3DBlob* errorGeometryCode = nullptr;
    res = D3DCompileFromFile(L"./Shaders/Csm.hlsl",
        nullptr /*macros*/,
        nullptr /*include*/,
        "GSMain",
        "gs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &(gShaders_["csm"].Bc),
        &errorGeometryCode);

    if (FAILED(res))
    {
        // If the shader failed to compile it should have written something to the error message.
        if (errorGeometryCode)
        {
            const char* compileErrors = static_cast<char*>(errorGeometryCode->GetBufferPointer());
            std::cout << compileErrors << std::endl;
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBox(game->GetDisplay()->hWnd, L"Csm.hlsl", L"Missing Shader File", MB_OK);
        }
        //return;
    }

    ID3DBlob* errorPixelCode = nullptr;
    res = D3DCompileFromFile(L"./Shaders/Base3d.hlsl",
        nullptr /*macros*/,
        nullptr /*include*/,
        "PSMain",
        "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &(pShaders_["base"].Bc),
        &errorPixelCode);

    res = game->GetDevice()->CreateVertexShader(
        vShaders_["base"].Bc->GetBufferPointer(),
        vShaders_["base"].Bc->GetBufferSize(),
        nullptr, &(vShaders_["base"].Shader));

    res = game->GetDevice()->CreateVertexShader(
        vShaders_["csm"].Bc->GetBufferPointer(),
        vShaders_["csm"].Bc->GetBufferSize(),
        nullptr, &(vShaders_["csm"].Shader));

    game->GetDevice()->CreateGeometryShader(
        gShaders_["csm"].Bc->GetBufferPointer(),
        gShaders_["csm"].Bc->GetBufferSize(),
        nullptr, &(gShaders_["csm"].Shader));

    res = game->GetDevice()->CreatePixelShader(
        pShaders_["base"].Bc->GetBufferPointer(),
        pShaders_["base"].Bc->GetBufferSize(),
        nullptr, &(pShaders_["base"].Shader));
}

void ResourceFactory::DestroyResources()
{
    if (!isInitialized_)
        return;

    for (const auto t : textures_)
    {
        t.second.TextureBuffer->Release();
        t.second.TextureView->Release();
    }
    textures_.clear();
    for (const auto vs : vShaders_)
    {
        vs.second.Bc->Release();
        vs.second.Shader->Release();
    }
    vShaders_.clear();
    for (const auto ps : pShaders_)
    {
        ps.second.Bc->Release();
        ps.second.Shader->Release();
    }
    pShaders_.clear();
    meshes_.clear();

    isInitialized_ = false;
}

ID3D11Resource* ResourceFactory::GetTextureBuffer(const wchar_t* name)
{
    if (textures_.find(name) != textures_.end())
        return textures_[name].TextureBuffer;

    LoadTexture(name);

    return textures_[name].TextureBuffer;
}

ID3D11ShaderResourceView* ResourceFactory::GetTextureView(const wchar_t* name)
{
    if (textures_.find(name) != textures_.end())
        return textures_[name].TextureView;

    LoadTexture(name);

    return textures_[name].TextureView;
}

ID3D11VertexShader* ResourceFactory::GetVertexShader(const std::string name)
{
    return vShaders_[name].Shader;
}

ID3D11PixelShader* ResourceFactory::GetPixelShader(const std::string name)
{
    return pShaders_[name].Shader;
}

ID3D11GeometryShader* ResourceFactory::GetGeometryShader(std::string name)
{
    return gShaders_[name].Shader;
}

ID3DBlob* ResourceFactory::GetVertexShaderBC(std::string name)
{
    return vShaders_[name].Bc;
}

ID3DBlob* ResourceFactory::GetPixelShaderBC(std::string name)
{
    return pShaders_[name].Bc;
}

ID3DBlob* ResourceFactory::GetGeometryShaderBC(std::string name)
{
    return gShaders_[name].Bc;
}

const std::vector<Vertex>& ResourceFactory::GetPoints(std::string name)
{
    if (meshes_.find(name) != meshes_.end())
        return meshes_[name].Points;

    LoadMesh(name);

    return meshes_[name].Points;
}

const std::vector<UINT>& ResourceFactory::GetIndices(std::string name)
{
    if (meshes_.find(name) != meshes_.end())
        return meshes_[name].Indices;

    LoadMesh(name);

    return meshes_[name].Indices;
}
