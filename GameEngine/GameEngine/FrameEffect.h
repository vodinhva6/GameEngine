#pragma once
#include "FrameBuffer.h"
#include "DataFormat.h"
#include <memory>
#include <vector>

class FrameEffectBlur : public FrameBuffer
{
public:
    void Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow = false, bool depth = true) override;
    void DrawedOn(ID3D11DeviceContext* immediateContext) override;
    void DrawToDebug() override;
private:
};

class FrameEffectSoftShadowMap : public FrameBuffer
{
public:
    void Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow = false, bool depth = true) override;
    void DrawedOn(ID3D11DeviceContext* immediateContext) override;
    void ClearDepthStencil(ID3D11DeviceContext* immediateContext, float depth = 1) override {}
    void ClearRenderTarget(ID3D11DeviceContext* immediateContext, float r = 0, float g = 0, float b = 0, float a = 0) override {}
    void DrawToDebug() override;
private:

};

class FrameEffectVolumetricLightScattering : public FrameBuffer
{
public:
    void Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow = false, bool depth = true) override;
    void DrawedOn(ID3D11DeviceContext* immediateContext) override;
    void DrawToDebug() override;

private:
    void BindConstantBuffer(ID3D11DeviceContext* dc, size_t slot);

private:
  
    struct ConstantBuffer
    {
        int slot;
        VECTOR3 dummy;
    };
    ConstantBuffer data;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
};

class FrameEffectMotionBlur : public FrameBuffer
{
public:
    void Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow = false, bool depth = true) override;
    void DrawedOn(ID3D11DeviceContext* immediateContext) override;
    void DrawToDebug() override;

};

class FrameEffectBloom : public FrameBuffer
{
public:
    void Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow = false, bool depth = true) override;
    void DrawedOn(ID3D11DeviceContext* immediateContext) override;
    void DrawToDebug() override;
    ~FrameEffectBloom() override;
private:
    const uint32_t downSamplingCount = 6;
    std::shared_ptr<FrameBuffer> downAndBlurSampleFrame[6];
    std::unique_ptr<FrameBuffer> downAndBlurSampleFrameSupport[6];
private:
};


class FrameEffectPSShadowMap : public FrameBuffer
{
public:
    void Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow = false, bool depth = true) override;
    void DrawedOn(ID3D11DeviceContext* immediateContext) override;
    void DrawToDebug() override;
    UINT getCascadeCount() { return cascadeCount; }
    ~FrameEffectPSShadowMap() override;
private:
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
private:
    const UINT cascadeCount = 4;
};

class FrameEffectPointLightShadowMap : public FrameBuffer
{
public:
    void Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow = false, bool depth = true) override;
    void DrawedOn(ID3D11DeviceContext* immediateContext) override;
    void DrawToDebug() override;
    UINT getSlideCount() { return slideCount; }
    ~FrameEffectPointLightShadowMap() override;
private:
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
private:
    const UINT slideCount = 6;
};