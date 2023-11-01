#include "FrameDebug.h"
#include "GameEngine.h"
#include "FrameBufferManager.h"
#include "ShaderManager.h"
void FramebufferGeometricDebug::Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow, bool depth)
{
    FrameBuffer::Setup(swapChain, device, viewPort, msaa, name, shadow);
}

void FramebufferGeometricDebug::DrawedOn(ID3D11DeviceContext* immediateContext)
{
    FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
    ShaderManager* shaderManager = GetFrom< ShaderManager>(GameEngine::get()->getShaderManager());

    frameBufferManager->Activate(immediateContext, this);
    shaderManager->BeginGeometricDrawDebug(immediateContext);
    GameEngine::get()->DrawAllUninvisibleObject(FrameBufferName::FRAMEGEOMETRICDEBUGER,DrawType::BOUDING);
    frameBufferManager->Deactivate(immediateContext, this);
}

void FramebufferGeometricDebug::DrawToDebug()
{
}


