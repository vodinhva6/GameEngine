#include "StateNode.h"
#include "GameEngine.h"
#include "CameraManager.h"
#include "ActorManager.h"

StateNode::StateNode(Character* owner):owner(owner)
{
    stateStep = 0;
}

void StateNode::Enter()
{

}

void StateNode::Run(float elapsedTime)
{

}

void StateNode::UpdateList(float elapsedTime, float speed)
{
    
}

void StateNode::Exit()
{

}

std::shared_ptr<StateNode> StateNode::SearchNode(std::string name)
{
    auto it = subNodes.find(name);
    if (it != subNodes.end())
        return it->second;
    return nullptr;
}

bool StateNode::UpdateState(std::string result)
{
    auto t = SearchNode(result);
    if (t.get() != runningSubNode.get())
    {
        runningSubNode = t;
        stateStep = 0;
        return true;
    }
    return false;
}

StateNode::~StateNode()
{
}
