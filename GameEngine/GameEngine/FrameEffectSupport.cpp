#include "FrameTempSupport.h"
#include "FrameBufferManager.h"
#include "GameEngine.h"
#include "ShaderManager.h"


void FrameEffectSupport::Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow, bool depth)
{
    FrameBuffer::Setup(swapChain, device, viewPort, msaa, name, shadow);
}

void FrameEffectSupport::DrawedOn(ID3D11DeviceContext* immediateContext)
{
}

void FrameEffectSupport::DrawToDebug()
{
}
