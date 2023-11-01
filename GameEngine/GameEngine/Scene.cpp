#include "Scene.h"
#include "GameEngine.h"
#include <fstream>
Scene::Scene()
{
    state = 0;
    timer = 0;
    actorManager = std::make_unique<ActorManager>();
    lightManager.reset(new LightManager);
    skymap = std::make_unique<Skymap>();
    saveLocal = "";
}

void Scene::Reset()
{
}

void Scene::Init()
{
    actorManager->Init();
    lightManager->Init();
    skymap->Init(GameEngine::get()->getDevice());
    state = 0;
    timer = 0;
}

void Scene::Update(float elapsedTime)
{
    actorManager->Update(elapsedTime);
    lightManager->Update();
    timer++;
}

void Scene::Release()
{
    actorManager->resetObject();
}

void Scene::CreateNewCerealFile()
{
    std::filesystem::path cerealFileName(saveLocal.c_str());
    cerealFileName.replace_extension("sce");
    std::ofstream ofs(cerealFileName.c_str(), std::ios::binary);
    cereal::BinaryOutputArchive serialization(ofs);
    serialization(saveLocal, actorManager, lightManager, skymap);
}

void Scene::LoadCerealFile(std::filesystem::path local)
{
    std::ifstream ifs(local.string().c_str(), std::ios::binary);
    if (ifs.fail())
        return;
    cereal::BinaryInputArchive deserialization(ifs);
    deserialization(saveLocal, actorManager, lightManager, skymap);
}

void Scene::Draw()
{
    actorManager->Draw();
}

Scene::~Scene()
{
    actorManager->resetObject();
    actorManager.reset();
}


