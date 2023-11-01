#include "FrameEffect.h"
#include "GameEngine.h"
#include "FrameBufferManager.h"
#include "ShaderManager.h"
#ifdef _DEBUG
#include "UsesImGui_h.h"
#endif // _DEBUG

void FrameEffectBlur::Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow, bool depth)
{
    FrameBuffer::Setup(swapChain, device, viewPort, msaa, name, shadow);
}

void FrameEffectBlur::DrawedOn(ID3D11DeviceContext* immediateContext)
{
    FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
    ShaderManager* shaderManager = GetFrom< ShaderManager>(GameEngine::get()->getShaderManager());
    frameBufferManager->Activate(immediateContext, this);



    frameBufferManager->Deactivate(immediateContext, this);
}

void FrameEffectBlur::DrawToDebug()
{

}
