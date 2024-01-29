#include "LightManager.h"
#include "ActorManager.h"
#include "GameEngine.h"
#ifdef USE_IMGUI
#include "UsesImGui_h.h"
#endif // USE_IMGUI
LightManager::LightManager()
{
    lightDirection = { 0,0,-1,0 };
    vitualPosition = { 0,0,10 };
    lightColor = { 1,1,1,1 };

    shadowManager = std::make_unique<ShadowManager>();
   
}

void LightManager::Init()
{
#ifdef _DEBUG
    pSmartVoid pSprite3D = SetToPoint(pointLightSpr3D);
    GameEngine::get()->CreateSprite3D("./EngineObject/Images/PointLight.png", pSprite3D);
    pointLightSpr3D = GetFromPoint<Sprite3D>(pSprite3D);
    Spr3DForLight();
#endif // !_DEBUG
    pSmartVoid pMesh = SetToPoint(shpereMesh_);
    //GameEngine::get()->LoadSkinnedMesh("Data\\Asset\\3DObject\\Sphere\\Sphere.fbx", pMesh);
    shpereMesh_ = GetFromPoint<Meshes>(pMesh);
    shadowManager.get()->Init();
    
    

    //for (size_t i = 0; i < listPointLights.size(); i++)
    //{
    //    listPointLights[i]->Init(GameEngine::get()->getDevice());
    //}
}
void LightManager::Update()
{
#ifdef _DEBUG
    ImGui::Begin("Light");
    ImGui::DragFloat3("Light Direct", &lightDirection.x, 0.003f, -10.0f, 10.0f);
    ImGui::SliderFloat3("Light position", &vitualPosition.x, -50, 50);

#endif // _DEBUG
    UpdateRemovePointLight();
    shadowManager.get()->Update();

#ifdef _DEBUG
    ImGui::End();
#endif // _DEBUG

   
   
    
    
}

void LightManager::setLightPos(const VECTOR3& pos)
{
    vitualPosition.x = pos.x;
    vitualPosition.y = pos.y;
    vitualPosition.z = pos.z;
}

void LightManager::setLightDir(const VECTOR3& dir)
{
    lightDirection.x = dir.x;
    lightDirection.y = dir.y;
    lightDirection.z = dir.z;
}


void LightManager::DeleteList()
{
    listPointLights.clear();
    listSpotLights.clear();
}

void LightManager::UpdateRemovePointLight()
{
    for(std::vector<std::shared_ptr<PointLight>>::iterator it = listPointLights.begin(); it != listPointLights.end();)
    {
        if ((*it)->getDeadFlag())
            it = listPointLights.erase(it);
        else it++;
    }
}



LightManager::~LightManager()
{
    DeleteList();
}

void LightManager::CreatePointLight()
{
    ActorManager* actorManager = GetFrom<ActorManager>(GameEngine::get()->getActorManager());
    std::shared_ptr<Actor> pointLight;

    PointLight* pL = new PointLight({ 1,0,0 }, { 1,1,1,1 }, 10, 10);
#ifndef DEBUG
    pL->sprite3D_ = pointLightSpr3D;
#endif // DEBUG
    pL->meshInfor.mesh_ = shpereMesh_;
    pointLight.reset(pL);
    listPointLights.push_back(std::dynamic_pointer_cast<PointLight>(pointLight));
    actorManager->AddListActor(pointLight);
}

void LightManager::Spr3DForLight()
{
    for (size_t i = 0; i < listPointLights.size(); i++)
    {
        listPointLights[i]->sprite3D_ = pointLightSpr3D;
    }
}


