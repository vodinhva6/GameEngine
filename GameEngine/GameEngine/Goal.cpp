#include "Goal.h"
#include "ObjectManager.h"
#include "../Collision.h"
#include "Scene.h"
GoalBlock::GoalBlock(int nameId, int typeId): Block(nameId, typeId)
{
    goalFloor = nullptr;
}

GoalBlock::GoalBlock(const GoalBlock* data): Block(data)
{
    goalFloor = nullptr;
}

void GoalBlock::update(float elapsedTime)
{
    if (goalFloor)
    {
        selectToSurface = goalFloor->getSelect();
    }
    if (selectToSurface)
        color_.w = 1;
    else
        color_.w = 0.1f;
    rotation_ += speedRotation_;
    Player* p = ObjectManager::get()->getPlayer();
    bool hit = false;
    if(p->getOnGround())
    for (auto& it : p->boundingHit_.raylist)
    {
        if (it->getType() != RayType::GroundRay) continue;
        if (goalFloor != nullptr)
        {
            p->position_.y -= 0.1f;
            p->updateWorldTrans();
            Collision::get()->ObjectAVsObjectBRaycastReturnPoint(p, goalFloor, *it, hit);
            if (hit == true)
            {
                SceneManager::get()->changeScene("SCENECLEAR", 0);
                break;
            }
            else
            {
                p->position_.y += 0.1f;
                p->updateWorldTrans();
            }
        }
    }
    Block::update(elapsedTime);
    if (timer % 300 == 0)
    {
        GraphicEngine::get()->drawEffect("GOAL", { position_.x,position_.y - 5,position_.z }, 1,2.0f);
    }
}

void GoalBlock::draw()
{
    int slotBuffer = 1;
    if (!selectToSurface)
        slotBuffer = 2;
    GraphicEngine::get()->drawSkinnedMesh(mesh_, worldTransform, color_, nullptr, drawTurn_, drawStates_, slotBuffer);
}
