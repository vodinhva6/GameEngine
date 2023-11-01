#include "FrameDebug.h"
#include "GameEngine.h"
#include "FrameBufferManager.h"
#include "ShaderManager.h"
void FramebufferSkinnedMeshDebug::Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow, bool depth)
{
    FrameBuffer::Setup(swapChain, device, viewPort, msaa, name, shadow);
}

void FramebufferSkinnedMeshDebug::DrawedOn(ID3D11DeviceContext* immediateContext)
{
    FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
    ShaderManager* shaderManager = GetFrom< ShaderManager>(GameEngine::get()->getShaderManager());

    frameBufferManager->Activate(immediateContext, this);
    shaderManager->BeginSkinnedMeshDrawDebug(immediateContext);
    GameEngine::get()->DrawAllUninvisibleObject(FrameBufferName::FRAMESKINNEDMESHDEBUG, DrawType::SkinnedMeshObject);
    frameBufferManager->Deactivate(immediateContext, this);
}

void FramebufferSkinnedMeshDebug::DrawToDebug()
{
}
