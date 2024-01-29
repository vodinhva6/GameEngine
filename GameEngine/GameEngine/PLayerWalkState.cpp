#include "PLayerWalkState.h"
#include "GameEngine.h"
#include "CameraManager.h"
#include "ActorManager.h"

PLayerWalkState::PLayerWalkState(Character* owner): StateNode(owner)
{
    frontPivot = sinf(MyMath::get()->convertToRadian(2 * 90 / 3));
    backPivot = -sinf(MyMath::get()->convertToRadian(2 * 90 / 3));
    leftPivot = sinf(MyMath::get()->convertToRadian( 90 / 3));;
    rightPivot = sinf(MyMath::get()->convertToRadian(90 / 3));;
}

void PLayerWalkState::Enter()
{
    runOldSubNode = nullptr;
    runningSubNode = nullptr;

    UpdateState(StateMoving());

    stateStep = 0;
}

void PLayerWalkState::Run(float elapedTime)
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
<<<<<<< HEAD
            {
                runOldSubNode->UpdateList(elapedTime, 1.5f);
                //owner->meshInfor.animator_->SetOldBlendAnimation(&runOldSubNode->listQua, &runOldSubNode->listNodeIndex, &runOldSubNode->listValue);
            }
              
            if (runningSubNode.get())
            {
                runningSubNode->UpdateList(elapedTime, 1.5f);
            }
               
            
=======
                runOldSubNode->UpdateList(elapsedTime, 0.01f);
              
            if (runningSubNode.get())
                runningSubNode->UpdateList(elapsedTime, 0.01f);
>>>>>>> parent of a31de18 (dda)

            owner->UpdateAnimation(elapedTime);
            break;
        }
        else
        {
            runOldSubNode = runningSubNode;
            stateStep++;
        }
    case 2:
        if (UpdateState(StateMoving())) return;
        runningSubNode->Run(elapedTime);
        break;
    default:
        break;
    }
}

void PLayerWalkState::Exit()
{
    if(runningSubNode.get())
    runningSubNode->Exit();
}

