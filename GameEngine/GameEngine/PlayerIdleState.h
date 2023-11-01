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
    std::string OwnerMove();
    
};

#endif // !IDLESTATE_H
