#include "FrameBufferManager.h"
#include "FrameDebug.h"
#include "FrameMainBlit.h"
#include "FrameMSAA.h"
#include "FrameEffect.h"
#include "FrameSprite2D.h"
#include "FrameTempSupport.h"
#include "FrameSample.h"
#ifdef USE_IMGUI
#include "UsesImGui_h.h"
#endif // USE_IMGUI

void FrameBufferManager::Activate(ID3D11DeviceContext* immediateContext, FrameBuffer* framebuffer)
{
    viewportCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;

    immediateContext->RSGetViewports(&viewportCount, cachedViewports);
    immediateContext->OMGetRenderTargets(1, cachedRenderTargetView.ReleaseAndGetAddressOf(), cachedDepthStencilView.ReleaseAndGetAddressOf());
    cachedViewportsStack.push(cachedViewports);
    cachedRenderTargetViewStack.push(cachedRenderTargetView);
    cachedDepthStencilViewStack.push(cachedDepthStencilView);

    immediateContext->RSSetViewports(1, &framebuffer->getViewPort());
    immediateContext->OMSetRenderTargets(1, framebuffer->getRenderTargetView().GetAddressOf(), framebuffer->getDepthStencilView());

}

void FrameBufferManager::Deactivate(ID3D11DeviceContext* immediateContext, FrameBuffer* framebuffer)
{
    D3D11_VIEWPORT* cachedViewportsPtr = cachedViewportsStack.top();
    for (int i = 0; i < D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE; i++) 
        cachedViewports[i] = cachedViewportsPtr[i];
    
    cachedRenderTargetView = cachedRenderTargetViewStack.top();
    cachedDepthStencilView = cachedDepthStencilViewStack.top();

    immediateContext->RSSetViewports(viewportCount, cachedViewports);
    immediateContext->OMSetRenderTargets(1, cachedRenderTargetView.GetAddressOf(), cachedDepthStencilView.Get());

    cachedViewportsStack.pop();
    cachedRenderTargetViewStack.pop();
    cachedDepthStencilViewStack.pop();
}