std::string PLayerWalkState::StateMoving()
{
    std::string result = "IDLE";

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
    if (lengthJoy == 0)
    {
        result = "IDLE";
        oldstate = result;
    }
        
    else
    {
        float angle = MyMath::get()->AngleBetweenVectorReturnDegrees({ 0,0,1 }, cameraForward);
        VECTOR3 t = MyMath::get()->Vector3Rotation(MyMath::get()->Normalize({ leftJoy.x,0,leftJoy.y })
            , { 0,1,0 }, angle);
        leftJoy.x = t.x;
        leftJoy.y = t.z;

        float dotJoyWithCam = MyMath::get()->Dot(MyMath::get()->Normalize({ leftJoy.x, 0, leftJoy.y })
            , MyMath::get()->Normalize(cameraForward));
        float dotPlayerWithCam = MyMath::get()->Dot(playerForwardVec, cameraForward);
        float CrossY = MyMath::get()->Cross({ leftJoy.x,0,leftJoy.y }, cameraForward).y;
        if (dotPlayerWithCam > 0)
        {
            if (dotJoyWithCam > frontPivot && dotJoyWithCam > 0 && (oldstate == "WALK_FORWARD" || oldstate == "WALK_LEFTFRONT" || oldstate == "WALK_RIGHTFRONT" || oldstate == "IDLE"))
            {
                oldstate = "WALK_FORWARD";
                return oldstate;
            }
               
            else 
                if (dotJoyWithCam < backPivot && dotJoyWithCam < 0 && (oldstate == "WALK_BACK" || oldstate == "WALK_LEFTBACK" || oldstate == "WALK_RIGHTBACK" || oldstate == "IDLE"))
                {
                    oldstate = "WALK_BACK";
                    return oldstate;
                }
            if (dotJoyWithCam > leftPivot && dotJoyWithCam < frontPivot)
                if (CrossY > 0 && (oldstate == "WALK_LEFTFRONT" || oldstate == "WALK_FORWARD" || oldstate == "WALK_STRAFELEFT" || oldstate == "IDLE"))
                {
                    oldstate = "WALK_LEFTFRONT";
                    return oldstate;
                } 
            else if(oldstate == "WALK_RIGHTFRONT" || oldstate == "WALK_FORWARD" || oldstate == "WALK_STRAFERIGHT" || oldstate == "IDLE")
                {
                    oldstate = "WALK_RIGHTFRONT";
                    return oldstate;
                } 

             if (dotJoyWithCam < -leftPivot && dotJoyWithCam > backPivot)
                if (CrossY > 0 && (oldstate == "WALK_LEFTBACK" || oldstate == "WALK_BACK" || oldstate == "WALK_STRAFELEFT" || oldstate == "IDLE"))
                {
                    oldstate = "WALK_LEFTBACK";
                    return oldstate;
                } 
                else if (oldstate == "WALK_RIGHTBACK" || oldstate == "WALK_BACK" || oldstate == "WALK_STRAFERIGHT" || oldstate == "IDLE")
                {
                    oldstate = "WALK_RIGHTBACK";
                    return oldstate;
                }



            if (fabsf(dotJoyWithCam) < leftPivot && CrossY < 0 && (oldstate == "WALK_STRAFERIGHT" || oldstate == "WALK_RIGHTFRONT" || oldstate == "WALK_RIGHTBACK" || oldstate == "IDLE"))
            {
                oldstate = "WALK_STRAFERIGHT";
                return oldstate;
            }
            else  if (fabsf(dotJoyWithCam) < rightPivot && CrossY > 0 && (oldstate == "WALK_STRAFELEFT" || oldstate == "WALK_LEFTFRONT" || oldstate == "WALK_LEFTBACK" || oldstate == "IDLE"))
            {
                oldstate = "WALK_STRAFELEFT";
                return oldstate;
            }

            oldstate = "IDLE";
            return oldstate;
        }
    }
            

    return result;
}

std::shared_ptr<StateNode> PLayerWalkState::SearchNode(std::string name)
{
    auto it = subNodes.find(name);
    return it->second;
}

bool PLayerWalkState::UpdateState(std::string result)
{
    if (result == "IDLE")
    {
        owner->getStateMachine()->ChangeState("IDLE");
        return true;
    }
    auto t = SearchNode(result);
    if (t.get() != runningSubNode.get())
    {
        runningSubNode = t;
        stateStep = 0;
        return true;
    }
    return false;
}


PlayerWalkForwardState::PlayerWalkForwardState(Character* owner) : StateNode(owner)
{
    moveSpeed = 0.01f;
    {
        DirectX::XMVECTOR V = DirectX::XMQuaternionIdentity();
        DirectX::XMVECTOR rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), DirectX::XMConvertToRadians(0));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMConvertToRadians(-39));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMConvertToRadians(13));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        auto& it = listQua.emplace_back();
        DirectX::XMStoreFloat4(&it, V);
        listNodeIndex.push_back(8);
        listValue.push_back(0);
    }

    {
        DirectX::XMVECTOR V = DirectX::XMQuaternionIdentity();
        DirectX::XMVECTOR rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), DirectX::XMConvertToRadians(-3));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMConvertToRadians(-33));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMConvertToRadians(13));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        auto& it = listQua.emplace_back();
        DirectX::XMStoreFloat4(&it, V);
        listNodeIndex.push_back(15);
        listValue.push_back(0);
    }
}

void PlayerWalkForwardState::Enter()
{
    stateStep = 0;
    owner->SetAnimation("WALK", "WALK_FORWARD");
    owner->BeginBlendingAnimation();
    owner->meshInfor.animator_->SetNextBlendAnimation(&listQua, &listNodeIndex, &listValue);

}

