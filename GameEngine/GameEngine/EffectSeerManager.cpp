#include "EffectSeerManager.h"

void EffectSeerManager::Init(ID3D11Device* device, ID3D11DeviceContext* immedia)
{
    EffRender = EffekseerRendererDX11::Renderer::Create(device, immedia, 2048);
    EffManager = Effekseer::Manager::Create(2048);

    EffManager->SetSpriteRenderer(EffRender->CreateSpriteRenderer());
    EffManager->SetRibbonRenderer(EffRender->CreateRibbonRenderer());
    EffManager->SetRingRenderer(EffRender->CreateRingRenderer());
    EffManager->SetTrackRenderer(EffRender->CreateTrackRenderer());
    EffManager->SetModelRenderer(EffRender->CreateModelRenderer());


    EffManager->SetTextureLoader(EffRender->CreateTextureLoader());
    EffManager->SetModelLoader(EffRender->CreateModelLoader());
    EffManager->SetMaterialLoader(EffRender->CreateMaterialLoader());


    EffManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

void EffectSeerManager::Deinit()
{
    EffRender.Reset();
    EffManager.Reset();
}

void EffectSeerManager::Update(float elapsedTime, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    EffManager->Update(elapsedTime * 60.0f);
    EffRender->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
    EffRender->SetProjectionMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&projection));
    timer++;
}

void EffectSeerManager::addToDrawQuene(Effect* eff)
{
    listEffect.push_back(eff);
}

void EffectSeerManager::stopAll()
{
    EffManager->StopAllEffects();
}

void EffectSeerManager::Render()
{
    EffRender->BeginRendering();
    EffManager->Draw();
    EffRender->EndRendering();
}

void EffectSeerManager::RenderHandle(Effekseer::Handle handle)
{
    EffRender->BeginRendering();
    EffManager->DrawHandle(handle);
    EffRender->EndRendering();
}
