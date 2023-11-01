#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <map>
#include <iostream>
#include "ActorManager.h"
#include "LightManager.h"
#include "Skymap.h"

class Scene
{
public:
    Scene();
    void Reset();
    void Init();
    void Update(float elapsedTime);
    void Draw();
    void Release();
    void CreateNewCerealFile();
    void LoadCerealFile(std::filesystem::path local);
    ~Scene();

public:
    template < class T>
    void serialize(T& archive)
    {
        archive(saveLocal, actorManager, lightManager, skymap);
    }

public:
    ActorManager* getActorManager() { return actorManager.get(); }
    LightManager* getLightManager() { return lightManager.get(); }
    Skymap* getSkymap() { return skymap.get(); }
    std::string& getSaveLocal() { return saveLocal; }
    void setSaveLocal(std::string path) { saveLocal = path; }
private:
    std::string saveLocal;
    std::unique_ptr<ActorManager> actorManager;
    std::unique_ptr<LightManager> lightManager;
    std::unique_ptr<Skymap> skymap;
    int timer;
    int state;
};





#endif // !SCENE_H

