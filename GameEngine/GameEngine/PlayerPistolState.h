#ifndef PLAYERPISTOLSTATE_H
#define PLAYERPISTOLSTATE_H
#include "StateNode.h"
class PlayerPistolState :
    public StateNode
{
public:
    PlayerPistolState(Character* owner);
    void Enter() override;
    void Run(float elapsedTime) override;
    void Exit() override;
    ~PlayerPistolState() {}

private:
    void StatePistol(std::string&);
};


class PlayerPistolAimState :public StateNode
{
public:
    PlayerPistolAimState(Character* owner);
    void Enter() override;
    void Run(float elapsedTime) override;
    void Exit() override;
    ~PlayerPistolAimState() {}

private:

};


#endif // !PLAYERPISTOLSTATE_H

