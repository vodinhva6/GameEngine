#include "FrameEffect.h"
#include "GameEngine.h"

void FrameEffectSoftShadowMap::Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow, bool depth)
{
    this->name = name;
    HRESULT hr{ S_OK };
    D3D11_TEXTURE2D_DESC texture2d_desc = {};
    texture2d_desc.Width = static_cast<UINT>(viewPort.Width);
    texture2d_desc.Height = static_cast<UINT>(viewPort.Height);
    texture2d_desc.MipLevels = 1;
    texture2d_desc.ArraySize = 1;
    texture2d_desc.Format = DXGI_FORMAT_R16_TYPELESS;
    texture2d_desc.SampleDesc.Count = 1;
    texture2d_desc.SampleDesc.Quality = 0;
    texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
    texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    texture2d_desc.CPUAccessFlags = 0;
    texture2d_desc.MiscFlags = 0;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
    hr = device->CreateTexture2D(&texture2d_desc, 0, texture2d.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc = {};
    depth_stencil_view_desc.Format = DXGI_FORMAT_D16_UNORM;
    depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depth_stencil_view_desc.Flags = 0;
    hr = device->CreateDepthStencilView(texture2d.Get(), &depth_stencil_view_desc, depthStencilView.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc = {};
    shader_resource_view_desc.Format = DXGI_FORMAT_R16_UNORM;
    shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shader_resource_view_desc.Texture2D.MipLevels = 1;
    hr = device->CreateShaderResourceView(texture2d.Get(), &shader_resource_view_desc, shaderResourceView[0].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

void FrameEffectSoftShadowMap::DrawedOn(ID3D11DeviceContext* immediateContext)
{
    //GraphicEngine* graphicEngine = GameEngine::get()->getGraphicEngine();
    //FrameBufferManager* frameBufferManager = graphicEngine->getFrameBufferManager();
    //frameBufferManager->Activate(immediateContext, this);
    //
    //immediateContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    //ID3D11RenderTargetView* null_rtv = nullptr;
    //immediateContext->OMSetRenderTargets(0, &null_rtv, depthStencilView.Get());
    //graphicEngine->getShaderManager()->BeginDraw3DCreateDepthBufferOfShadow(immediateContext);
    //graphicEngine->DrawAllUninvisibleObject(0);
    //graphicEngine->DrawAllUninvisibleObject(1);
    //graphicEngine->DrawAllUninvisibleObject(2);
    //frameBufferManager->Deactivate(immediateContext, this);
}

void FrameEffectSoftShadowMap::DrawToDebug()
{
}