#pragma once
#include "FrameBuffer.h"

class FrameMSAATempSupport : public FrameBuffer
{
public:
    void Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow = false, bool depth = true) override;
    void DrawedOn(ID3D11DeviceContext* immediateContext) override;
    void DrawToDebug() override;
    void ClearDepthStencil(ID3D11DeviceContext* immediateContext, float depth = 1) override {}
private:
    void Resize(const D3D11_VIEWPORT& viewport, bool msaa = false, bool shadow = false) override {}

};


class FrameEffectSupport : public FrameBuffer
{
public:
    void Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow = false, bool depth = true) override;
    void DrawedOn(ID3D11DeviceContext* immediateContext) override;
    void DrawToDebug() override;

};



class FrameDummySupport : public FrameBuffer
{
public:
    void Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow = false, bool depth = true) override;
    void DrawedOn(ID3D11DeviceContext* immediateContext) override;
    void DrawToDebug() override;

};
