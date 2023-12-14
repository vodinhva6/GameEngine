#include "GameCamera.h"
#include "GameEngine.h"
#include "MousePoint.h"
#include "ControlPad.h"
#include "ActorManager.h"
#include "Collision.h"
void GameCamera::Init()
{
    Camera::Init();

    saveDistanceToCameraPos = 40;
    saveDistanceFromPlayer = -8;
    saveDistanceUpFromPlayerPos = 10;

    SetDefault();
    //if (!boundingHit_.capsule_.get())
    //{
    //    pSmartVoid p;
    //    GameEngine::get()->CreateCapsuleDebug(p, 2, 1, { 1,0,1 });
    //    boundingHit_.capsule_ = std::static_pointer_cast<BoundingCapsule>(p);
    //    p.reset();
    //}
        //OBJ3D::CreateBoundingCapsule(5, 5, { 1,1,1 });
    projection2DViewNow = false;
    changeToProjection = nullptr;
    speedChange = 0;
    project2DSize = 100;
    projectionChangeSpeedUpRate = 1;
    color_ = { 5,5,5,1 };
    firstPersonGameCameraMode = false;
    vectorToSpherePivot = { 1,0,0 };
    vectorSphereToCameraPos = { 0,0,1 };
#if _DEBUG
    pSmartVoid pMesh = SetToPoint(meshInfor.mesh_);
    GameEngine::get()->LoadSkinnedMesh("./EngineObject/Camera/camera.fbx", pMesh);
    meshInfor.mesh_ = GetFromPoint<SkinnedMesh>(pMesh);
#else
    while (ShowCursor(false) >= 0) {}
#endif // _DEBUG
}

void GameCamera::Update(float elapsedTime)
{

    PerspectiveFovInforUpdate();
#ifdef _DEBUG
    ImGui::Begin("Game Camera");
    ImGui::SliderFloat3("Camera Pos", &position_.x, -20, 20);
    ImGui::SliderFloat3("Camera Tagert", &cameraTarget.x, -100, 100);
    ImGui::SliderFloat("Camera sensitivity", &sensitivity, 1, 100);
    ImGui::SliderFloat("FBSVelocity down rate", &downRateFBSVelocity, 0.1f, 1);
    ImGui::SliderFloat("AngleFov", &perspectiveInforNow.angleFovY, 1, 180);
    ImGui::DragFloat("distanceToCameraPos", &distanceToCameraPos, 0.1f, 1.0f, 100.0f, "%.3f");
    ImGui::DragFloat("distanceFromPlayer", &distanceFromPlayer, 0.1f, -100.0f, 100.0f);;
    ImGui::DragFloat("distanceUpFromPlayerPos", &distanceUpFromPlayerPos, 0.1f, 1.0f, 100.0f);;
    ControlPad* padP = GetFrom<ControlPad>(GameEngine::get()->getControlPad());
    if (padP->PressF4(0))
    {
        if (FPSCameraLock)
            ShowCursor(true);


        else
            ShowCursor(false);
        FPSCameraLock = !FPSCameraLock;
    }
    ImGui::End();
  
        
       
#endif // _DEBUG
  
    EngineRunMode mode = GameEngine::get()->getGameMode();

    if (mode == EngineRunMode::EngineGameMode)
    {
        (firstPersonGameCameraMode) ? GameCameraFirstPerson() : GameCameraThirdPerson();
    }
    Camera::Update(elapsedTime);
}

void GameCamera::Draw()
{
    DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&position_);
    DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&cameraTarget);
    DirectX::XMVECTOR Up = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&cameraUp));
    DirectX::XMVECTOR Forward = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(Focus, Eye));
    DirectX::XMVECTOR Right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(Up, Forward));

    Up = DirectX::XMVector3Cross(Forward, Right);
    DirectX::XMMATRIX R = DirectX::XMMatrixIdentity();
    R.r[0] = Right;
    R.r[1] = Up;
    R.r[2] = Forward;
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(Eye);
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1, 1, 1);
    DirectX::XMStoreFloat4x4(&worldTransform, S * R * T);
    Camera::Draw();
}

void GameCamera::SetAimmingCamera(float disUp, float disScreenWight, float disNear)
{
    saveDistanceToCameraPos     = distanceToCameraPos;
    saveDistanceFromPlayer      = distanceFromPlayer;
    saveDistanceUpFromPlayerPos = distanceUpFromPlayerPos;


    distanceUpFromPlayerPos = disUp;
    distanceToCameraPos     = disNear;
    distanceFromPlayer      = disScreenWight;
}

void GameCamera::SetDefault()
{
    distanceUpFromPlayerPos = saveDistanceUpFromPlayerPos;
    distanceToCameraPos = saveDistanceToCameraPos;
    distanceFromPlayer = saveDistanceFromPlayer;
}

