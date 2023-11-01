#pragma once
#include "FrameBuffer.h"


class FramebufferMainBlit : public FrameBuffer
{
public:
    void Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow = false, bool depth = true) override;
    void DrawedOn(ID3D11DeviceContext* immediateContext) override;
    void DrawToDebug() override;
private:
    void Draw2DEarly(ID3D11DeviceContext* immediateContext);
    void Draw2DLate(ID3D11DeviceContext* immediateContext);
    void Draw3D(ID3D11DeviceContext* immediateContext);
};