void PlayerWalkForwardState::Run(float elapedTime)
{
    /*
    ImGui::Begin("adaa");
    static float it = 0;
    ImGui::DragFloat("adadff", &moveSpeed, 0.01f, 0.01f, 0.1f);
    ImGui::End(); 
    */
   
    UpdateList(elapedTime, 0.0f);
    owner->meshInfor.animator_->SetNowBlendAnimation(&listQua, &listNodeIndex, &listValue);
    owner->UpdateAnimation(elapedTime);
    owner->impluseMoveSpeed(elapedTime);
    //int frameNow = owner->getFrameAnimationNow();
    //if (fabs(frameNow - 100) < 5  || fabs(frameNow - 115) < 5  || fabs(frameNow - 130) < 5)
    //    owner->addSpeed(sp*0.1f);

}

void PlayerWalkForwardState::UpdateList(float elapsedTime, float speed)
{
    if (listValue.empty()) return;
    
    owner->UpdateMove(moveSpeed, elapsedTime);
    VECTOR3 sp = (owner->getSpeed());
    //if (MyMath::get()->Length(sp) < speed) return;
    sp = MyMath::get()->Normalize(sp);
    CameraManager* cameraManager = GetFrom<CameraManager>(GameEngine::get()->getCameraManager());

    std::shared_ptr<Camera> gameCamera = cameraManager->getCamera(CameraName::GameScene);
    VECTOR3 fw = gameCamera->getCameraForward();
    float angle = MyMath::get()->AngleBetweenVectorReturnRadian({ sp.x,sp.z }, { fw.x,fw.z });
    float crossY = MyMath::get()->Cross(fw, sp).y;
    int g = (crossY > 0) - (crossY < 0);
    float value = angle / cosf(MyMath::get()->convertToRadian(45)) * g;
    listValue.at(0) = MyMath::get()->Clamp(value,-1,1);
    listValue.at(1) = MyMath::get()->Clamp(value, -1, 1);
}

void PlayerWalkForwardState::Exit()
{
    owner->meshInfor.animator_->SetOldBlendAnimation(&listQua, &listNodeIndex, &listValue);
}


void PlayerWalkBackState::Enter()
{
    owner->SetAnimation("WALK", "WALK_BACK");
    owner->BeginBlendingAnimation();
    owner->meshInfor.animator_->SetNextBlendAnimation(&listQua, &listNodeIndex, &listValue);
}

PlayerWalkBackState::PlayerWalkBackState(Character* owner) : StateNode(owner)
{
<<<<<<< HEAD
    moveSpeed = 0.09f;
=======
    moveSpeed = 0.01f;
>>>>>>> parent of a31de18 (dda)
    {
        DirectX::XMVECTOR V = DirectX::XMQuaternionIdentity();
        DirectX::XMVECTOR rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), DirectX::XMConvertToRadians(0));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMConvertToRadians(35));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMConvertToRadians(-6));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        auto& it = listQua.emplace_back();
        DirectX::XMStoreFloat4(&it, V);
        listNodeIndex.push_back(8);
        listValue.push_back(0);
    }

    {
        DirectX::XMVECTOR V = DirectX::XMQuaternionIdentity();
        DirectX::XMVECTOR rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), DirectX::XMConvertToRadians(0));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMConvertToRadians(35));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMConvertToRadians(-10));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        auto& it = listQua.emplace_back();
        DirectX::XMStoreFloat4(&it, V);
        listNodeIndex.push_back(15);
        listValue.push_back(0);
    }
}

void PlayerWalkBackState::Run(float elapedTime)
{
   
    UpdateList(elapedTime, 0.0f);
    owner->meshInfor.animator_->SetNowBlendAnimation(&listQua, &listNodeIndex, &listValue);
    owner->UpdateAnimation(elapedTime);
    owner->impluseMoveSpeed(elapedTime);
    //int frameNow = owner->getFrameAnimationNow();
    //if (fabs(frameNow - 100) < 5 || fabs(frameNow - 115) < 5 || fabs(frameNow - 130) < 5)
    //    owner->addSpeed(sp * 0.1f);
}

