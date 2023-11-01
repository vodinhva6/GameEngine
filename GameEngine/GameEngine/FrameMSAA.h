#pragma once
#include "FrameBuffer.h"
class FramebufferMSAA1 : public FrameBuffer
{
public:
    void Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow = false, bool depth = true) override;
    void DrawedOn(ID3D11DeviceContext* immediateContext) override;
    void DrawToDebug() override;
private:
};

class FramebufferMSAA2 : public FrameBuffer
{
public:
    void Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow = false, bool depth = true) override;
    void DrawedOn(ID3D11DeviceContext* immediateContext) override;
    void DrawToDebug() override;
private:
};

class FramebufferMSAA3 : public FrameBuffer
{
public:
    void Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow = false, bool depth = true) override;
    void DrawedOn(ID3D11DeviceContext* immediateContext) override;
    void DrawToDebug() override;
private:
};
