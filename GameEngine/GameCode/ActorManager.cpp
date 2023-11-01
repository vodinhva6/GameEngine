#include "ActorManager.h"
#include "MeshLoaderManager.h"
#include "AnimatorManager.h"
#include "GameEngine.h"
ActorManager::~ActorManager()
{
    
}

std::shared_ptr<Player> ActorManager::getPlayer()
{
    for (auto& it : ActorList)
    {
        std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(it);
        if (player.get())
            return player;
    }

    return nullptr;
}


void ActorManager::Init()
{
#ifdef _DEBUG
    actorDebug = std::make_unique<ActorDebug>();
    actorDebug.get()->Init();
#endif // _DEBUG
    MeshLoaderManager* meshLoaderManager = GetFrom<MeshLoaderManager>(GameEngine::get()->getMeshLoaderManager());
    AnimatorManager* animationManager = GetFrom<AnimatorManager>(GameEngine::get()->getAnimatorManager());
    for (auto& it : ActorList)
    {
        std::shared_ptr<OBJ3D> obj = std::dynamic_pointer_cast<OBJ3D>(it);
        if (obj.get())
        {
            if (obj->meshInfor.mesh_)
            {
                pSmartVoid pMesh = SetToPoint(obj->meshInfor.mesh_);
                meshLoaderManager->RegisterMesh(pMesh, obj->meshInfor.materials_);
                if(obj->meshInfor.animator_)
                    animationManager->RegsiterAnimator(obj->meshInfor.animator_);
            }
            if (obj->sprite3D_)
            {

            }
        }
        
    }


}


void ActorManager::AddListActor(std::shared_ptr<Actor>& obj)
{
    
    int n = 0;
    for (auto& it : ActorList)
    {
        std::string itName = it->getActorName();
        std::string objName = obj->getActorName();
        std::transform(itName.begin(), itName.end(), itName.begin(), ::toupper);
        std::transform(objName.begin(), objName.end(), objName.begin(), ::toupper);
        itName = itName.substr(0, itName.find_last_of("."));
        if (itName.find(objName) != std::string::npos)
        {
            n++;
        }
    }
    if (n != 0)
        obj->setName(obj->getActorName() + "(" + std::to_string(n) + ")");
    ActorList.push_back(obj);

}

void ActorManager::Update(float elapsedTime)
{
#ifdef _DEBUG
    actorDebug.get()->Update();
#endif // _DEBUG
    for (std::vector<std::shared_ptr<Actor>>::iterator it = ActorList.begin(); it != ActorList.end();)
    {
        
        if ((*it).get()->getDeadFlag())
        {
            it = ActorList.erase(it);
        }
        else
        {
            if(GameEngine::get()->getGameMode() == EngineRunMode::EngineDebugMode)
                (*it)->UpdateDebug(elapsedTime);
            else
                (*it)->Update(elapsedTime);
            it++;
        }
    }

}


void ActorManager::resetObject()
{
    ActorList.clear();
}

void ActorManager::Draw()
{
#ifdef _DEBUG
    actorDebug.get()->Draw();
#endif // _DEBUG
    for (auto& it : ActorList)
    {
        it.get()->Draw();

#ifdef _DEBUG
        it->DrawDebug();
#endif // _DEBUG
    }
}