void PlayerWalkBackState::UpdateList(float elapsedTime, float speed)
{
   
    owner->UpdateMove(moveSpeed, elapsedTime);
    VECTOR3 sp = (owner->getSpeed());
    //if (MyMath::get()->Length(sp) < speed) return;
    sp = MyMath::get()->Normalize(sp);
    CameraManager* cameraManager = GetFrom<CameraManager>(GameEngine::get()->getCameraManager());

    std::shared_ptr<Camera> gameCamera = cameraManager->getCamera(CameraName::GameScene);
    VECTOR3 fw = gameCamera->getCameraForward();
    float angle = MyMath::get()->AngleBetweenVectorReturnRadian({ sp.x,sp.z }, { -fw.x,-fw.z });
    float crossY = MyMath::get()->Cross(sp, -fw).y;
    int g = (crossY > 0) - (crossY < 0);
    float value = angle / cosf(MyMath::get()->convertToRadian(45)) * g;
    listValue.at(0) = MyMath::get()->Clamp(value, -1, 1);
    listValue.at(1) = MyMath::get()->Clamp(value, -1, 1);
}

void PlayerWalkBackState::Exit()
{
    owner->meshInfor.animator_->SetOldBlendAnimation(&listQua, &listNodeIndex, &listValue);
}




PlayerWalkStrafeLeftState::PlayerWalkStrafeLeftState(Character* owner) : StateNode(owner) 
{
<<<<<<< HEAD
    moveSpeed = 0.09f;
=======
    moveSpeed = 0.01f;
>>>>>>> parent of a31de18 (dda)
    {
        DirectX::XMVECTOR V = DirectX::XMQuaternionIdentity();
        DirectX::XMVECTOR rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), DirectX::XMConvertToRadians(-5));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMConvertToRadians(-15));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMConvertToRadians(3));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        auto& it = listQua.emplace_back();
        DirectX::XMStoreFloat4(&it, V);
        listNodeIndex.push_back(8);
        listValue.push_back(0);
    }

    {
        DirectX::XMVECTOR V = DirectX::XMQuaternionIdentity();
        DirectX::XMVECTOR rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), DirectX::XMConvertToRadians(0));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMConvertToRadians(-15));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMConvertToRadians(11));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        auto& it = listQua.emplace_back();
        DirectX::XMStoreFloat4(&it, V);
        listNodeIndex.push_back(15);
        listValue.push_back(0);
    }
}

void PlayerWalkStrafeLeftState::Enter()
{
    owner->SetAnimation("WALK", "WALK_STRAFELEFT");
    owner->BeginBlendingAnimation();
    owner->meshInfor.animator_->SetNextBlendAnimation(&listQua, &listNodeIndex, &listValue);
}

void PlayerWalkStrafeLeftState::Run(float elapedTime)
{
    UpdateList(elapedTime, 0.0f);
    owner->meshInfor.animator_->SetNowBlendAnimation(&listQua, &listNodeIndex, &listValue);
    owner->UpdateAnimation(elapedTime);
    owner->impluseMoveSpeed(elapedTime);
}

void PlayerWalkStrafeLeftState::UpdateList(float elapsedTime, float speed)
{
    if (listValue.empty()) return;
    owner->UpdateMove(moveSpeed, elapsedTime);
    VECTOR3 sp = (owner->getSpeed());
    //if (MyMath::get()->Length(sp) < speed) return;
    sp = MyMath::get()->Normalize(sp);
    CameraManager* cameraManager = GetFrom<CameraManager>(GameEngine::get()->getCameraManager());

    std::shared_ptr<Camera> gameCamera = cameraManager->getCamera(CameraName::GameScene);
    VECTOR3 right = -gameCamera->getCameraRight();
    float angle = MyMath::get()->AngleBetweenVectorReturnRadian({ sp.x,sp.z }, { right.x,right.z });
    float crossY = MyMath::get()->Cross(right, sp).y;
    int g = (crossY > 0) - (crossY < 0);
    float value = angle / sinf(MyMath::get()->convertToRadian(90 / 3)) * g;
    listValue.at(0) = MyMath::get()->Clamp(value, -1, 1);
    listValue.at(1) = MyMath::get()->Clamp(value, -1, 1);
}

