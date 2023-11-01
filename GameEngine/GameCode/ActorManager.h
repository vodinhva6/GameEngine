#pragma once
#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H
#include "Player.h"
#include "Background.h"
#include "OBJ3D.h"
#include "ActorDebug.h"
#include <vector>

class ActorManager
{
public:
    ActorManager() {}
    void Init();
    void Update(float elapsedTime);
    void Draw();
    void resetObject();
    void AddListActor(std::shared_ptr<Actor>& obj);
    ~ActorManager();
public:
    
    template < class T>
    void serialize(T& archive)
    {
        archive(ActorList, actorDebug);
       
    }
    
public:

    ActorDebug* getActorDebug() { return actorDebug.get(); }
    std::vector<std::shared_ptr<Actor>>& getActorList() { return ActorList; }

    std::shared_ptr<Player> getPlayer();

public:

  
private:
    std::vector<std::shared_ptr<Actor>> ActorList;
    std::unique_ptr<ActorDebug> actorDebug;

};
#endif // !OBJECTMANAGER_H

