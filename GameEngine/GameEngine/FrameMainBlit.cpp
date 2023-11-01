#include "FrameMainBlit.h"
#include "FrameBufferManager.h"
#include "GameEngine.h"
#include "ShaderManager.h"
#include "CameraManager.h"
#ifdef USE_IMGUI
#include "UsesImGui_h.h"
#endif // USE_IMGUI
void FramebufferMainBlit::Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow, bool depth)
{
    FrameBuffer::Setup(swapChain, device, viewPort, msaa, name, shadow, depth);
}

void FramebufferMainBlit::DrawedOn(ID3D11DeviceContext* immediateContext)
{
#ifdef USE_IMGUI
    ImGui::Begin("Game");
    {
        EngineRunMode mode = GameEngine::get()->getGameMode();
        std::shared_ptr<Camera> gameCamera = 
            GetFrom<CameraManager>(GameEngine::get()->getCameraManager())->getCamera(CameraName::GameScene);
        if(mode == EngineRunMode::EngineGameMode && gameCamera->getFPSLock())
            ImGui::SetWindowFocus();
    }
    ImGui::End();
#endif // USE_IMGUI
    FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
    ShaderManager* shaderManager = GetFrom< ShaderManager>(GameEngine::get()->getShaderManager());

    FrameBuffer* frameDummy = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMEDUMMYSUPPORT);
    frameBufferManager->ClearFramebuffer(immediateContext, frameDummy);

    frameBufferManager->Activate(immediateContext, this);
   
    Draw2DEarly(immediateContext);
    Draw3D(immediateContext);
    Draw2DLate(immediateContext);
   
    frameBufferManager->Deactivate(immediateContext, this);

}

void FramebufferMainBlit::DrawToDebug()
{
#ifdef USE_IMGUI
    ImGui::Begin("Game");
    ImVec2 ImSize = ImGui::GetContentRegionAvail();
    if (ImSize.x < 0 || ImSize.y < 0)
    {
        ImGui::End();
        return;
    }
    float tSizeY = (ImSize.x * (9.0f / 16.0f));
    float tSizeX = (ImSize.y * (16.0f / 9.0f));
    if (tSizeX < ImSize.x)
        ImSize.x = tSizeX;
    else
        if (tSizeY < ImSize.y)
            ImSize.y = tSizeY;
    ImGui::Image(shaderResourceView[0].Get(), { ImSize.x, ImSize.y });
    ImGui::End();
#endif // USE_IMGUI
}

void FramebufferMainBlit::Draw2DEarly(ID3D11DeviceContext* immediateContext)
{
    FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
    ShaderManager* shaderManager = GetFrom< ShaderManager>(GameEngine::get()->getShaderManager());

    FrameBuffer* frameBuffer = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMEBUFFER2DEARLY);
    shaderManager->BeginBlitFullScreenQuad(immediateContext);
    frameBufferManager->BlitFrom(immediateContext, frameBuffer);
    shaderManager->End(immediateContext);
}

void FramebufferMainBlit::Draw2DLate(ID3D11DeviceContext* immediateContext)
{
    FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
    ShaderManager* shaderManager = GetFrom< ShaderManager>(GameEngine::get()->getShaderManager());

    FrameBuffer* frameBuffer = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMEBUFFER2DLATE);
    shaderManager->BeginBlitFullScreenQuad(immediateContext);
    frameBufferManager->BlitFrom(immediateContext, frameBuffer);
    shaderManager->End(immediateContext);
}

void FramebufferMainBlit::Draw3D(ID3D11DeviceContext* immediateContext)
{
    FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
    ShaderManager* shaderManager = GetFrom< ShaderManager>(GameEngine::get()->getShaderManager());

    FrameBuffer* frameEffectMotionBlur = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMEEFFECTMOTIONBLUR);
    FrameBuffer* frameShadow = frameBufferManager->getFrameBuffer(FrameBufferName::DIRECTIONLIGHTSHADOWFRAMEBUFFER);
    FrameBuffer* framePointLightShadow = frameBufferManager->getFrameBuffer(FrameBufferName::POINTLIGHTSSHADOWMAP);

    ID3D11ShaderResourceView* post_effects_views[] =
    {
        frameEffectMotionBlur->getShaderResourceView(0).Get(),
        frameBufferManager->getFrameBuffer(FrameBufferName::MSAABUFFER1)->getShaderResourceView(1).Get(),
        frameShadow->getShaderResourceView(1).Get(),
        framePointLightShadow->getShaderResourceView(1).Get(),
    };


    FrameBuffer* frameTemp2 = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMEEFFECTSUPPORT1);
    frameBufferManager->ClearFramebuffer(immediateContext, frameTemp2);
    frameBufferManager->Activate(immediateContext, frameTemp2);
    shaderManager->BeginShadowApplyPostEffect(immediateContext);
    frameBufferManager->getFullscreenQuad()->blit(immediateContext, post_effects_views, 0, _countof(post_effects_views));
    shaderManager->End(immediateContext);
    frameBufferManager->Deactivate(immediateContext, frameTemp2);


    FrameBuffer* frameEffectVolumentricLight = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMEVOLUMENTRICLIGHTSCATTREING);
    FrameBuffer* frameTemp = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMEDUMMYSUPPORT);


    frameBufferManager->ClearFramebuffer(immediateContext, frameTemp);
    frameBufferManager->Activate(immediateContext, frameTemp);
    shaderManager->BeginBlitAddColorFromTwoFramebufferFullScreenQuad(immediateContext);
    frameBufferManager->BlitAddFrom2Framebuffer(immediateContext, frameTemp2, frameEffectVolumentricLight, false);
    shaderManager->End(immediateContext);
    frameBufferManager->Deactivate(immediateContext, frameTemp);


    FrameBuffer* frameBloom = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMEEFFECTBLOOM);
    shaderManager->BeginBlitAddColorFromTwoFramebufferFullScreenQuad(immediateContext);
    frameBufferManager->BlitAddFrom2Framebuffer(immediateContext, frameTemp, frameBloom,false);
    shaderManager->End(immediateContext);
   


    

}