void PlayerWalkStrafeLeftState::Exit()
{
    owner->meshInfor.animator_->SetOldBlendAnimation(&listQua, &listNodeIndex, &listValue);
}

PlayerWalkStrafeRightState::PlayerWalkStrafeRightState(Character* owner) : StateNode(owner) 
{
<<<<<<< HEAD
    moveSpeed = 0.09f;
=======
    moveSpeed = 0.01f;
>>>>>>> parent of a31de18 (dda)
    {
        DirectX::XMVECTOR V = DirectX::XMQuaternionIdentity();
        DirectX::XMVECTOR rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), DirectX::XMConvertToRadians(-5));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMConvertToRadians(15));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMConvertToRadians(-3));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        auto& it = listQua.emplace_back();
        DirectX::XMStoreFloat4(&it, V);
        listNodeIndex.push_back(8);
        listValue.push_back(0);
    }

    {
        DirectX::XMVECTOR V = DirectX::XMQuaternionIdentity();
        DirectX::XMVECTOR rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), DirectX::XMConvertToRadians(0));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMConvertToRadians(15));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMConvertToRadians(-11));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        auto& it = listQua.emplace_back();
        DirectX::XMStoreFloat4(&it, V);
        listNodeIndex.push_back(15);
        listValue.push_back(0);
    }
}

void PlayerWalkStrafeRightState::Enter()
{
    owner->SetAnimation("WALK", "WALK_STRAFERIGHT");
    owner->BeginBlendingAnimation();
    owner->meshInfor.animator_->SetNextBlendAnimation(&listQua, &listNodeIndex, &listValue);
}

void PlayerWalkStrafeRightState::Run(float elapedTime)
{
    UpdateList(elapedTime, 0.0f);
    owner->meshInfor.animator_->SetNowBlendAnimation(&listQua, &listNodeIndex, &listValue);
    owner->UpdateAnimation(elapedTime);
    owner->impluseMoveSpeed(elapedTime);
  
}

void PlayerWalkStrafeRightState::UpdateList(float elapsedTime, float speed)
{
    if (listValue.empty()) return;
    owner->UpdateMove(moveSpeed, elapsedTime);
    VECTOR3 sp = (owner->getSpeed());
    //if (MyMath::get()->Length(sp) < speed) return;
    CameraManager* cameraManager = GetFrom<CameraManager>(GameEngine::get()->getCameraManager());

    std::shared_ptr<Camera> gameCamera = cameraManager->getCamera(CameraName::GameScene);
    VECTOR3 right = gameCamera->getCameraRight();
    float angle = MyMath::get()->AngleBetweenVectorReturnRadian({ sp.x,sp.z }, { right.x,right.z });
    float crossY = MyMath::get()->Cross(sp, right).y;
    int g = (crossY > 0) - (crossY < 0);
    float value = angle / sinf(MyMath::get()->convertToRadian(90 / 3)) * g;
    listValue.at(0) = MyMath::get()->Clamp(value, -1, 1);
    listValue.at(1) = MyMath::get()->Clamp(value, -1, 1);
}

void PlayerWalkStrafeRightState::Exit()
{
    owner->meshInfor.animator_->SetOldBlendAnimation(&listQua, &listNodeIndex, &listValue);
}

