#include "FrameBuffer.h"
#include "GameEngine.h"
void FrameBuffer::Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow, bool depth)
{
    renderTargetView.Reset();
    depthStencilView.Reset();
    shaderResourceView[0].Reset();
    shaderResourceView[1].Reset();
    this->name = name;
    viewport_ = viewPort;

    HRESULT hr{ S_OK };
    Microsoft::WRL::ComPtr<ID3D11Texture2D> renderTargetBuffer;
    D3D11_TEXTURE2D_DESC texture2dDesc{};
    texture2dDesc.Width = static_cast<UINT>(viewPort.Width);
    texture2dDesc.Height = static_cast<UINT>(viewPort.Height);
    texture2dDesc.MipLevels = 1;
    texture2dDesc.ArraySize = 1;
    texture2dDesc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
    texture2dDesc.SampleDesc.Count = msaa ? 4 : 1;
    texture2dDesc.SampleDesc.Quality = 0;
    texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
    texture2dDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texture2dDesc.CPUAccessFlags = 0;
    texture2dDesc.MiscFlags = 0;
    hr = device->CreateTexture2D(&texture2dDesc, 0, renderTargetBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
    renderTargetViewDesc.Format = texture2dDesc.Format;
    renderTargetViewDesc.ViewDimension = msaa ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
    hr = device->CreateRenderTargetView(renderTargetBuffer.Get(), &renderTargetViewDesc, renderTargetView.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
    shaderResourceViewDesc.Format = texture2dDesc.Format;
    shaderResourceViewDesc.ViewDimension = msaa ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;
    hr = device->CreateShaderResourceView(renderTargetBuffer.Get(), &shaderResourceViewDesc,
        shaderResourceView[0].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
    texture2dDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    texture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    hr = device->CreateTexture2D(&texture2dDesc, 0, depthStencilBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    if (depth)
    {
        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
        depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilViewDesc.ViewDimension = msaa ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc.Flags = 0;
        hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc,
            depthStencilView.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }
    shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    hr = device->CreateShaderResourceView(depthStencilBuffer.Get(), &shaderResourceViewDesc,
        shaderResourceView[1].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
   
}

void FrameBuffer::ClearDepthStencil(ID3D11DeviceContext* immediateContext, float depth)
{
    if(depthStencilView.Get())
    immediateContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, 0);
}

void FrameBuffer::ClearRenderTarget(ID3D11DeviceContext* immediateContext, float r, float g, float b, float a)
{
    float color[4]{ r,g,b,a};
    if(renderTargetView.Get())
    immediateContext->ClearRenderTargetView(renderTargetView.Get(), color);
}

void FrameBuffer::Resize(const D3D11_VIEWPORT& viewport, bool msaa, bool shadow)
{
    Setup(GameEngine::get()->getSwapChain(), GameEngine::get()->getDevice(), viewport, msaa, name, shadow);
}

FrameBuffer::~FrameBuffer()
{
    int a = 1;
    a = 2;
}