void FrameBufferManager::Resolve(ID3D11Device* device, ID3D11DeviceContext* immediateContext, FrameBuffer* framebufferSource, FrameBuffer* framebufferReception)
{
    Microsoft::WRL::ComPtr<ID3D11Resource> resouce[2];
    framebufferSource->getShaderResourceView(0)->GetResource(resouce[0].GetAddressOf());
    framebufferReception->getShaderResourceView(0)->GetResource(resouce[1].GetAddressOf());

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
    HRESULT hr = resouce[0].Get()->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
    D3D11_TEXTURE2D_DESC texDesc{};
    texture2d.Get()->GetDesc(&texDesc);
    texDesc.SampleDesc.Count = 1;
    hr = device->CreateTexture2D(&texDesc, 0, texture2d.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
    framebufferReception->getShaderResourceView(0)->GetDesc(&shaderResourceViewDesc);
    hr = device->CreateShaderResourceView(texture2d.Get(), &shaderResourceViewDesc,
        shaderResourceView.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    hr = resouce[0].Get()->QueryInterface<ID3D11Texture2D>(texture2d.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    D3D11_TEXTURE2D_DESC texture2d_desc;
    texture2d->GetDesc(&texture2d_desc);

    _ASSERT_EXPR(texture2d_desc.SampleDesc.Count > 1, L"source texture must be multisample texture");

    

    if (texture2d_desc.SampleDesc.Count > 1)
    {
        Microsoft::WRL::ComPtr<ID3D11Resource> res;
        framebufferReception->getRenderTargetView()->GetResource(res.GetAddressOf());
        immediateContext->ResolveSubresource(res.Get(), D3D11CalcSubresource(0, 0, 1), resouce[0].Get(), D3D11CalcSubresource(0, 0, 1), shaderResourceViewDesc.Format);
    }

}

void FrameBufferManager::DrawToDebug()
{
#ifdef USE_IMGUI
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
    for (auto& frameBuffer : listFrameBuffer)
    {
        frameBuffer.get()->DrawToDebug();
    }
    ImGui::PopStyleVar();
#endif // USE_IMGUI
}

void FrameBufferManager::Init(IDXGISwapChain* swapChain, ID3D11DeviceContext* immediateContext, ID3D11Device* device, const D3D11_VIEWPORT& viewPort)
{
    for (auto& buffer : listFrameBuffer)
    {
        buffer.reset();
    }
    listFrameBuffer.clear();




    listFrameBuffer.push_back(std::make_unique<Framebuffer2DEarly>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::FRAMEBUFFER2DEARLY);

    listFrameBuffer.push_back(std::make_unique<FramebufferMSAA1>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, true, FrameBufferName::MSAABUFFER1);


    //listFrameBuffer.push_back(std::make_unique<FramebufferMSAA2>());
    //listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::MSAABUFFER2);
    //
    //
    //listFrameBuffer.push_back(std::make_unique<FramebufferMSAA3>());
    //listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::MSAABUFFER3);


    listFrameBuffer.push_back(std::make_unique<FramebufferGeometricDebug>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::FRAMEGEOMETRICDEBUGER);


    listFrameBuffer.push_back(std::make_unique<FramebufferSkinnedMeshDebug>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::FRAMESKINNEDMESHDEBUG);


    listFrameBuffer.push_back(std::make_unique<Framebuffer2DLate>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::FRAMEBUFFER2DLATE);


    //listFrameBuffer.push_back(std::make_unique<FrameEffectSoftShadowMap>());
    //listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::SHADOWFRAMEBUFFER, true);


    listFrameBuffer.push_back(std::make_unique<FrameSample>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::FRAMESAMPLE);


    listFrameBuffer.push_back(std::make_unique<FrameEffectMotionBlur>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::FRAMEEFFECTMOTIONBLUR);


    listFrameBuffer.push_back(std::make_unique<FrameEffectVolumetricLightScattering>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::FRAMEVOLUMENTRICLIGHTSCATTREING);


    listFrameBuffer.push_back(std::make_unique<FrameEffectPSShadowMap>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::DIRECTIONLIGHTSHADOWFRAMEBUFFER);
    

    listFrameBuffer.push_back(std::make_unique<FrameEffectPointLightShadowMap>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::POINTLIGHTSSHADOWMAP);


    listFrameBuffer.push_back(std::make_unique<FrameEffectBlur>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::BLURFRAMEBUFFER);


    listFrameBuffer.push_back(std::make_unique<FrameSceneDebug>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::FRAMESCENEDEBUG);


    listFrameBuffer.push_back(std::make_unique<FrameEffectBloom>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::FRAMEEFFECTBLOOM);


    listFrameBuffer.push_back(std::make_unique<FramebufferMainBlit>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::MAINBLITFRAMEBUFFER);


    listFrameBuffer.push_back(std::make_unique<FrameMSAATempSupport>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::MSAATEMPFRAMEBUFFER);


    listFrameBuffer.push_back(std::make_unique<FrameEffectSupport>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::FRAMEEFFECTSUPPORT1);


    listFrameBuffer.push_back(std::make_unique<FrameEffectSupport>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::FRAMEEFFECTSUPPORT2);


    listFrameBuffer.push_back(std::make_unique<FrameEffectSupport>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::FRAMEEFFECTSUPPORT3);


    listFrameBuffer.push_back(std::make_unique<FrameEffectSupport>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::FRAMEEFFECTSUPPORT4);


    listFrameBuffer.push_back(std::make_unique<FrameEffectSupport>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::FRAMEEFFECTSUPPORT5);


    listFrameBuffer.push_back(std::make_unique<FrameDummySupport>());
    listFrameBuffer.back().get()->Setup(swapChain, device, viewPort, false, FrameBufferName::FRAMEDUMMYSUPPORT);

    blitTransfer.reset();
    blitTransfer = std::make_unique<FullscreenQuad>(device);

}


void FrameBufferManager::ClearAllBuffer(ID3D11DeviceContext* immediateContext)
{
    for (std::shared_ptr<FrameBuffer>& it : listFrameBuffer)
    {
        it.get()->ClearRenderTarget(immediateContext);
        it.get()->ClearDepthStencil(immediateContext);
    }
}

void FrameBufferManager::ClearFramebuffer(ID3D11DeviceContext* immediateContext, FrameBuffer* framebuffer)
{
    framebuffer->ClearRenderTarget(immediateContext);
    framebuffer->ClearDepthStencil(immediateContext);
}

void FrameBufferManager::DrawOnFrameBuffer(ID3D11DeviceContext* immediateContext)
{
    for (size_t i = 0; i < listFrameBuffer.size(); i++)
    {
       
        listFrameBuffer[i].get()->DrawedOn(immediateContext);
        
    }
}

void FrameBufferManager::ResolveMSAAFrameBufferToTempBuffer(ID3D11Device* device,ID3D11DeviceContext* immediateContext, FrameBuffer* MSAAframeBuffer)
{
    FrameBuffer* frameBuffer = getFrameBuffer(FrameBufferName::MAINBLITFRAMEBUFFER );
    Resolve(device, immediateContext, MSAAframeBuffer, frameBuffer);
}

void FrameBufferManager::BlitMainFrameBufferToScreen(ID3D11DeviceContext* immediateContext, ShaderManager* shaderManager)
{
    FrameBuffer* frameBuffer = getFrameBuffer(FrameBufferName::MAINBLITFRAMEBUFFER);
    shaderManager->BeginBlitFullScreenQuad(immediateContext);
    BlitFrom(immediateContext, frameBuffer);
    shaderManager->End(immediateContext);
}

void FrameBufferManager::BlitFrom(ID3D11DeviceContext* immediateContext, FrameBuffer* frameBuffer, int slot)
{
    blitTransfer.get()->blit(immediateContext, frameBuffer->getShaderResourceView(slot).GetAddressOf(), 0, 1);
}

void FrameBufferManager::BlitAddFrom2Framebuffer(ID3D11DeviceContext* immediateContext, FrameBuffer* frameBuffer1, FrameBuffer* frameBuffer2, bool depthStencil)
{
    depthStencil?
        blitTransfer.get()->blit2ShaderResourceView2DepthStencilView(immediateContext, frameBuffer1->getShaderResourceView(0).GetAddressOf(), frameBuffer2->getShaderResourceView(0).GetAddressOf(), frameBuffer1->getShaderResourceView(1).GetAddressOf(), frameBuffer2->getShaderResourceView(1).GetAddressOf(), 0, 1) :
    blitTransfer.get()->blit2ShaderResourceView(immediateContext, frameBuffer1->getShaderResourceView(0).GetAddressOf(), frameBuffer2->getShaderResourceView(0).GetAddressOf(), 0, 1);
}

void FrameBufferManager::BlitAddFrom2Framebuffer(ID3D11DeviceContext* immediateContext, FrameBuffer* frameBuffer1, FrameBuffer* frameBuffer2, int slot1, int slot2)
{
    blitTransfer->blit2ShaderResourceView(immediateContext, frameBuffer1->getShaderResourceView(slot1).GetAddressOf(),
        frameBuffer2->getShaderResourceView(slot2).GetAddressOf(), 0, 0);
}

void FrameBufferManager::BlitFromNumFrameBuffer(ID3D11DeviceContext* immediateContext, std::shared_ptr<FrameBuffer>* listFrame, uint32_t num)
{

    std::vector< ID3D11ShaderResourceView*> data;
    for (size_t i = 0; i < num; i++)
        data.push_back(listFrame[i]->getShaderResourceView(0).Get());

    blitTransfer->BlitFromNumResourceView(immediateContext, data.data(), 0, num);
}

D3D11_VIEWPORT& FrameBufferManager::getViewPort(FrameBufferName name)
{
    return getFrameBuffer(name)->getViewPort();
}

FrameBuffer* FrameBufferManager::getFrameBuffer(FrameBufferName name)
{
    FrameBuffer* frameBuffer = nullptr;
    for (std::shared_ptr<FrameBuffer>& it : listFrameBuffer)
        if (name == it.get()->getFrameBufferName())
        {
            frameBuffer = it.get();
            break;
        }
    return frameBuffer;
}

UINT FrameBufferManager::getDirectionLightShadowCascadeCount()
{
    return dynamic_cast<FrameEffectPSShadowMap*>(getFrameBuffer(FrameBufferName::DIRECTIONLIGHTSHADOWFRAMEBUFFER))->getCascadeCount();
}

UINT FrameBufferManager::getPointLightShadowMapSlide()
{
    return dynamic_cast<FrameEffectPointLightShadowMap*>(getFrameBuffer(FrameBufferName::POINTLIGHTSSHADOWMAP))->getSlideCount();
}


FrameBufferManager::~FrameBufferManager()
{
    for (std::shared_ptr<FrameBuffer>& it : listFrameBuffer)
    {
        it.reset();
    }
}