PlayerWalkLeftFrontState::PlayerWalkLeftFrontState(Character* owner) : StateNode(owner)
{
<<<<<<< HEAD
    moveSpeed = 0.09f;
=======
    moveSpeed = 0.01f;
>>>>>>> parent of a31de18 (dda)
    {
        DirectX::XMVECTOR V = DirectX::XMQuaternionIdentity();
        DirectX::XMVECTOR rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), DirectX::XMConvertToRadians(0));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMConvertToRadians(25));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMConvertToRadians(-9));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        auto& it = listQua.emplace_back();
        DirectX::XMStoreFloat4(&it, V);
        listNodeIndex.push_back(8);
        listValue.push_back(0);
    }

    {
        DirectX::XMVECTOR V = DirectX::XMQuaternionIdentity();
        DirectX::XMVECTOR rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), DirectX::XMConvertToRadians(0));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMConvertToRadians(20));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMConvertToRadians(-5));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        auto& it = listQua.emplace_back();
        DirectX::XMStoreFloat4(&it, V);
        listNodeIndex.push_back(15);
        listValue.push_back(0);
    }
}

void PlayerWalkLeftFrontState::Enter()
{
    moveSpeed = 0.01f;
    owner->SetAnimation("WALK", "WALK_LEFTFRONT");
    owner->BeginBlendingAnimation();
    owner->meshInfor.animator_->SetNextBlendAnimation(&listQua, &listNodeIndex, &listValue);
}

void PlayerWalkLeftFrontState::Run(float elapedTime)
{
   
    UpdateList(elapedTime, 0.0f);
    owner->meshInfor.animator_->SetNowBlendAnimation(&listQua, &listNodeIndex, &listValue);
    owner->UpdateAnimation(elapedTime);
    owner->impluseMoveSpeed(elapedTime);
}

void PlayerWalkLeftFrontState::UpdateList(float elapsedTime, float speed)
{
    owner->UpdateMove(moveSpeed, elapsedTime);
    VECTOR3 sp = (owner->getSpeed());
    //if (MyMath::get()->Length(sp) < speed) return;
    sp = MyMath::get()->Normalize(sp);
    CameraManager* cameraManager = GetFrom<CameraManager>(GameEngine::get()->getCameraManager());

    std::shared_ptr<Camera> gameCamera = cameraManager->getCamera(CameraName::GameScene);
    VECTOR3 fw = gameCamera->getCameraForward();
    float angle = MyMath::get()->AngleBetweenVectorReturnRadian({ sp.x,sp.z }, { fw.x,fw.z });
    float crossY = MyMath::get()->Cross(sp, fw).y;
    float value = (angle - sinf(MyMath::get()->convertToRadian(90 / 3))) / sinf(MyMath::get()->convertToRadian(90 / 3));
   
    listValue.at(0) =  MyMath::get()->Clamp(value, -1, 1);
    listValue.at(1) =  MyMath::get()->Clamp(value, -1, 1);
}

void PlayerWalkLeftFrontState::Exit()
{
    owner->meshInfor.animator_->SetOldBlendAnimation(&listQua, &listNodeIndex, &listValue);
}


PlayerWalkRightFrontState::PlayerWalkRightFrontState(Character* owner) : StateNode(owner) 
{
<<<<<<< HEAD
    moveSpeed = 0.09f;
=======
    moveSpeed = 0.01f;
>>>>>>> parent of a31de18 (dda)
    {
        DirectX::XMVECTOR V = DirectX::XMQuaternionIdentity();
        DirectX::XMVECTOR rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), DirectX::XMConvertToRadians(0));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMConvertToRadians(-25));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMConvertToRadians(9));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        auto& it = listQua.emplace_back();
        DirectX::XMStoreFloat4(&it, V);
        listNodeIndex.push_back(8);
        listValue.push_back(0);
    }

    {
        DirectX::XMVECTOR V = DirectX::XMQuaternionIdentity();
        DirectX::XMVECTOR rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), DirectX::XMConvertToRadians(0));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMConvertToRadians(-20));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMConvertToRadians(5));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        auto& it = listQua.emplace_back();
        DirectX::XMStoreFloat4(&it, V);
        listNodeIndex.push_back(15);
        listValue.push_back(0);
    }
}

