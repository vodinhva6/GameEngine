#ifndef STATENODE_H
#define STATENODE_H
#include "DataFormat.h"
#include <memory>
#include <string>
#include <unordered_map>
class Character;

class StateNode
{
public:

    StateNode(Character* owner);
    virtual void Enter();
    virtual void Run(float elapsedTime);
    virtual void UpdateList(float elapsedTime, float);
    virtual void Exit();
    virtual std::vector<VECTOR4>&getlistQua       () { return listQua; }
    virtual std::vector<int>    &getlistNodeIndex() { return listNodeIndex; }
    virtual std::vector<float>  &getlistValue() { return listValue; }
    std::shared_ptr<StateNode> SearchNode(std::string name);
    bool UpdateState(std::string result);
    std::unordered_map<std::string, std::shared_ptr<StateNode>>& getNodes() { return subNodes; }
    ~StateNode();

protected:
    Character* owner;
    std::unordered_map<std::string, std::shared_ptr<StateNode>> subNodes;
    std::shared_ptr<StateNode> runningSubNode;
    std::shared_ptr<StateNode> runOldSubNode;
    int stateStep;
    float moveSpeed;
public:
    std::vector<VECTOR4> listQua;
    std::vector<int>     listNodeIndex;
    std::vector<float>   listValue;
};



#endif // !STATENODE_H

