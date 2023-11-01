#pragma once
#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#include "StateNode.h"
class Character;


class StateMachine
{
public:

    StateMachine(Character* owner);
    void Init(std::string stateSet);
    void Run(float elapsedTime);
    void RegisterNode(std::string name, std::shared_ptr<StateNode> node);
    void RegisterSubNode(std::string subName, std::string nodeName, std::shared_ptr<StateNode> node);
    void ChangeState(std::string stateName);
    ~StateMachine();

private:
    void SearchAndPush(std::string subName, std::string nodeName, std::shared_ptr<StateNode>& node);

    std::unordered_map<std::string, std::shared_ptr<StateNode>> nodes;


private:
    std::shared_ptr<StateNode> stateNow;
    Character* owner;
};



#endif // !STATEMACHINE_H

