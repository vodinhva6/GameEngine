#ifndef IDLESTATE_H
#define IDLESTATE_H
#include "StateNode.h"

class PlayerIdleState : public StateNode
{
public:
    PlayerIdleState(Character* owner);
    void Enter() override;
    void Run(float elapedTime) override;
    void Exit() override;
    ~PlayerIdleState();

private:
    void PlayerMove(std::string& result);
    void PlayerBattle(std::string& result);
};

#endif // !IDLESTATE_H
