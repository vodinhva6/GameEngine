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

StateNode::~StateNode()
{
}
