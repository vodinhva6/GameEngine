
#ifndef PLAYERWALKSTATE_H
#define PLAYERWALKSTATE_H
#include "StateNode.h"

class PLayerWalkState :
    public StateNode
{
public:
    PLayerWalkState(Character* owner);
    void Enter() override;
    void Run(float elapedTime) override;
    void Exit() override;
    ~PLayerWalkState() {}
private:
    std::string StateMoving();
    std::shared_ptr<StateNode> SearchNode(std::string name);
    bool UpdateState(std::string result);

    std::string oldstate;
private:
    std::shared_ptr<StateNode> runningSubNode;
    std::shared_ptr<StateNode> runOldSubNode;
    float frontPivot;
    float backPivot;
    float leftPivot;
    float rightPivot;
};


class PlayerWalkForwardState : public StateNode
{
public:
    PlayerWalkForwardState(Character* owner);
    void Enter() override;
    void Run(float elapedTime) override;
    void UpdateList(float elapsedTime, float speed) override;
    void Exit() override;
    ~PlayerWalkForwardState() {}

private:
};


class PlayerWalkBackState :
    public StateNode
{
public:
    PlayerWalkBackState(Character* owner);
    void Enter() override;
    void Run(float elapedTime) override;
    void UpdateList(float elapsedTime, float speed) override;
    void Exit() override;
    ~PlayerWalkBackState() {}
private:
    
};


class PlayerWalkStrafeLeftState :
    public StateNode
{
public:
    PlayerWalkStrafeLeftState(Character* owner);
    void Enter() override;
    void Run(float elapedTime) override;
    void UpdateList(float elapsedTime, float speed) override;
    void Exit() override;
    ~PlayerWalkStrafeLeftState() {}
private:
};


class PlayerWalkStrafeRightState :
    public StateNode
{
public:
    PlayerWalkStrafeRightState(Character* owner);
    void Enter() override;
    void Run(float elapedTime) override;
    void UpdateList(float elapsedTime, float speed) override;
    void Exit() override;
    ~PlayerWalkStrafeRightState() {}


private:
};


class PlayerWalkLeftFrontState : public StateNode
{
public:
    PlayerWalkLeftFrontState(Character* owner);
    void Enter() override;
    void Run(float elapedTime) override;
    void UpdateList(float elapsedTime, float speed) override;
    void Exit() override;
    ~PlayerWalkLeftFrontState() {}

private:
};

class PlayerWalkRightFrontState : public StateNode
{
public:
    PlayerWalkRightFrontState(Character* owner);
    void Enter() override;
    void Run(float elapedTime) override;
    void UpdateList(float elapsedTime, float speed) override;
    void Exit() override;
    ~PlayerWalkRightFrontState() {}

private:
};

class PlayerWalkRightBackState : public StateNode
{
public:
    PlayerWalkRightBackState(Character* owner) : StateNode(owner) {}
    void Enter() override;
    void Run(float elapedTime) override;
    void Exit() override;
    ~PlayerWalkRightBackState() {}

private:
};

class PlayerWalkLeftBackState : public StateNode
{
public:
    PlayerWalkLeftBackState(Character* owner) : StateNode(owner) {}
    void Enter() override;
    void Run(float elapedTime) override;
    void Exit() override;
    ~PlayerWalkLeftBackState() {}

private:
};
#endif // !PLAYERWALKSTATE_H


