#pragma once
#include "FrameBuffer.h"
#include "SkinnedMesh.h"
#include "Camera.h"
class FrameThumbnail : public FrameBuffer
{
public:
    FrameThumbnail(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow = false);
    void Draw(ID3D11DeviceContext* immediateContext, SkinnedMesh* mesh, std::unordered_map<int64_t, std::shared_ptr<Material>>& dummyMaterial);
    void ClearRenderTarget(ID3D11DeviceContext* immediateContext, float r = 0, float g = 0, float b = 0, float a = 0) override;
private:
    std::unique_ptr<Camera> camera;
    struct ThumbnailSceneConstant
    {
        DirectX::XMFLOAT4X4 viewProjectionThumbnail;
    };
    ThumbnailSceneConstant thumbnailSceneConstant;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
    int PSSlotNum;
    int VSSlotNum;
    void SettingCamera(SkinnedMesh* mesh);
};