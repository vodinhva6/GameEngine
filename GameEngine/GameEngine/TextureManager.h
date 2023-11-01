#pragma once
#include <d3d11.h>
#include <map>
#include <string>
#include <wrl.h>
#include <filesystem>
class TextureManager
{
public:
    void Init(ID3D11Device* device);
    bool loadTextureFromFile(ID3D11Device* device, std::filesystem::path local,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, D3D11_TEXTURE2D_DESC* texture2d_desc);
    bool LoadTextureFromMemory(ID3D11Device* device, const void* data, size_t size,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView);

    void getColorDummyTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView);
    void getEmissiveDummyTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView);
    void getOcclusionDummyTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView);
    void getMetallicDummyTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView);
    
    
    
    void getNormalDummyTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView);
    bool ReleaseTexture(std::filesystem::path);
    ~TextureManager();
private:
    bool CreateColorDummyTexture(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, UINT dimension);
    bool CreateEmissiveDummyTexture(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, UINT dimension);
    bool CreateOcclusionDummyTexture(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, UINT dimension);
    bool CreateMetallicDummyTexture(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, UINT dimension);
    bool CreateNormalDummyTexture(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, UINT dimension);
    void releaseAllTextures();
    std::map<std::filesystem::path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> resources;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> colorDummyTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalDummyTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> emissiveTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> occlusionTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metallicTexture;
};

