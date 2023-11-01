#include "SceneManager.h"
#include "GameEngine.h"
#include "DataBase.h"
#include <fstream>
void SceneManager::saveGame()
{
    std::ofstream myfile;
    myfile.open(L"./Data/Save/savegame.txt");
    //Save Game



    //

    myfile.close();
}

void SceneManager::loadGame()
{

    std::ifstream input;
    input.open(L"./Data/Save/savegame.txt");
    //Load Game




    //
    input.close();

}

bool SceneManager::checkHadFileLoadGame()
{
    std::ifstream input;
    input.open(L"./Data/Save/savegame.txt");

    if (input.fail())
    {
        return false;
    }
    return true;
}

void SceneManager::NewScene()
{
    sceneNow.reset();
    sceneNow = std::make_unique<Scene>();
    sceneNow->Init();
}

void SceneManager::SaveScene(std::filesystem::path local, bool saveAs)
{
    if (saveAs)
    {
        sceneNow->setSaveLocal(local.string());
      
    }

    sceneNow->CreateNewCerealFile();
}

void SceneManager::OpenScene(std::filesystem::path local)
{
    sceneNow.reset();
    sceneNow = std::make_unique<Scene>();
    sceneNow->LoadCerealFile(local);
    sceneNow->Init();
    

}


SceneManager::~SceneManager()
{
   
}


SceneManager::SceneManager()
{
    timer = 0;
    sceneNow = std::make_unique<Scene>();
    stageManager = std::make_unique<StageManager>();
    cameraManager = std::make_unique<CameraManager>();
    sceneNext;
}

void SceneManager::Init()
{
    if (sceneNow.get())
    sceneNow->Init();
    cameraManager.get()->Init();
  
}

void SceneManager::Update(float elapsedTime)
{
    if(sceneNow.get())
    sceneNow->Update(elapsedTime);
    cameraManager.get()->Update(elapsedTime);
  
}

void SceneManager::DrawSubscribe()
{
    if (sceneNow.get())
    sceneNow->Draw();
    cameraManager.get()->Draw();
}

void SceneManager::UpdateChangeScene()
{
    //if (timer <= 0)
    //{
    //    if (sceneNow != sceneNext)
    //    {
    //        sceneNow->Release();
    //        sceneNext->Init();
    //        sceneNow = sceneNext;
    //    }
    //}
    //else timer--;
}

//bool SceneManager::changeScene(SceneName sceneName, int delayTimer)
//{
//    //if (sceneNext == listScene[sceneName].get())
//    //    return false;
//    //timer = delayTimer;
//    //sceneNext = listScene[sceneName].get();
//    return true;
//}