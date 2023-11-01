#include "Actor.h"

std::string Actor::getActorTypeString()
{
    switch (actorType)
    {
    case ActorType::Object3D:
        return "Object3D";
        break;
    case ActorType::Object2D:
        return "Object2D";
        break;
    }
    return "NON";
}

void Actor::Update(float elapsedTime)
{
    timer++;
}

void Actor::Draw()
{
}

void Actor::DrawDebug()
{
}

