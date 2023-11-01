#pragma once
#include "Sprite3D.h"
#include <memory>
#include <map>
class Sprite3DLoadManager
{
public:
    bool CreateSprite3D(std::filesystem::path local, std::shared_ptr<Sprite3D>& pSprite3D);


private:
    bool CheckHadSprite3D(std::filesystem::path local, std::shared_ptr<Sprite3D>& pSprite3D);
private:
    std::map<std::filesystem::path, std::shared_ptr<Sprite3D> > sprites3DResource;
};

