#include "FrameTempSupport.h"
#include "FrameBufferManager.h"
#include "GameEngine.h"
#include "ShaderManager.h"
#ifdef USE_IMGUI
#include "UsesImGui_h.h"
#endif // USE_IMGUI



void FrameDummySupport::Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow, bool depth)
{
    FrameBuffer::Setup(swapChain, device, viewPort, msaa, name, shadow, depth);
}

void FrameDummySupport::DrawedOn(ID3D11DeviceContext* immediateContext)
{
}

void FrameDummySupport::DrawToDebug()
{
}
