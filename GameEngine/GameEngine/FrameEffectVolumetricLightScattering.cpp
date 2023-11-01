#include "FrameEffect.h"
#include "GameEngine.h"
#include "FrameBufferManager.h"
#include "LightManager.h"
#include "ShaderManager.h"
#ifdef USE_IMGUI
#include "UsesImGui_h.h"
#endif // USE_IMGUI
#include "GltfModel.h"
#include "Log.h"


void FrameEffectVolumetricLightScattering::Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow, bool depth)
{
    FrameBuffer::Setup(swapChain, device, viewPort, msaa, name, shadow, depth);
    if (constantBuffer.Get())
        return;
    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(ConstantBuffer);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    HRESULT hr = device->CreateBuffer(&buffer_desc, nullptr, constantBuffer.ReleaseAndGetAddressOf());
#ifdef USE_IMGUI
    SUCCEEDEDRESULT(hr);
#endif // USE_IMGUI
}

void FrameEffectVolumetricLightScattering::DrawedOn(ID3D11DeviceContext* immediateContext)
{
    FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
    ShaderManager* shaderManager = GetFrom< ShaderManager>(GameEngine::get()->getShaderManager());

    FrameBuffer* frameEffectDummy1 = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMEEFFECTSUPPORT1);
    FrameBuffer* frameEffectDummy2 = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMEEFFECTSUPPORT2);
    FrameBuffer* frameEffectDummy3 = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMEEFFECTSUPPORT3);
    FrameBuffer* frameEffectDummy4 = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMEEFFECTSUPPORT4);
    FrameBuffer* frameEffectDummy5 = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMEEFFECTSUPPORT5);

    FrameBuffer* frameBufferDummy = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMEDUMMYSUPPORT);

    frameBufferManager->Activate(immediateContext, frameBufferDummy);
  
    shaderManager->BeginMeshWithBlackNONTexture(immediateContext);
    GameEngine::get()->DrawAllUninvisibleObject(FrameBufferName::MSAABUFFER1, DrawType::SkinnedMeshObject);
    shaderManager->End(immediateContext);

    frameBufferManager->Deactivate(immediateContext, frameBufferDummy);


    
 
    LightManager* liMager = GetFrom<LightManager>(GameEngine::get()->getLightManager());
    size_t n = liMager->getSizePointLightList();
    for (size_t i = 0; i < n; i++)
    {
        FrameBuffer* frameSaveNow = frameEffectDummy1;
        FrameBuffer* frameSavePrevious = frameEffectDummy2;
        FrameBuffer* frameSotSave = frameEffectDummy3;
        FrameBuffer* framePointLight = frameEffectDummy4;
        FrameBuffer* framesample = frameEffectDummy5;
        if (i % 2 != 0)
        {
            frameSaveNow = frameEffectDummy2;
            frameSavePrevious = frameEffectDummy1;
        }
        frameBufferManager->ClearFramebuffer(immediateContext, frameSaveNow);
        frameBufferManager->ClearFramebuffer(immediateContext, frameSotSave);
        frameBufferManager->ClearFramebuffer(immediateContext, framePointLight);
        frameBufferManager->ClearFramebuffer(immediateContext, framesample);


        frameBufferManager->Activate(immediateContext, framePointLight);
        shaderManager->BeginMeshLightWithEmission(immediateContext);
        liMager->getPointLightAt(i)->DrawSphere(immediateContext);
        shaderManager->End(immediateContext);
        frameBufferManager->Deactivate(immediateContext, framePointLight);

        frameBufferManager->Activate(immediateContext, framesample);
        shaderManager->BeginBlitAddColorVolumetricLightScatteringFullScreenQuad(immediateContext,true);
        frameBufferManager->BlitAddFrom2Framebuffer(immediateContext, frameBufferDummy, framePointLight, true);
        shaderManager->End(immediateContext);
        frameBufferManager->Deactivate(immediateContext, framesample);


        frameBufferManager->Activate(immediateContext, frameSotSave);
        if (liMager->getPointLightAt(i)->getRenderWithSphere())
        {
            BindConstantBuffer(immediateContext,i);
            shaderManager->BeginBlitFullScreenQuadWithVolumetricLightScattering(immediateContext);
        }
        else  shaderManager->BeginBlitFullScreenQuad(immediateContext);

        frameBufferManager->BlitFrom(immediateContext, framesample);
        shaderManager->End(immediateContext);
        frameBufferManager->Deactivate(immediateContext, frameSotSave);



        frameBufferManager->Activate(immediateContext, frameSaveNow);
        shaderManager->BeginBlitAddColorVolumetricLightScatteringFullScreenQuad(immediateContext);
        frameBufferManager->BlitAddFrom2Framebuffer(immediateContext, frameSavePrevious, frameSotSave, false);
        shaderManager->End(immediateContext);
        frameBufferManager->Deactivate(immediateContext, frameSaveNow);

    }

   
    FrameBuffer* effectDummyLast = frameEffectDummy1;
    if (n % 2 == 0)
        effectDummyLast = frameEffectDummy2;
    frameBufferManager->Activate(immediateContext, this);
    shaderManager->BeginBlitFullScreenQuad(immediateContext);
    frameBufferManager->BlitFrom(immediateContext, effectDummyLast);
    shaderManager->End(immediateContext);
    frameBufferManager->Deactivate(immediateContext, this);
   

   
   
}

void FrameEffectVolumetricLightScattering::DrawToDebug()
{
}

void FrameEffectVolumetricLightScattering::BindConstantBuffer(ID3D11DeviceContext* dc, size_t slot)
{
    data.slot = (int)slot;
    dc->UpdateSubresource(constantBuffer.Get(), 0, 0, &data, 0, 0);
    dc->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
}
