#pragma once
#include "Scene.h"
#include "StageManager.h"
#include "CameraManager.h"
class SceneManager
{
public:
    SceneManager();
    void Init();
    void Update(float elapsedTime);
    void DrawSubscribe();
    ~SceneManager();
public:
    CameraManager* getCameraManager() { return cameraManager.get(); }
    ActorManager* getActorManager() { return sceneNow->getActorManager(); }
    Skymap* getSkymap() { return sceneNow->getSkymap(); }
public:
    Scene* getSceneNow()
    {
        return sceneNow.get();
    }
    Scene* getSceneGame()
    {
        return nullptr;// listScene[SceneName::Game].get();
    }


public:
    void UpdateChangeScene();
    //void setSceneNext(Scene* sc) { sceneNext = sc; }
    //bool changeScene(SceneName sceneName, int delayTimer);
    void saveGame();
    void loadGame();
    bool checkHadFileLoadGame();
    void NewScene();
    void SaveScene(std::filesystem::path local, bool saveAs);
    void OpenScene(std::filesystem::path local);
    bool CheckSceneHadSaveLocal() { return sceneNow->getSaveLocal() != ""; }
private:

    std::unique_ptr<StageManager> stageManager;
    std::unique_ptr<CameraManager>cameraManager;
   

    std::unique_ptr<Scene> sceneNow;
    std::unique_ptr<Scene> sceneNext;
    int timer;
};