void PlayerWalkRightFrontState::Enter()
{
    moveSpeed = 0.01f;
    owner->SetAnimation("WALK", "WALK_RIGHTFRONT");
    owner->BeginBlendingAnimation();
    owner->meshInfor.animator_->SetNextBlendAnimation(&listQua, &listNodeIndex, &listValue);
}

void PlayerWalkRightFrontState::Run(float elapedTime)
{
   
    UpdateList(elapedTime, 0.0f);
    owner->meshInfor.animator_->SetNowBlendAnimation(&listQua, &listNodeIndex, &listValue);
    owner->UpdateAnimation(elapedTime);
    owner->impluseMoveSpeed(elapedTime);

}

void PlayerWalkRightFrontState::UpdateList(float elapsedTime, float speed)
{
    owner->UpdateMove(moveSpeed, elapsedTime);
    VECTOR3 sp = (owner->getSpeed());
    //if (MyMath::get()->Length(sp) < speed) return;
    sp = MyMath::get()->Normalize(sp);
    CameraManager* cameraManager = GetFrom<CameraManager>(GameEngine::get()->getCameraManager());

    std::shared_ptr<Camera> gameCamera = cameraManager->getCamera(CameraName::GameScene);
    VECTOR3 fw = gameCamera->getCameraForward();
    float angle = MyMath::get()->AngleBetweenVectorReturnRadian({ sp.x,sp.z }, { fw.x,fw.z });
    float crossY = MyMath::get()->Cross(sp, fw).y;
    int g = (crossY > 0) - (crossY < 0);
    float value = (angle - sinf(MyMath::get()->convertToRadian(90 / 3))) / sinf(MyMath::get()->convertToRadian(90 / 3));
    listValue.at(0) =  MyMath::get()->Clamp(value, -1, 1);
    listValue.at(1) =  MyMath::get()->Clamp(value, -1, 1);
}

void PlayerWalkRightFrontState::Exit()
{
    owner->meshInfor.animator_->SetOldBlendAnimation(&listQua, &listNodeIndex, &listValue);
}

void PlayerWalkRightBackState::Enter()
{
    moveSpeed = 0.01f;
    owner->SetAnimation("WALK", "WALK_RIGHTBACK");
    owner->BeginBlendingAnimation();
    owner->meshInfor.animator_->SetNextBlendAnimation(&listQua, &listNodeIndex, &listValue);
}

void PlayerWalkRightBackState::Run(float elapedTime)
{
    owner->UpdateMove(moveSpeed, elapedTime);
    owner->meshInfor.animator_->SetNowBlendAnimation(&listQua, &listNodeIndex, &listValue);
    owner->UpdateAnimation(elapedTime);
    owner->impluseMoveSpeed(elapedTime);
}

void PlayerWalkRightBackState::Exit()
{
    owner->meshInfor.animator_->SetOldBlendAnimation(&listQua, &listNodeIndex, &listValue);
}

void PlayerWalkLeftBackState::Enter()
{
    moveSpeed = 0.01f;
    owner->SetAnimation("WALK", "WALK_LEFTBACK");
    owner->BeginBlendingAnimation();
    owner->meshInfor.animator_->SetNextBlendAnimation(&listQua, &listNodeIndex, &listValue);
}

void PlayerWalkLeftBackState::Run(float elapedTime)
{
    owner->UpdateMove(moveSpeed, elapedTime);
    owner->meshInfor.animator_->SetNowBlendAnimation(&listQua, &listNodeIndex, &listValue);
    owner->UpdateAnimation(elapedTime);
    owner->impluseMoveSpeed(elapedTime);
}

void PlayerWalkLeftBackState::Exit()
{
    owner->meshInfor.animator_->SetOldBlendAnimation(&listQua, &listNodeIndex, &listValue);
}
