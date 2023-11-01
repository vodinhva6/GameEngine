#include "EffectSeerManager.h"

Effect::Effect(const char* filename)
{
    char16_t utfFilename[256];
    Effekseer::ConvertUtf8ToUtf16(utfFilename, 256, filename);

    Effekseer::ManagerRef effManger = EffectSeerManager::get()->getEffManager();
    Effect_ = Effekseer::Effect::Create(effManger, (EFK_CHAR*)utfFilename);
}

Effekseer::Handle Effect::Play(const VECTOR3 position, float scale, const VECTOR3 axis, const float angle)
{
    Effekseer::ManagerRef  effManager = EffectSeerManager::get()->getEffManager();

    handle_ = effManager->Play(Effect_, position.x, position.y, position.z);
    effManager->SetScale(handle_, scale, scale, scale);
    setRotation(handle_, axis, angle);
    effManager->SetSpeed(handle_, 1.0f);
    return handle_;
}


void Effect::Stop()
{
    Effekseer::ManagerRef effManager = EffectSeerManager::get()->getEffManager();
    //effManager->StopAllEffects();
    effManager->StopEffect(handle_);
}

void Effect::setGameCameraPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
    Effekseer::ManagerRef  effManager = EffectSeerManager::get()->getEffManager();
    effManager->SetLocation(handle, position.x, position.y, position.z);
}

void Effect::setScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
    Effekseer::ManagerRef  effManager = EffectSeerManager::get()->getEffManager();
    effManager->SetScale(handle, scale.x, scale.y, scale.z);
}

void Effect::setRotation(Effekseer::Handle handle, const VECTOR3 axis, const float angle)
{
    Effekseer::ManagerRef  effManager = EffectSeerManager::get()->getEffManager();
    effManager->SetRotation(handle, { axis.x,axis.y,axis.z }, DirectX::XMConvertToRadians(angle));
}

