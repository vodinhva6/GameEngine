#pragma once
#include "SingletonPatternTemplate.h"
#include "EffekseerRendererDX11.h"
#include "DataFormat.h"
#include <wrl.h>
class Effect;
class EffectSeerManager:public SingletonPatternTemplate<EffectSeerManager>
{
    friend class SingletonPatternTemplate<EffectSeerManager>;
public:
    void Init(ID3D11Device*, ID3D11DeviceContext*);

    void Deinit();

    void Update(float elapsedTime, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

    void addToDrawQuene(Effect* eff);
    void stopAll();
    void Render();
    void RenderHandle(Effekseer::Handle);
    Effekseer::ManagerRef getEffManager()
    {
        return EffManager;
    }

private:
    Effekseer::ManagerRef EffManager;
    EffekseerRenderer::RendererRef EffRender;
    std::list<Effect*> listEffect;
    int timer;
};





class Effect
{
public:
    Effect(const char* filename);
    ~Effect() {};
    Effekseer::Handle Play(const VECTOR3 position, float scale = 1.0f, const VECTOR3 axis = { 0,0,0 }, const float angle = 0.0f);

    void Stop();

    void setGameCameraPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position);

    void setScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale);

private:
    Effekseer::EffectRef Effect_;
    Effekseer::Handle handle_;
    void setRotation(Effekseer::Handle handle, const VECTOR3 axis, const float angle);

};

