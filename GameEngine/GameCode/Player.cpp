#include "Player.h"
#include "GameEngine.h"
#include "Collision.h"
#include "CameraManager.h"
#include "PlayerIdleState.h"
#include "PLayerWalkState.h"

Player::Player()
{
    moveSpeed = 5;
    MAXSpeed = 1;
    stateMachine = std::make_shared<StateMachine>(this);
    
    stateMachine->RegisterNode("IDLE", std::make_shared<PlayerIdleState>(this));
    stateMachine->RegisterNode("WALK", std::make_shared<PLayerWalkState>(this));

    stateMachine->RegisterSubNode("WALK_FORWARD", "WALK", std::make_shared<PlayerWalkForwardState>(this));
    stateMachine->RegisterSubNode("WALK_BACK", "WALK", std::make_shared<PlayerWalkBackState>(this));
    stateMachine->RegisterSubNode("WALK_STRAFERIGHT", "WALK", std::make_shared<PlayerWalkStrafeRightState>(this));
    stateMachine->RegisterSubNode("WALK_STRAFELEFT", "WALK", std::make_shared<PlayerWalkStrafeLeftState>(this));

    stateMachine->RegisterSubNode("WALK_LEFTFRONT", "WALK", std::make_shared<PlayerWalkLeftFrontState>(this));
    stateMachine->RegisterSubNode("WALK_LEFTBACK", "WALK", std::make_shared<PlayerWalkLeftBackState>(this));
    stateMachine->RegisterSubNode("WALK_RIGHTFRONT", "WALK", std::make_shared<PlayerWalkRightFrontState>(this));
    stateMachine->RegisterSubNode("WALK_RIGHTBACK", "WALK", std::make_shared<PlayerWalkRightBackState>(this));


    stateMachine->Init("IDLE");
    animationClip = "IDLE";
    animationClipSub = "IDLE_FREE";
    animationClipOld = animationClip;
    animationClipSubOld = animationClipSub;
    blending = false;
}

Player::Player(const OBJ3D& obj) : Character(obj)
{
    moveSpeed = 5;
    MAXSpeed = 1;
    stateMachine = std::make_shared<StateMachine>(this);

    stateMachine->RegisterNode("IDLE", std::make_shared<PlayerIdleState>(this));
    stateMachine->RegisterNode("WALK", std::make_shared<PLayerWalkState>(this));

    stateMachine->RegisterSubNode("WALK_FORWARD", "WALK", std::make_shared<PlayerWalkForwardState>(this));
    stateMachine->RegisterSubNode("WALK_BACK", "WALK", std::make_shared<PlayerWalkBackState>(this));
    stateMachine->RegisterSubNode("WALK_STRAFERIGHT", "WALK", std::make_shared<PlayerWalkStrafeRightState>(this));
    stateMachine->RegisterSubNode("WALK_STRAFELEFT", "WALK", std::make_shared<PlayerWalkStrafeLeftState>(this));

    stateMachine->RegisterSubNode("WALK_LEFTFRONT", "WALK", std::make_shared<PlayerWalkLeftFrontState>(this));
    stateMachine->RegisterSubNode("WALK_LEFTBACK", "WALK", std::make_shared<PlayerWalkLeftBackState>(this));
    stateMachine->RegisterSubNode("WALK_RIGHTFRONT", "WALK", std::make_shared<PlayerWalkRightFrontState>(this));
    stateMachine->RegisterSubNode("WALK_RIGHTBACK", "WALK", std::make_shared<PlayerWalkRightBackState>(this));


    stateMachine->Init("IDLE");
    animationClip = "IDLE";
    animationClipSub = "IDLE_FREE";
    animationClipOld = animationClip;
    animationClipSubOld = animationClipSub;
    blending = false;
}

void Player::Update(float elapsedTime)
{
    
    Character::Update(elapsedTime);
    OBJ3D::Update(elapsedTime);
   
  
}


void Player::Draw()
{
    GameEngine::get()->DrawSkinnedMesh(meshInfor.mesh_, worldTransform, &meshInfor.materials_, color_, drawTurn_, drawStates_, animation.key, FrameBufferName::MSAABUFFER1);
}



Player::~Player()
{

}

void Player::UpdateMove(float value, float elapsed_time)
{
    ControlPad* controlPad = GetFrom<ControlPad>(GameEngine::get()->getControlPad());
    CameraManager* cameraManager = GetFrom<CameraManager>(GameEngine::get()->getCameraManager());

    std::shared_ptr<Camera> gameCamera = cameraManager->getCamera(CameraName::GameScene);

    VECTOR2 leftJoy = MyMath::get()->Normalize(controlPad->getPosLeftJoy(0));
    VECTOR3 cameraForward = MyMath::get()->Normalize(gameCamera->getCameraForward());
    VECTOR3 playerForwardVec = MyMath::get()->Normalize(getVectorForward());
    playerForwardVec.y = 0;
    cameraForward.y = 0;

    float angle = MyMath::get()->AngleBetweenVectorReturnDegrees(leftJoy, { 0,1 });
    VECTOR3 moveVec = MyMath::get()->Vector3Rotation(cameraForward, { 0,1,0 }, angle);

    moveVec = MyMath::get()->Normalize(moveVec);
    moveVec = MyMath::get()->ScaleVector3(moveVec, value);
    float plAngle = MyMath::get()->AngleBetweenVectorReturnDegrees(playerForwardVec, cameraForward);
    float factor = 1;
    //addSpeedRotation({ 0,DirectX::XMConvertToRadians(plAngle) * factor,0 });
  
   
    addSpeed(moveVec);
    
}



