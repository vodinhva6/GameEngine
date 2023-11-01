#include "FrameSprite2D.h"
#include "GameEngine.h"
#include "FrameBufferManager.h"
#include "ShaderManager.h"
#ifdef USE_IMGUI
#include "UsesImGui_h.h"
#endif // USE_IMGUI

void Framebuffer2DEarly::Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow, bool depth)
{
    renderTargetView.Reset();
    depthStencilView.Reset();
    shaderResourceView[0].Reset();
    shaderResourceView[1].Reset();
    this->name = name;
    HRESULT hr{ S_OK };
    Microsoft::WRL::ComPtr<ID3D11Texture2D> renderTargetBuffer;
    D3D11_TEXTURE2D_DESC texture2dDesc{};
    texture2dDesc.Width = static_cast<UINT>(viewPort.Width);
    texture2dDesc.Height = static_cast<UINT>(viewPort.Height);
    texture2dDesc.MipLevels = 1;
    texture2dDesc.ArraySize = 1;
    texture2dDesc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
    texture2dDesc.SampleDesc.Count = 1;
    texture2dDesc.SampleDesc.Quality = 0;
    texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
    texture2dDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texture2dDesc.CPUAccessFlags = 0;
    texture2dDesc.MiscFlags = 0;
    hr = device->CreateTexture2D(&texture2dDesc, 0, renderTargetBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
    renderTargetViewDesc.Format = texture2dDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    hr = device->CreateRenderTargetView(renderTargetBuffer.Get(), &renderTargetViewDesc, renderTargetView.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
    shaderResourceViewDesc.Format = texture2dDesc.Format;
    shaderResourceViewDesc.ViewDimension =D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;
    hr = device->CreateShaderResourceView(renderTargetBuffer.Get(), &shaderResourceViewDesc,
        shaderResourceView[0].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    viewport_ = viewPort;
}

void Framebuffer2DEarly::DrawedOn(ID3D11DeviceContext* immediateContext)
{
#ifdef USE_IMGUI
    ImGui::Begin("2D Scene");
    {

    }
    ImGui::End();
#endif // USE_IMGUI
    FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
    ShaderManager* shaderManager = GetFrom< ShaderManager>(GameEngine::get()->getShaderManager());

    frameBufferManager->Activate(immediateContext, this);


    shaderManager->BeginDraw2D(immediateContext);
    GameEngine::get()->DrawAllUninvisibleObject(FrameBufferName::FRAMEBUFFER2DEARLY, DrawType::TEXTUREOBJECT);

    frameBufferManager->Deactivate(immediateContext, this);
}

void Framebuffer2DEarly::DrawToDebug()
{
#ifdef USE_IMGUI
    ImGui::Begin("2D Scene");
    {
        ImVec2 ImSize = ImGui::GetContentRegionAvail();
        if (ImSize.x < 0 || ImSize.y < 0)
        {
            ImGui::End();
            return;
        }
        float tSizeY = (ImSize.x * (9.0f / 16.0f));
        float tSizeX = (ImSize.y * (16.0f / 9.0f));
        if (tSizeX < ImSize.x)
            ImSize.x = tSizeX;
        else
            if (tSizeY < ImSize.y)
                ImSize.y = tSizeY;
        ImGui::Image(shaderResourceView[0].Get(), { ImSize.x - 1, ImSize.y - 1 });
    }
    ImGui::End();
#endif // USE_IMGUI
}

