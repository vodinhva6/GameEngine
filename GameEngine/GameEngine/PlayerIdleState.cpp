#include "PlayerIdleState.h"
#include "Player.h"
#include "GameEngine.h"
#include "CameraManager.h"
#include "ActorManager.h"
PlayerIdleState::PlayerIdleState(Character* owner): StateNode(owner)
{
}

void PlayerIdleState::Enter()
{
	stateStep = 0;
	owner->SetAnimation("IDLE", "IDLE_FREE");
	owner->meshInfor.animator_->SetNextBlendAnimation(nullptr, nullptr, nullptr);
	owner->BeginBlendingAnimation(0.1f);
	owner->setSpeed({ 0,0,0 });
}

void PlayerIdleState::Run(float elapedTime)
{
	std::string moveState = "";
	PlayerMove(moveState);
	PlayerBattle(moveState);
	owner->meshInfor.animator_->SetNowBlendAnimation(nullptr, nullptr, nullptr);
	owner->UpdateAnimation(elapedTime);
	switch (stateStep)
	{
	case 0:
		if (!owner->GetBlending())
			stateStep++;
		else break;
	case 1:
		if (moveState != "")
			owner->getStateMachine()->ChangeState(moveState);
	default:
		break;
	}

}

void PlayerIdleState::Exit()
{
	owner->meshInfor.animator_->SetOldBlendAnimation(nullptr, nullptr, nullptr);
}

PlayerIdleState::~PlayerIdleState()
{
}

void PlayerIdleState::PlayerMove(std::string& result)
{
	ControlPad* controlPad = GetFrom<ControlPad>(GameEngine::get()->getControlPad());
	CameraManager* cameraManager = GetFrom<CameraManager>(GameEngine::get()->getCameraManager());
	ActorManager* actorManager = GetFrom<ActorManager>(GameEngine::get()->getActorManager());

	std::shared_ptr<Camera> gameCamera = cameraManager->getCamera(CameraName::GameScene);
	
	VECTOR2 leftJoy = controlPad->getPosLeftJoy(0);

	VECTOR3 cameraForward = gameCamera->getCameraForward();
	VECTOR3 playerForwardVec = owner->getVectorForward();
	playerForwardVec.y = 0;
	cameraForward.y = 0;
	float lengthJoy = MyMath::get()->Length(leftJoy);
	float dotJoyWithCam = MyMath::get()->Dot({ leftJoy.x, 0, leftJoy.y }, cameraForward);

	/*if (lengthJoy > 0.7f)
	{
		if (fabsf(dotJoyWithCam) > 0.7f)
			result = "RUN";
		else result = "WALK";
	}
		else */
			if (/*lengthJoy < 0.7f && */lengthJoy > 0.05f)
				result = "WALK";
	
}

void PlayerIdleState::PlayerBattle(std::string& result)
{
	ControlPad* controlPad = GetFrom<ControlPad>(GameEngine::get()->getControlPad());
	if (controlPad->getTriggerLeft(0) > 0.05f)
		result = "BATTLE";
}
