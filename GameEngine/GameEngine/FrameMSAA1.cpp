#include "FrameMSAA.h"
#include "GameEngine.h"
#include "FrameBufferManager.h"
#include "ShaderManager.h"
#ifdef USE_IMGUI
#include "UsesImGui_h.h"
#endif // USE_IMGUI

void FramebufferMSAA1::Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow, bool depth)
{
	FrameBuffer::Setup(swapChain, device, viewPort, msaa, name, shadow);

}
void FramebufferMSAA1::DrawedOn(ID3D11DeviceContext* immediateContext)
{
    FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
    ShaderManager* shaderManager = GetFrom< ShaderManager>(GameEngine::get()->getShaderManager());
    frameBufferManager->Activate(immediateContext, this);

    GameEngine::get()->DrawSkymap();
    GameEngine::get()->SetIBLSkyMap();
    shaderManager->BeginDraw3D(immediateContext);
    GameEngine::get()->DrawAllUninvisibleObject(FrameBufferName::MSAABUFFER1, DrawType::SkinnedMeshObject);
    shaderManager->End(immediateContext);

    frameBufferManager->Deactivate(immediateContext, this);
}

void FramebufferMSAA1::DrawToDebug()
{
}