#include "StateMachine.h"
#include "Charater.h"

StateMachine::StateMachine(Character* owner): owner(owner)
{
    
}

void StateMachine::Init(std::string stateSet)
{
    stateNow = nodes.find(stateSet)->second;
}

void StateMachine::Run(float elapsedTime)
{
    if(stateNow.get())
    stateNow->Run(elapsedTime);
}

void StateMachine::RegisterNode(std::string name, std::shared_ptr<StateNode> node)
{
    nodes.insert(std::make_pair(name, node));
}

void StateMachine::RegisterSubNode(std::string subName, std::string nodeName, std::shared_ptr<StateNode> node)
{
    auto it = nodes.find(nodeName);
    if (it != nodes.end())
        it->second->getNodes().insert(std::make_pair(subName, node));
    else
        for (auto& node : nodes)
            SearchAndPush(subName, nodeName, node.second);
}

void StateMachine::ChangeState(std::string stateName)
{
    auto it = nodes.find(stateName);
    if (it != nodes.end())
    {
        stateNow->Exit();
        it->second->Enter();
        stateNow = it->second;
    }
}

StateMachine::~StateMachine()
{
}

void StateMachine::SearchAndPush(std::string subName, std::string nodeName, std::shared_ptr<StateNode>& node)
{
    static bool result = false;
    if (result) return;
    auto nodes = node->getNodes();
    auto it = nodes.find(nodeName);
    if (it != nodes.end())
    {
        result = true;
        nodes.insert(std::make_pair(subName, node));
    }
    else
        for (auto& pNode : nodes)
            if (result) break;
            else SearchAndPush(subName, nodeName, node);
}
