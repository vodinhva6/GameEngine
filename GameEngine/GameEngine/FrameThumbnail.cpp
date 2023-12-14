#include "FrameThumbnail.h"
#include "FrameBufferManager.h"
#include "MaterialManager.h"
#include "GameEngine.h"
FrameThumbnail::FrameThumbnail(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow)
{
    Setup(swapChain, device, viewPort, msaa, name, shadow);
    camera = std::make_unique<Camera>();
    camera->getPerspectiveInforNow().aspectRatio = viewPort.Width / viewPort.Height;
    camera->Init();
    HRESULT hr{ S_OK };
    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;
    buffer_desc.ByteWidth = sizeof(ThumbnailSceneConstant);
    hr = device->CreateBuffer(&buffer_desc, nullptr, constantBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    
   
    PSSlotNum = 1;
    VSSlotNum = 1;
}

void FrameThumbnail::Draw(ID3D11DeviceContext* immediateContext, SkinnedMesh* mesh, std::unordered_map<int64_t, std::shared_ptr<Material>>& dummyMaterial)
{
    ClearDepthStencil(immediateContext);
    ClearRenderTarget(immediateContext);
    FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
    ShaderManager* shaderManager = GetFrom<ShaderManager>(GameEngine::get()->getShaderManager());
    MaterialManager* materialManager = GetFrom<MaterialManager>(GameEngine::get()->getMaterialManager());

    frameBufferManager->Activate(immediateContext, this);
    shaderManager->BeginSkinnedMeshDrawDebugThumbnail(immediateContext);
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, DirectX::XMMatrixIdentity());
    Microsoft::WRL::ComPtr<ID3D11Buffer> tempConstantBuffer;
    immediateContext->VSGetConstantBuffers(VSSlotNum, 1, tempConstantBuffer.GetAddressOf());
    immediateContext->VSSetConstantBuffers(VSSlotNum, 1, constantBuffer.GetAddressOf());
    SettingCamera(mesh);
    immediateContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &thumbnailSceneConstant, 0, 0);
    if(dummyMaterial.empty())
    dummyMaterial.insert(std::make_pair(materialManager->getDummyMaterial()->getUniqueId(), materialManager->getDummyMaterial()));
    mesh->Render(immediateContext, transform, dummyMaterial, { 1,1,1,1 });
    immediateContext->VSSetConstantBuffers(VSSlotNum, 1, tempConstantBuffer.GetAddressOf());
    frameBufferManager->Deactivate(immediateContext, this);
}

void FrameThumbnail::ClearRenderTarget(ID3D11DeviceContext* immediateContext, float r, float g, float b, float a)
{
    float color[4]{ 0.5f,0.5f,0.5f,0.5f };
    immediateContext->ClearRenderTargetView(renderTargetView.Get(), color);
}

void FrameThumbnail::SettingCamera(SkinnedMesh* mesh)
{
    float angle = camera.get()->getPerspectiveInforNow().angleFovY;
    VECTOR3 vertexMin = mesh->getVertexMin();
    VECTOR3 vertexMax = mesh->getVertexMax();

    float height = (fabsf(vertexMax.y) + fabsf(vertexMin.y));
    float width = (fabsf(vertexMax.x) + fabsf(vertexMin.x));
    VECTOR3 pivot = { (vertexMax.x + vertexMin.x) / 2 ,
        (vertexMax.y + vertexMin.y) / 2,
        (vertexMax.z + vertexMin.z) / 2 };
    float length;
    if (height > width)
        length = height;
    else
        length = width;
    float hypotenuse = length / sinf(MyMath::get()->convertToRadian(angle / 2));
    float dis = cosf(MyMath::get()->convertToRadian(angle / 2)) * hypotenuse;
    pivot.z += dis / 2 + dis * 0.1f;
    float timer = 20;
    camera.get()->setCameraPos(pivot);
    camera.get()->setCameraTarget({ pivot.x,pivot.y,pivot.z -0.01f  });
    camera.get()->Update(GameEngine::get()->getElapsedTime());
    DirectX::XMStoreFloat4x4(&thumbnailSceneConstant.viewProjectionThumbnail, camera.get()->GetSceneView3DNow());

}
