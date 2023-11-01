#pragma once
#include "FrameBuffer.h"
class FrameSample : public FrameBuffer
{
public:
    void Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow = false, bool depth = true) override;
    void DrawedOn(ID3D11DeviceContext* immediateContext) override;
    void DrawToDebug() override;

private:


};

