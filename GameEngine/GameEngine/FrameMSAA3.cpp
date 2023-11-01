#include "FrameMSAA.h"
#include "GameEngine.h"
#include "FrameBufferManager.h"
#include "ShaderManager.h"
void FramebufferMSAA3::Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow, bool depth)
{
    FrameBuffer::Setup(swapChain, device, viewPort, msaa, name, shadow);
}

void FramebufferMSAA3::DrawedOn(ID3D11DeviceContext* immediateContext)
{
    FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
    ShaderManager* shaderManager = GetFrom< ShaderManager>(GameEngine::get()->getShaderManager());
    frameBufferManager->Activate(immediateContext, this);

    shaderManager->BeginDraw3D(immediateContext);
    GameEngine::get()->DrawAllUninvisibleObject(FrameBufferName::MSAABUFFER3, DrawType::SkinnedMeshObject);
    frameBufferManager->Deactivate(immediateContext, this);
}

void FramebufferMSAA3::DrawToDebug()
{
}