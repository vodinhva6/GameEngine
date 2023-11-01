#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <cstdint>
#include "Misc.h"
#include "FramebufferNumName.h"
class FrameBuffer
{
public:
    virtual void Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow = false, bool depth = true);
    virtual void ClearDepthStencil(ID3D11DeviceContext* immediateContext, float depth = 1);
    virtual void ClearRenderTarget(ID3D11DeviceContext* immediateContext, float r = 0, float g = 0, float b = 0, float a = 0);
    virtual void DrawedOn(ID3D11DeviceContext* immediateContext) {}
    virtual void DrawToDebug() {}
    virtual void Resize(const D3D11_VIEWPORT& viewport, bool msaa = false, bool shadow = false);
    virtual ~FrameBuffer();
public:
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& getRenderTargetView() { return renderTargetView; }
    ID3D11DepthStencilView* getDepthStencilView() { return depthStencilView.Get(); }
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& getShaderResourceView(int slotNum) { return shaderResourceView[slotNum]; }
    FrameBufferName getFrameBufferName() { return name; }
    D3D11_VIEWPORT& getViewPort() { return viewport_; }

protected:
    FrameBufferName name;
   
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView[2];
    D3D11_VIEWPORT viewport_;
};
