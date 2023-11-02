#include "PlayerBattleState.h"
#include "Charater.h"
#include "GameEngine.h"

PlayerBattleState::PlayerBattleState(Character* owner):StateNode(owner)
{
}

void PlayerBattleState::Enter()
{
    stateStep = 0;
    runOldSubNode = nullptr;
    runningSubNode = nullptr;


}

void PlayerBattleState::Run(float elapsedTime)
{
    switch (stateStep)
    {
    case 0:
        if (runOldSubNode.get())
            runOldSubNode->Exit();
        if (runningSubNode.get())
            runningSubNode->Enter();

        stateStep++;
    case 1:
        if (owner->GetBlending())
        {
            if (runOldSubNode.get())
                runOldSubNode->UpdateList(elapsedTime, 1.5f);

            if (runningSubNode.get())
                runningSubNode->UpdateList(elapsedTime, 1.5f);

            owner->UpdateAnimation(elapsedTime);
            break;
        }
        else
        {
            runOldSubNode = runningSubNode;
            stateStep++;
        }
    case 2:
    {
        std::string result = "";
        StateBattle(result);
        if (UpdateState(result)) return;
        runningSubNode->Run(elapsedTime);
        break;
    }
    default:
        break;
    }
}

void PlayerBattleState::Exit()
{
    if(runningSubNode.get())
    runningSubNode->Exit();
}

void PlayerBattleState::StateBattle(std::string& result)
{
    result = "PISTOL";
}
