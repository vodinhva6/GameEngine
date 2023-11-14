#include "PlayerPistolState.h"
#include "Charater.h"
#include "GameEngine.h"
#include "CameraManager.h"
#include "GameCamera.h"
PlayerPistolState::PlayerPistolState(Character* owner):StateNode(owner)
{

}

void PlayerPistolState::Enter()
{
    stateStep = 0;
    runOldSubNode = nullptr;
    runningSubNode = nullptr;

}

void PlayerPistolState::Run(float elapsedTime)
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
        StatePistol(result);
        if (result == "")
        {
            owner->getStateMachine()->ChangeState("IDLE");
            return;
        }
        if (UpdateState(result)) return;
        runningSubNode->Run(elapsedTime);
        break;
    }
    default:
        break;
    }
}

void PlayerPistolState::Exit()
{
    if(runningSubNode.get())
    runningSubNode->Exit();
}

void PlayerPistolState::StatePistol(std::string& result)
{
    ControlPad* controlPad = GetFrom<ControlPad>(GameEngine::get()->getControlPad());
    if (controlPad->getTriggerLeft(0) > 0.05f)
        result = "PISTOL_AIM";
}

PlayerPistolAimState::PlayerPistolAimState(Character* owner) :StateNode(owner)
{

}

void PlayerPistolAimState::Enter()
{
    stateStep = 0;
    owner->SetAnimation("PISTOL", "PISTOL_AIM");
    owner->BeginBlendingAnimation(0.1f);
    owner->meshInfor.animator_->SetNextBlendAnimation(&listQua, &listNodeIndex, &listValue);
    CameraManager* cameraManager = GetFrom<CameraManager>(GameEngine::get()->getCameraManager());
    std::shared_ptr<GameCamera> gameCamera = std::dynamic_pointer_cast<GameCamera>(cameraManager->getCamera(CameraName::GameScene));
    gameCamera->SetAimmingCamera(20, -8, 12);

}

void PlayerPistolAimState::Run(float elapsedTime)
{
    owner->UpdateMove(0.1f, elapsedTime);
    owner->UpdateAnimation(elapsedTime);
}

void PlayerPistolAimState::Exit()
{
    owner->meshInfor.animator_->SetOldBlendAnimation(&listQua, &listNodeIndex, &listValue);
    CameraManager* cameraManager = GetFrom<CameraManager>(GameEngine::get()->getCameraManager());
    std::shared_ptr<GameCamera> gameCamera = std::dynamic_pointer_cast<GameCamera>(cameraManager->getCamera(CameraName::GameScene));
    gameCamera->SetDefault();
}
