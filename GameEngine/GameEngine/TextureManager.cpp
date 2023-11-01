#include "TextureManager.h"
#include <WICTextureLoader.h>
#include <filesystem>
#include <DDSTextureLoader.h>
#include "Misc.h"
#include "Log.h"

void TextureManager::Init(ID3D11Device* device)
{
    CreateColorDummyTexture(device, colorDummyTexture, 16);
    CreateNormalDummyTexture(device, normalDummyTexture, 16);
    CreateMetallicDummyTexture(device, metallicTexture, 16);
    CreateEmissiveDummyTexture(device, emissiveTexture, 16);
    CreateOcclusionDummyTexture(device, occlusionTexture, 16);
}

bool TextureManager::loadTextureFromFile(ID3D11Device* device, std::filesystem::path local,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, D3D11_TEXTURE2D_DESC* texture2d_desc)
{
    HRESULT hr{ S_OK };
    bool succeded = true;
    Microsoft::WRL::ComPtr<ID3D11Resource> resource;
  
    auto it = resources.find(local);
    if (it != resources.end())
    {
        shader_resource_view = it->second.Get();
        shader_resource_view->GetResource(resource.GetAddressOf());
        //*shader_resource_view = it->second.Get();
        //int a = (*shader_resource_view)->AddRef();
        //(*shader_resource_view)->GetResource(resource.GetAddressOf());
    }
    else
    {
        std::filesystem::path dds_filename(local);
        dds_filename.replace_extension("dds");
        if (std::filesystem::exists(dds_filename.c_str()))
        {
            hr = DirectX::CreateDDSTextureFromFile(device, dds_filename.c_str(),
                resource.GetAddressOf(), shader_resource_view.ReleaseAndGetAddressOf());
        }
        else
        {
            std::wstring StringName = local;
            std::wstring extension = StringName.substr(StringName.find_last_of(L".") + 1);
            if (extension == L"tga" || extension == L"TGA")
                return false;
            hr = DirectX::CreateWICTextureFromFile(device, local.wstring().c_str(), resource.GetAddressOf(), shader_resource_view.ReleaseAndGetAddressOf());

        }
        SUCCEEDEDRESULTRETURN(hr);
        resources.insert(std::make_pair(local, shader_resource_view));
    }

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
    hr = resource.Get()->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
    SUCCEEDEDRESULTRETURN(hr);
    texture2d->GetDesc(texture2d_desc);
    return true;
}

bool TextureManager::LoadTextureFromMemory(ID3D11Device* device, const void* data, size_t size, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView)
{
    HRESULT hr{ S_OK };
    Microsoft::WRL::ComPtr<ID3D11Resource> resource;

    hr = DirectX::CreateDDSTextureFromMemory(device, reinterpret_cast<const uint8_t*>(data),
        size, resource.GetAddressOf(), shaderResourceView.GetAddressOf());
    if (hr != S_OK)
    {
        hr = DirectX::CreateWICTextureFromMemory(device, reinterpret_cast<const uint8_t*>(data),
            size, resource.GetAddressOf(), shaderResourceView.GetAddressOf());
        SUCCEEDEDRESULTRETURN(hr);
    }

    return true;
}

void TextureManager::getColorDummyTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView)
{
    shaderResourceView = colorDummyTexture;
}

void TextureManager::getEmissiveDummyTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView)
{
    shaderResourceView = emissiveTexture;
}

void TextureManager::getOcclusionDummyTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView)
{
    shaderResourceView = occlusionTexture;
}

void TextureManager::getMetallicDummyTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView)
{
    shaderResourceView = metallicTexture;
}

void TextureManager::getNormalDummyTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView)
{
    shaderResourceView = normalDummyTexture;
}

