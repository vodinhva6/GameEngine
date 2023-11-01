#pragma once
#include "FrameBuffer.h"
#include "DataFormat.h"
#include "FullscreenQuad.h"
#include <vector>
#include <memory>
#include <stack>
#include "ShaderManager.h"
class FrameBufferManager 
{
public:
    void Init(IDXGISwapChain* swapChain, ID3D11DeviceContext* immediateContext, ID3D11Device* device, const D3D11_VIEWPORT& viewPort);
    void ClearAllBuffer(ID3D11DeviceContext* immediateContext);
    void ClearFramebuffer(ID3D11DeviceContext* immediateContext, FrameBuffer* framebuffer);
    void DrawOnFrameBuffer(ID3D11DeviceContext* immediateContext);
    void ResolveMSAAFrameBufferToTempBuffer(ID3D11Device* device,ID3D11DeviceContext* immediateContext, FrameBuffer* MSAAframeBuffer);
    void BlitMainFrameBufferToScreen(ID3D11DeviceContext* immediateContext, ShaderManager* shaderManager);
    void BlitFrom(ID3D11DeviceContext* immediateContext, FrameBuffer* frameBuffer, int slot = 0);
    void BlitAddFrom2Framebuffer(ID3D11DeviceContext* immediateContext, FrameBuffer* frameBuffer1, FrameBuffer* frameBuffer2, bool depthStencil = false);
    void BlitAddFrom2Framebuffer(ID3D11DeviceContext* immediateContext, FrameBuffer* frameBuffer1, FrameBuffer* frameBuffer2, int slot1 = 0, int slot2 = 0);
    void BlitFromNumFrameBuffer(ID3D11DeviceContext* immediateContext, std::shared_ptr<FrameBuffer>* listFrame, uint32_t num);
    void Activate(ID3D11DeviceContext* immediateContext, FrameBuffer* framebuffer);
    void Deactivate(ID3D11DeviceContext* immediateContext, FrameBuffer* framebuffer);
    void Resolve(ID3D11Device* device, ID3D11DeviceContext* immediateContext, FrameBuffer* framebufferSource, FrameBuffer* framebufferReception);
    void DrawToDebug();

public:

    D3D11_VIEWPORT& getViewPort(FrameBufferName name);
    FrameBuffer* getFrameBuffer(FrameBufferName name);
    size_t getSizeListFrameBuffer() { return listFrameBuffer.size(); }
    UINT getDirectionLightShadowCascadeCount();
    UINT getPointLightShadowMapSlide();
    FullscreenQuad* getFullscreenQuad() { return blitTransfer.get(); }
    ~FrameBufferManager();
private:
    
    std::unique_ptr<FullscreenQuad>          blitTransfer;
    std::vector<std::shared_ptr<FrameBuffer>> listFrameBuffer;

    //std::stack<UINT[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]> viewportCountStack;
    std::stack<D3D11_VIEWPORT*> cachedViewportsStack;
    std::stack<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>> cachedRenderTargetViewStack;
    std::stack<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>> cachedDepthStencilViewStack;


    UINT viewportCount{ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE };
    D3D11_VIEWPORT cachedViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> cachedRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> cachedDepthStencilView;
 
};