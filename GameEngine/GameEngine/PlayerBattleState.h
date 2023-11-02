#pragma once
#include "StateNode.h"
class PlayerBattleState :
    public StateNode
{
public:
    PlayerBattleState(Character* owner);
    void Enter() override;
    void Run(float elapsedTime) override;
    void Exit() override;
    ~PlayerBattleState() {}

private:
    void StateBattle(std::string& );


private:

};

