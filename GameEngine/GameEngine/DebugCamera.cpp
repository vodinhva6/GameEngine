#include "DebugCamera.h"
#include "GameEngine.h"
#include "MousePoint.h"
#include "ControlPad.h"
void DebugCamera::Init()
{
    Camera::Init();
    FPSCameraLock = true;
}

void DebugCamera::Update(float elapsedTime)
{
    PerspectiveFovInforUpdate();
#ifdef _DEBUG
    ImGui::Begin("Debug Camera");
    ImGui::SliderFloat3("Camera Pos", &position_.x, -20, 20);
    ImGui::SliderFloat3("Camera Tagert", &cameraTarget.x, -100, 100);
    ImGui::SliderFloat("Camera sensitivity", &sensitivity, 1, 100);
    ImGui::SliderFloat("FBSVelocity down rate", &downRateFBSVelocity, 0.1f, 1);
    ImGui::End();
#endif // _DEBUG
    MousePoint* mousePoint = GetFrom<MousePoint>(GameEngine::get()->getMousePoint());
    ControlPad* controlPad = GetFrom<ControlPad>(GameEngine::get()->getControlPad());

    if (controlPad->PressF2(0))
    {
        FPSCameraLock = !FPSCameraLock;
        if (FPSCameraLock)
            while (ShowCursor(true) < 0) {}
        else
            while (ShowCursor(false) >= 0) {}
    }

    VECTOR2 leftJoy{};
    float gotoYaw = 0;
    if (!FPSCameraLock)
    {
        FBSVelocity += mousePoint->getIns() * sensitivity / 100;
        FBSVelocity = MyMath::get()->Clamp(FBSVelocity, -9.9f, 9.9f);
        leftJoy = controlPad->getPosLeftJoy(0);
        if (controlPad->HoldeZ(0))
            gotoYaw = -1;
        if (controlPad->HoldeX(0))
            gotoYaw = 1;
        mousePoint->resetPosition();
    }
    DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&position_);
    DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&cameraTarget);
    DirectX::XMVECTOR Up = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&cameraUp));

    DirectX::XMVECTOR Forward = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(Focus, Eye));
    DirectX::XMVECTOR Right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(Up, Forward));

    DirectX::XMVECTOR qYaw = DirectX::XMQuaternionRotationAxis(Up, DirectX::XMConvertToRadians(FBSVelocity.x));
    DirectX::XMVECTOR qPitch = DirectX::XMQuaternionRotationAxis(Right, DirectX::XMConvertToRadians(FBSVelocity.y));
    DirectX::XMVECTOR qRotation = DirectX::XMQuaternionMultiply(qPitch, qYaw);

    Forward = DirectX::XMVector3Rotate(Forward, qRotation);

    float angle = DirectX::XMConvertToDegrees(DirectX::XMVectorGetX(DirectX::XMVector3AngleBetweenNormals(Forward, Up)));
    if (angle < 10)
    {
        Right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(Up, Forward));
        Forward = DirectX::XMVector3Normalize(DirectX::XMVector3Rotate(Up, DirectX::XMQuaternionRotationAxis(Right, DirectX::XMConvertToRadians(10))));

    }
    else
        if (angle > 170)
        {
            Right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(Up, Forward));
            Forward = DirectX::XMVector3Normalize(DirectX::XMVector3Rotate(Up, DirectX::XMQuaternionRotationAxis(Right, DirectX::XMConvertToRadians(170))));
        }


    Eye = DirectX::XMVectorAdd(Eye, DirectX::XMVectorScale(DirectX::XMVector3Normalize(Forward), leftJoy.y * 0.5f));
    Eye = DirectX::XMVectorAdd(Eye, DirectX::XMVectorScale(DirectX::XMVector3Normalize(Right), leftJoy.x * 0.5f));
    Eye = DirectX::XMVectorAdd(Eye, DirectX::XMVectorSet(0, gotoYaw, 0, 0));
    DirectX::XMStoreFloat3(&position_, Eye);
    Focus = DirectX::XMVectorAdd(Eye, Forward);
    DirectX::XMStoreFloat3(&cameraTarget, Focus);





    FBSVelocity *= downRateFBSVelocity;
    Camera::Update(elapsedTime);
    
}

void DebugCamera::Draw()
{
}

void DebugCamera::PerspectiveFovInforUpdate()
{
    perspectiveInforNow.aspectRatio = GameEngine::get()->getAspectRatio(FrameBufferName::FRAMESCENEDEBUG);
}