bool TextureManager::CreateColorDummyTexture(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, UINT dimension)
{
    HRESULT hr{ S_OK };

    D3D11_TEXTURE2D_DESC texture2d_desc{};
    texture2d_desc.Width = dimension;
    texture2d_desc.Height = dimension;
    texture2d_desc.MipLevels = 1;
    texture2d_desc.ArraySize = 1;
    texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texture2d_desc.SampleDesc.Count = 1;
    texture2d_desc.SampleDesc.Quality = 0;
    texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
    texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    size_t texels = dimension * dimension;
    std::unique_ptr<DWORD[]> sysmem{ std::make_unique< DWORD[]>(texels) };
    for (size_t i = 0; i < texels; ++i) sysmem[i] = 0xFFFFFFFF;

    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = sysmem.get();
    subresource_data.SysMemPitch = sizeof(DWORD) * dimension;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
    hr = device->CreateTexture2D(&texture2d_desc, &subresource_data, &texture2d);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{};
    shader_resource_view_desc.Format = texture2d_desc.Format;
    shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shader_resource_view_desc.Texture2D.MipLevels = 1;
    hr = device->CreateShaderResourceView(texture2d.Get(), &shader_resource_view_desc,
        shader_resource_view.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    return true;

}

bool TextureManager::CreateEmissiveDummyTexture(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, UINT dimension)
{
    HRESULT hr{ S_OK };

    D3D11_TEXTURE2D_DESC texture2d_desc{};
    texture2d_desc.Width = dimension;
    texture2d_desc.Height = dimension;
    texture2d_desc.MipLevels = 1;
    texture2d_desc.ArraySize = 1;
    texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texture2d_desc.SampleDesc.Count = 1;
    texture2d_desc.SampleDesc.Quality = 0;
    texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
    texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    size_t texels = dimension * dimension;
    std::unique_ptr<DWORD[]> sysmem{ std::make_unique< DWORD[]>(texels) };
    for (size_t i = 0; i < texels; ++i) sysmem[i] = 0x00000000;

    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = sysmem.get();
    subresource_data.SysMemPitch = sizeof(DWORD) * dimension;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
    hr = device->CreateTexture2D(&texture2d_desc, &subresource_data, &texture2d);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{};
    shader_resource_view_desc.Format = texture2d_desc.Format;
    shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shader_resource_view_desc.Texture2D.MipLevels = 1;
    hr = device->CreateShaderResourceView(texture2d.Get(), &shader_resource_view_desc,
        shader_resource_view.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    return true;
}

bool TextureManager::CreateOcclusionDummyTexture(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, UINT dimension)
{
    HRESULT hr{ S_OK };

    D3D11_TEXTURE2D_DESC texture2d_desc{};
    texture2d_desc.Width = dimension;
    texture2d_desc.Height = dimension;
    texture2d_desc.MipLevels = 1;
    texture2d_desc.ArraySize = 1;
    texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texture2d_desc.SampleDesc.Count = 1;
    texture2d_desc.SampleDesc.Quality = 0;
    texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
    texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    size_t texels = dimension * dimension;
    std::unique_ptr<DWORD[]> sysmem{ std::make_unique< DWORD[]>(texels) };
    for (size_t i = 0; i < texels; ++i) sysmem[i] = 0xFFFFFFFF;

    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = sysmem.get();
    subresource_data.SysMemPitch = sizeof(DWORD) * dimension;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
    hr = device->CreateTexture2D(&texture2d_desc, &subresource_data, &texture2d);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{};
    shader_resource_view_desc.Format = texture2d_desc.Format;
    shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shader_resource_view_desc.Texture2D.MipLevels = 1;
    hr = device->CreateShaderResourceView(texture2d.Get(), &shader_resource_view_desc,
        shader_resource_view.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    return true;
}

bool TextureManager::CreateMetallicDummyTexture(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, UINT dimension)
{
    HRESULT hr{ S_OK };

    D3D11_TEXTURE2D_DESC texture2d_desc{};
    texture2d_desc.Width = dimension;
    texture2d_desc.Height = dimension;
    texture2d_desc.MipLevels = 1;
    texture2d_desc.ArraySize = 1;
    texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texture2d_desc.SampleDesc.Count = 1;
    texture2d_desc.SampleDesc.Quality = 0;
    texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
    texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    size_t texels = dimension * dimension;
    std::unique_ptr<DWORD[]> sysmem{ std::make_unique< DWORD[]>(texels) };
    for (size_t i = 0; i < texels; ++i) sysmem[i] = 0xFFFFFFFF;

    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = sysmem.get();
    subresource_data.SysMemPitch = sizeof(DWORD) * dimension;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
    hr = device->CreateTexture2D(&texture2d_desc, &subresource_data, &texture2d);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{};
    shader_resource_view_desc.Format = texture2d_desc.Format;
    shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shader_resource_view_desc.Texture2D.MipLevels = 1;
    hr = device->CreateShaderResourceView(texture2d.Get(), &shader_resource_view_desc,
        shader_resource_view.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    return true;
}

bool TextureManager::CreateNormalDummyTexture(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, UINT dimension)
{
    HRESULT hr{ S_OK };

    D3D11_TEXTURE2D_DESC texture2d_desc{};
    texture2d_desc.Width = dimension;
    texture2d_desc.Height = dimension;
    texture2d_desc.MipLevels = 1;
    texture2d_desc.ArraySize = 1;
    texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texture2d_desc.SampleDesc.Count = 1;
    texture2d_desc.SampleDesc.Quality = 0;
    texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
    texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    size_t texels = dimension * dimension;
    std::unique_ptr<DWORD[]> sysmem{ std::make_unique< DWORD[]>(texels) };
    for (size_t i = 0; i < texels; ++i) sysmem[i] = 0xFFFF7F7F;

    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = sysmem.get();
    subresource_data.SysMemPitch = sizeof(DWORD) * dimension;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
    hr = device->CreateTexture2D(&texture2d_desc, &subresource_data, &texture2d);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{};
    shader_resource_view_desc.Format = texture2d_desc.Format;
    shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shader_resource_view_desc.Texture2D.MipLevels = 1;
    hr = device->CreateShaderResourceView(texture2d.Get(), &shader_resource_view_desc,
        shader_resource_view.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    return true;
}

bool TextureManager::ReleaseTexture(std::filesystem::path local)
{
    auto it = resources.find(local);
    if (it != resources.end())
    {
        if (it->first == local)
        {
            int a = it->second->Release();
            it->second->AddRef();
            if (a == 0)
            {
                resources.erase(it);
            }
            return true;
        }
    }
    return false;
}

void TextureManager::releaseAllTextures()
{
    resources.clear();
}

TextureManager::~TextureManager()
{
    releaseAllTextures();
}
