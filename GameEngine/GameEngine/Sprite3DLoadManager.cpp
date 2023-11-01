#include "Sprite3DLoadManager.h"
#include "GameEngine.h"
bool Sprite3DLoadManager::CreateSprite3D(std::filesystem::path local, std::shared_ptr<Sprite3D>& pSprite3D)
{
    if(CheckHadSprite3D(local,pSprite3D))
        return true;
    pSprite3D.reset(new Sprite3D(GameEngine::get()->getDevice(), local));

    sprites3DResource.insert(std::make_pair(local, pSprite3D));
    return true;
}

bool Sprite3DLoadManager::CheckHadSprite3D(std::filesystem::path local, std::shared_ptr<Sprite3D>& pSprite3D)
{
    auto it = sprites3DResource.find(local);
    if (it != sprites3DResource.end())
    {
        pSprite3D = it->second;
        return true;
    }
    return false;
}
