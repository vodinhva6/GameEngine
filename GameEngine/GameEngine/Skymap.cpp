#include "Skymap.h"
#include "GameEngine.h"
#include "Log.h"
Skymap::Skymap()
{
    textureMapLocal = "Data/Asset/Images/Skymap/FluffballDay4k/FluffballDay4k.dds";
}


void Skymap::Init(ID3D11Device* device)
{
    CreateCOM(device);
    LoadTexture();
}

void Skymap::CreateCOM(ID3D11Device* device)
{
    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(ConstantBuffer);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    HRESULT hr = device->CreateBuffer(&buffer_desc, nullptr, constantBuffer.GetAddressOf());
#ifdef USE_IMGUI
    SUCCEEDEDRESULT(hr);
#endif // USE_IMGUI
}

void Skymap::Draw(ID3D11DeviceContext* dc, uint32_t startSlot, uint32_t numViews, FullscreenQuad* blitQuad)
{
    dc->UpdateSubresource(constantBuffer.Get(), 0, 0, &colorFilerConstant, 0, 0);
    dc->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
    blitQuad->blit(dc, skymapTexture[(int)TextureName::Colormap].GetAddressOf(), 32, 1);
}

void Skymap::SetIBL(ID3D11DeviceContext* dc)
{
    dc->PSSetShaderResources(32, 1, skymapTexture[(int)TextureName::Diffuse].GetAddressOf());
    dc->PSSetShaderResources(33, 1, skymapTexture[(int)TextureName::Diffuse].GetAddressOf());
    dc->PSSetShaderResources(34, 1, skymapTexture[(int)TextureName::Specular].GetAddressOf());
    dc->PSSetShaderResources(35, 1, skymapTexture[(int)TextureName::LutGGX].GetAddressOf());
}



bool Skymap::LoadTexture()
{
    D3D11_TEXTURE2D_DESC texture2dDesc;
    bool result = true;
    result &= GameEngine::get()->LoadTextureFromFile(textureMapLocal, skymapTexture[(int)TextureName::Colormap], &texture2dDesc);
    std::filesystem::path path = textureMapLocal;
    path.replace_filename("diffuse_iem.dds");
    result &= GameEngine::get()->LoadTextureFromFile(path.string(), skymapTexture[(int)TextureName::Diffuse], &texture2dDesc);
    path.replace_filename("specular_pmrem.dds");
    result &= GameEngine::get()->LoadTextureFromFile(path.string(), skymapTexture[(int)TextureName::Specular], &texture2dDesc);
    path.replace_filename("lut_ggx.dds");
    result &= GameEngine::get()->LoadTextureFromFile(path.string(), skymapTexture[(int)TextureName::LutGGX], &texture2dDesc);


    return result;
}


Skymap::~Skymap()
{
}
