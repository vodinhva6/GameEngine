#include "FrameEffect.h"
#include "FrameBufferManager.h"
#include "GameEngine.h"
#include "ShaderManager.h"
#ifdef USE_IMGUI
#include "UsesImGui_h.h"
#endif // USE_IMGUI


void FrameEffectBloom::Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow, bool depth)
{
    FrameBuffer::Setup(swapChain, device, viewPort, msaa, name, shadow);
    for (size_t downsampled_index = 0; downsampled_index < downSamplingCount; ++downsampled_index)
    {
        D3D11_VIEWPORT supViewPort = viewPort;
        supViewPort.Width  = (FLOAT)(((uint32_t)supViewPort.Width ) >> downsampled_index);
        supViewPort.Height = (FLOAT)(((uint32_t)supViewPort.Height) >> downsampled_index);
        downAndBlurSampleFrame[downsampled_index] = std::make_shared<FrameBuffer>();
        downAndBlurSampleFrame[downsampled_index]->Setup(swapChain, device, supViewPort, msaa, FrameBufferName::NON, false, false);
        downAndBlurSampleFrameSupport[downsampled_index] = std::make_unique<FrameBuffer>();
        downAndBlurSampleFrameSupport[downsampled_index]->Setup(swapChain, device, supViewPort, msaa, FrameBufferName::NON, false, false);
    }

}

void FrameEffectBloom::DrawedOn(ID3D11DeviceContext* immediateContext)
{
    FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
    ShaderManager* shaderManager = GetFrom< ShaderManager>(GameEngine::get()->getShaderManager());

    frameBufferManager->ClearFramebuffer(immediateContext, this);
    frameBufferManager->Activate(immediateContext, this);

    FrameBuffer* frameSample = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMESAMPLE);
    FrameBuffer* frameExtractionColor = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMEDUMMYSUPPORT);
    FrameBuffer* frameSupport = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMEEFFECTSUPPORT1);
    frameBufferManager->ClearFramebuffer(immediateContext, frameExtractionColor);
    for (uint32_t samplerIndex = 0; samplerIndex < downSamplingCount; ++samplerIndex)
    {
        frameBufferManager->ClearFramebuffer(immediateContext, downAndBlurSampleFrame[samplerIndex].get());
        frameBufferManager->ClearFramebuffer(immediateContext, downAndBlurSampleFrameSupport[samplerIndex].get());
    }


    frameBufferManager->ClearFramebuffer(immediateContext, frameExtractionColor);
    frameBufferManager->Activate(immediateContext, frameExtractionColor);
    shaderManager->BeginGlowExtraction(immediateContext);
    frameBufferManager->BlitFrom(immediateContext, frameSample);
    shaderManager->End(immediateContext);
    frameBufferManager->Deactivate(immediateContext, frameExtractionColor);

    frameBufferManager->ClearFramebuffer(immediateContext, frameSupport);
    frameBufferManager->Activate(immediateContext, downAndBlurSampleFrame[0].get());
    shaderManager->BeginDownSampling(immediateContext);
    frameBufferManager->BlitFrom(immediateContext, frameExtractionColor);
    shaderManager->End(immediateContext);
    frameBufferManager->Deactivate(immediateContext, downAndBlurSampleFrame[0].get());

    frameBufferManager->ClearFramebuffer(immediateContext, downAndBlurSampleFrameSupport[0].get());
    frameBufferManager->Activate(immediateContext, downAndBlurSampleFrameSupport[0].get());
    shaderManager->BeginGaussianBlurHorizontal(immediateContext);
    frameBufferManager->BlitFrom(immediateContext, downAndBlurSampleFrame[0].get());
    shaderManager->End(immediateContext);
    frameBufferManager->Deactivate(immediateContext, downAndBlurSampleFrameSupport[0].get());

    frameBufferManager->ClearFramebuffer(immediateContext, downAndBlurSampleFrame[0].get());
    frameBufferManager->Activate(immediateContext, downAndBlurSampleFrame[0].get());
    shaderManager->BeginGaussianBlurVertical(immediateContext);
    frameBufferManager->BlitFrom(immediateContext, downAndBlurSampleFrameSupport[0].get());
    shaderManager->End(immediateContext);
    frameBufferManager->Deactivate(immediateContext, downAndBlurSampleFrame[0].get());




    for (uint32_t samplerIndex = 1; samplerIndex < downSamplingCount; ++samplerIndex)
    {
        frameBufferManager->ClearFramebuffer(immediateContext, downAndBlurSampleFrame[samplerIndex].get());
        frameBufferManager->Activate(immediateContext, downAndBlurSampleFrame[samplerIndex].get());
        shaderManager->BeginDownSampling(immediateContext);
        frameBufferManager->BlitFrom(immediateContext, downAndBlurSampleFrame[samplerIndex - 1].get());
        shaderManager->End(immediateContext);
        frameBufferManager->Deactivate(immediateContext, downAndBlurSampleFrame[samplerIndex].get());
        
        frameBufferManager->ClearFramebuffer(immediateContext, downAndBlurSampleFrameSupport[samplerIndex].get());
        frameBufferManager->Activate(immediateContext, downAndBlurSampleFrameSupport[samplerIndex].get());
        shaderManager->BeginGaussianBlurHorizontal(immediateContext);
        frameBufferManager->BlitFrom(immediateContext, downAndBlurSampleFrame[samplerIndex].get());
        shaderManager->End(immediateContext);
        frameBufferManager->Deactivate(immediateContext, downAndBlurSampleFrameSupport[samplerIndex].get());
        
        frameBufferManager->ClearFramebuffer(immediateContext, downAndBlurSampleFrame[samplerIndex].get());
        frameBufferManager->Activate(immediateContext, downAndBlurSampleFrame[samplerIndex].get());
        shaderManager->BeginGaussianBlurVertical(immediateContext);
        frameBufferManager->BlitFrom(immediateContext, downAndBlurSampleFrameSupport[samplerIndex].get());
        shaderManager->End(immediateContext);
        frameBufferManager->Deactivate(immediateContext, downAndBlurSampleFrame[samplerIndex].get());
    }

    shaderManager->BeginUpSampling(immediateContext);
    frameBufferManager->BlitFromNumFrameBuffer(immediateContext, downAndBlurSampleFrame, downSamplingCount);
    shaderManager->End(immediateContext);


    frameBufferManager->Deactivate(immediateContext, this);
}

void FrameEffectBloom::DrawToDebug()
{
    
}

FrameEffectBloom::~FrameEffectBloom()
{

}
