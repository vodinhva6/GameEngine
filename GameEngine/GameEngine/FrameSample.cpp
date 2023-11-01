#include "FrameSample.h"
#include "FrameBufferManager.h"
#include "GameEngine.h"
#include "ShaderManager.h"
#ifdef USE_IMGUI
#include "UsesImGui_h.h"
#endif // USE_IMGUI



void FrameSample::Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow, bool depth)
{
    FrameBuffer::Setup(swapChain, device, viewPort, msaa, name, shadow, depth);
}

void FrameSample::DrawedOn(ID3D11DeviceContext* immediateContext)
{
    FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
    ShaderManager* shaderManager = GetFrom< ShaderManager>(GameEngine::get()->getShaderManager());

    FrameBuffer* frameBuffer = frameBufferManager->getFrameBuffer(FrameBufferName::MSAABUFFER1);
    FrameBuffer* tempBuffer = frameBufferManager->getFrameBuffer(FrameBufferName::MSAATEMPFRAMEBUFFER);

    frameBufferManager->Resolve(GameEngine::get()->getDevice(), immediateContext, frameBuffer, tempBuffer);
   
    frameBufferManager->Activate(immediateContext, this);
    
    shaderManager->BeginBlitFullScreenQuad(immediateContext);
    frameBufferManager->BlitFrom(immediateContext, tempBuffer);
    shaderManager->End(immediateContext);
    frameBufferManager->Deactivate(immediateContext, this);

}

void FrameSample::DrawToDebug()
{
#ifdef USE_IMGUI
#endif // USE_IMGUI
}