GameCamera::~GameCamera()
{
    int a = 1;
    a = 2;
}



void GameCamera::GameCameraFirstPerson()
{
    MousePoint* mouseP = GetFrom<MousePoint>(GameEngine::get()->getMousePoint());
    ControlPad* padP = GetFrom<ControlPad>(GameEngine::get()->getControlPad());
   
    VECTOR2 leftJoy{};
    float gotoYaw = 0;
    if (!FPSCameraLock)
    {
        FBSVelocity += mouseP->getIns() * sensitivity / 100;
        FBSVelocity = MyMath::get()->Clamp(FBSVelocity, -9.9f, 9.9f);
        leftJoy = padP->getPosLeftJoy(0);
        if (padP->HoldeZ(0))
            gotoYaw = -1;
        if (padP->HoldeX(0))
            gotoYaw = 1;
        mouseP->resetPosition();
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


    Eye = DirectX::XMVectorAdd(Eye, DirectX::XMVectorScale(DirectX::XMVector3Normalize(Forward), leftJoy.y * 1.5f));
    Eye = DirectX::XMVectorAdd(Eye, DirectX::XMVectorScale(DirectX::XMVector3Normalize(Right), leftJoy.x * 1.5f));
    Eye = DirectX::XMVectorAdd(Eye, DirectX::XMVectorSet(0, gotoYaw, 0, 0));
    DirectX::XMStoreFloat3(&position_, Eye);
    Focus = DirectX::XMVectorAdd(Eye, Forward);
    DirectX::XMStoreFloat3(&cameraTarget, Focus);
   



    FBSVelocity *= downRateFBSVelocity;
}

void GameCamera::GameCameraThirdPerson()
{
    ActorManager* actorManager = GetFrom<ActorManager>(GameEngine::get()->getActorManager());
    ControlPad* padP = GetFrom<ControlPad>(GameEngine::get()->getControlPad());

    static VECTOR2 rightJoy;
    
    rightJoy = MyMath::get()->Lerp(rightJoy, rightJoy+ padP->getPosRightJoy(0), 0.6f);
    std::shared_ptr<Player> player = actorManager->getPlayer();
    VECTOR3 plPos = player->getPosition();
    //if (!FPSCameraLock)
        //rightJoy = { 0,0 };
    vectorToSpherePivot = MyMath::get()->Normalize(MyMath::get()->Vector3Rotation(vectorToSpherePivot, cameraUp, MyMath::get()->convertToRadian(rightJoy.x * sensitivity)));
    static VECTOR3 temp;
    temp = MyMath::get()->Lerp(temp, plPos + MyMath::get()->ScaleVector3(vectorToSpherePivot, distanceFromPlayer), 0.7f);
    spherePivot.x = temp.x;
    spherePivot.z = temp.z;
    spherePivot.y = MyMath::get()->Lerp(spherePivot.y, plPos.y + distanceUpFromPlayerPos, 0.3f);

    vectorSphereToCameraPos = MyMath::get()->Vector3Rotation(vectorSphereToCameraPos, cameraUp, MyMath::get()->convertToRadian(rightJoy.x * sensitivity));
    
    
    VECTOR3 right = MyMath::get()->Cross(cameraUp, vectorSphereToCameraPos);
    vectorSphereToCameraPos = MyMath::get()->Vector3Rotation(vectorSphereToCameraPos, right, MyMath::get()->convertToRadian(rightJoy.y * sensitivity));
    vectorSphereToCameraPos = MyMath::get()->Lerp(vectorSphereToCameraPos, MyMath::get()->ScaleVector3(vectorSphereToCameraPos, distanceToCameraPos), 0.3f);

    position_ = spherePivot + vectorSphereToCameraPos;
    cameraTarget = spherePivot - vectorSphereToCameraPos;


    DirectX::XMVECTOR Eye;
    DirectX::XMVECTOR Focus;
    DirectX::XMVECTOR Up;

    Up = DirectX::XMLoadFloat3(&cameraUp);
    Eye = DirectX::XMLoadFloat3(&position_);
    Focus = DirectX::XMLoadFloat3(&cameraTarget);

    view = { DirectX::XMMatrixLookAtLH(Eye, Focus, Up) };
    if (FPSCameraLock)
    {
        MousePoint* mouseP = GetFrom<MousePoint>(GameEngine::get()->getMousePoint());
        mouseP->resetPosition();
    }
    rightJoy *= 0.85f;
}


void GameCamera::InitSceneGame(float speed)
{
    if (projection2DViewNow)
    {
        // 2D -> 3D
        //ChangeProjection(PROJECTIONNAME::ProjectionPerspectiveFovLH, 0.0001f);
        projectionChangeSpeedUpRate = 1.15f;
        projection2DViewNow = false;

    }
    else
    {
        // 3D -> 2D
        //ChangeProjection(PROJECTIONNAME::ProjectionOrthographicLH, 0.1f);
        projectionChangeSpeedUpRate = 1;
        projection2DViewNow = true;
        project2DSize = 100;

    }
}

void GameCamera::ChangeProjection(int projectionNum, float speed)
{
    //changeToProjection = &projections[projectionNum];
    speedChange = speed;
}

void GameCamera::ChangeProjectionSlerp(DirectX::XMMATRIX& changeToPjection)
{

    DirectX::XMFLOAT4X4 projec;
    DirectX::XMStoreFloat4x4(&projec, projectionNow);
    float aspect_ratio = GameEngine::get()->getAspectRatio(FrameBufferName::MSAABUFFER1);
    DirectX::XMFLOAT4X4 Toprojec;
    DirectX::XMStoreFloat4x4(&Toprojec, changeToPjection);


    VECTOR4 row1 = { projec._11,projec._12,projec._13,projec._14 };
    VECTOR4 row2 = { projec._21,projec._22,projec._23,projec._24 };
    VECTOR4 row3 = { projec._31,projec._32,projec._33,projec._34 };
    VECTOR4 row4 = { projec._41,projec._42,projec._43,projec._44 };

    VECTOR4 toRow1 = { Toprojec._11,Toprojec._12,Toprojec._13,Toprojec._14 };
    VECTOR4 toRow2 = { Toprojec._21,Toprojec._22,Toprojec._23,Toprojec._24 };
    VECTOR4 toRow3 = { Toprojec._31,Toprojec._32,Toprojec._33,Toprojec._34 };
    VECTOR4 toRow4 = { Toprojec._41,Toprojec._42,Toprojec._43,Toprojec._44 };

    bool equal1 = MyMath::get()->Vector4AndVector4Equal(row1, toRow1, 5);
    bool equal2 = MyMath::get()->Vector4AndVector4Equal(row2, toRow2, 5);
    bool equal3 = MyMath::get()->Vector4AndVector4Equal(row3, toRow3, 5);
    bool equal4 = MyMath::get()->Vector4AndVector4Equal(row4, toRow4, 5);

    DirectX::XMVECTOR Row1 = DirectX::XMLoadFloat4(&row1);
    DirectX::XMVECTOR Row2 = DirectX::XMLoadFloat4(&row2);
    DirectX::XMVECTOR Row3 = DirectX::XMLoadFloat4(&row3);
    DirectX::XMVECTOR Row4 = DirectX::XMLoadFloat4(&row4);

    DirectX::XMVECTOR ToRow1 = DirectX::XMLoadFloat4(&toRow1);
    DirectX::XMVECTOR ToRow2 = DirectX::XMLoadFloat4(&toRow2);
    DirectX::XMVECTOR ToRow3 = DirectX::XMLoadFloat4(&toRow3);
    DirectX::XMVECTOR ToRow4 = DirectX::XMLoadFloat4(&toRow4);

    if (equal1)
        Row1 = ToRow1;
    else Row1 = DirectX::XMVectorLerp(Row1, ToRow1, speedChange);

    if (equal2)
        Row2 = ToRow2;
    else Row2 = DirectX::XMVectorLerp(Row2, ToRow2, speedChange);

    if (equal3)
        Row3 = ToRow3;
    else Row3 = DirectX::XMVectorLerp(Row3, ToRow3, speedChange);

    if (equal4)
        Row4 = ToRow4;
    else Row4 = DirectX::XMVectorLerp(Row4, ToRow4, speedChange);

    if (equal1 && equal2 && equal3 && equal4)
    {
        changeToProjection = nullptr;
        return;
    }
    else
    {
        speedChange *= projectionChangeSpeedUpRate;
    }

    projectionNow = { Row1 ,Row2,Row3,Row4 };
}


bool GameCamera::GetChangedProjection2DView()
{
    return(projection2DViewNow && changeToProjection == nullptr);
}


void GameCamera::ResetProjections()
{
    float aspect_ratio = GameEngine::get()->getAspectRatio(FrameBufferName::MSAABUFFER1);
    //projections[PROJECTIONNAME::ProjectionPerspectiveFovLH] = { DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), aspect_ratio, 0.1f, 1000.0f) };
    //projections[PROJECTIONNAME::ProjectionOrthographicLH] = DirectX::XMMatrixOrthographicLH(Project2DSize * aspect_ratio, Project2DSize, 0.1f, 1000.0f);
}

void GameCamera ::UpdateChangeProjetion()
{
    if (changeToProjection)
    {
        ChangeProjectionSlerp(*changeToProjection);
    }
}

void GameCamera::ChangeSize2DProjection()
{
    if (projection2DViewNow && !changeToProjection)
    {
        //float aspect_ratio = GraphicEngine::get()->getAspectRatio();
        //Project2DSize -= MousePoint::get()->getWheelValue() / 10;
        //Project2DSize = clamp(Project2DSize, 50, 200);
        //projectionNow = DirectX::XMMatrixOrthographicLH((Project2DSize)*aspect_ratio, Project2DSize, 0.1f, 1000.0f);
    }
    ;
}

void GameCamera::CameraVibrate()
{
    static float angle = 0;
    float a = cosf(MyMath::get()->convertToRadian(angle + 90));
    //gameCameraInfor.cameraPos.y += a ;
    position_.x += a;
    position_.z += sinf(MyMath::get()->convertToRadian(angle));
    angle += 30;

}


void GameCamera::Change3DTo2D(float speed)
{
    // 3D -> 2D
    //ChangeProjection(PROJECTIONNAME::ProjectionOrthographicLH, speed);
    projectionChangeSpeedUpRate = 1;
    projection2DViewNow = true;
    project2DSize = 100;
}

void GameCamera::Change2DTo3D(float speed)
{
    // 2D -> 3D
    //ChangeProjection(PROJECTIONNAME::ProjectionPerspectiveFovLH, speed);
    projectionChangeSpeedUpRate = 1.15f;
    projection2DViewNow = false;
}

void GameCamera::PerspectiveFovInforUpdate()
{
    perspectiveInforNow.aspectRatio = GameEngine::get()->getAspectRatio(FrameBufferName::MSAABUFFER1);
}

void GameCamera::CreateViewPortRay()
{
    //boundingHit_.raylist.clear();

    //float angleFovX = perspectiveInforNow.angleFovY * perspectiveInforNow.aspectRatio;
    //VECTOR3 begin = {};
    //VECTOR3 forward = cameraTarget - position_;
    //VECTOR3 right = getCameraRight();
    //VECTOR3 up = getCameraUp();
    //pSmartVoid pRayVoid;
    //std::shared_ptr<Ray> ray;
    //
    //VECTOR3 viewTopLeft = MyMath::get()->Vector3Rotation(forward, right, perspectiveInforNow.angleFovY / 2);
    //viewTopLeft = MyMath::get()->Vector3Rotation(viewTopLeft, up, -angleFovX / 2);
    //VECTOR3 end = MyMath::get()->PointTransform(position_, viewTopLeft, perspectiveInforNow.farDis);
    //
    //GameEngine::get()->CreateRay(pRayVoid, begin, -end, {1,0,0,1});
    //ray = std::static_pointer_cast<Ray>(pRayVoid);
    //boundingHit_.raylist.push_back(ray);
    //pRayVoid.reset();
    //ray.reset();
    //
    //
    //VECTOR3 viewTopRight = MyMath::get()->Vector3Rotation(viewTopLeft, up, angleFovX);
    //end = MyMath::get()->PointTransform(position_, viewTopRight, perspectiveInforNow.farDis);
    //
    //GameEngine::get()->CreateRay(pRayVoid, begin, -end, { 1,0,0,1 });
    //ray = std::static_pointer_cast<Ray>(pRayVoid);
    //boundingHit_.raylist.push_back(ray);
    //pRayVoid.reset();
    //ray.reset();
    //
    //
    //VECTOR3 viewBottomLeft = MyMath::get()->Vector3Rotation(forward, right, -perspectiveInforNow.angleFovY / 2);
    //viewBottomLeft = MyMath::get()->Vector3Rotation(viewBottomLeft, up, -angleFovX / 2);
    //end = MyMath::get()->PointTransform(position_, viewBottomLeft, perspectiveInforNow.farDis);
    //
    //GameEngine::get()->CreateRay(pRayVoid, begin, -end, { 1,0,0,1 });
    //ray = std::static_pointer_cast<Ray>(pRayVoid);
    //boundingHit_.raylist.push_back(ray);
    //pRayVoid.reset();
    //ray.reset();
    //
    //
    //VECTOR3 viewBottomRight = MyMath::get()->Vector3Rotation(forward, right, -perspectiveInforNow.angleFovY / 2);
    //viewBottomRight = MyMath::get()->Vector3Rotation(viewBottomRight, up, angleFovX / 2);
    //end = MyMath::get()->PointTransform(position_, viewBottomRight, perspectiveInforNow.farDis);
    //
    //GameEngine::get()->CreateRay(pRayVoid, begin, -end, { 1,0,0,1 });
    //ray = std::static_pointer_cast<Ray>(pRayVoid);
    //boundingHit_.raylist.push_back(ray);
    //pRayVoid.reset();
    //ray.reset();

}
