#include "FrameEffect.h"
#include "FrameBufferManager.h"
#include "GameEngine.h"
#include "ShaderManager.h"
#ifdef USE_IMGUI
#include "UsesImGui_h.h"
#endif // USE_IMGUI

void FrameEffectMotionBlur::Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow, bool depth)
{
    FrameBuffer::Setup(swapChain, device, viewPort, msaa, name, shadow, depth);
}

void FrameEffectMotionBlur::DrawedOn(ID3D11DeviceContext* immediateContext)
{
    FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
    ShaderManager* shaderManager = GetFrom< ShaderManager>(GameEngine::get()->getShaderManager());

    FrameBuffer* frameBuffer = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMESAMPLE);
    FrameBuffer* tempBuffer = frameBufferManager->getFrameBuffer(FrameBufferName::MSAABUFFER1);

    frameBufferManager->Activate(immediateContext, this);
    shaderManager->BeginBlitFullScreenQuadWithMotionBlur(immediateContext);
    immediateContext->PSSetShaderResources(1, 1, tempBuffer->getShaderResourceView(1).GetAddressOf());
    
    frameBufferManager->BlitFrom(immediateContext, frameBuffer);
    
    ID3D11ShaderResourceView* dummy = nullptr;
    immediateContext->PSSetShaderResources(1, 1, &dummy);
    frameBufferManager->Deactivate(immediateContext, this);
}

void FrameEffectMotionBlur::DrawToDebug()
{
}
