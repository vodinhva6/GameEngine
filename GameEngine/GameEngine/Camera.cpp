#include "Camera.h"
#include "GameEngine.h"
void Camera::Init()
{
    firstPersonGameCameraMode = false;
    sensitivity = 30;
    downRateFBSVelocity = 0.5f;
    FPSCameraLock = false;

    cameraUp = { 0,1,0 };
    position_ = { 0,0,50 };
    cameraTarget = { 0,0,-10 };
  
    
    FBSVelocity = { 0,0 };
    perspectiveInforNow.angleFovY = 50;
    perspectiveInforNow.nearDis = 0.1f;
    perspectiveInforNow.farDis = 10000;
    perspectiveInforOld = perspectiveInforNow;
    PerspectiveFovInforUpdate();
    UpdateSceneView();
    UpdateProjection();
    UpdateViewProjection();
    //projections[PROJECTIONNAME::ProjectionOrthographicLH] = DirectX::XMMatrixOrthographicLH(Project2DSize * aspect_ratio, Project2DSize, 0.1f, 1000.0f);
    // = projections[PROJECTIONNAME::ProjectionPerspectiveFovLH];

}

void Camera::Update(float elapsedTime)
{
    if (perspectiveInforNow != perspectiveInforOld)
    {
        UpdateProjection();
        CreateViewPortRay();
        perspectiveInforOld = perspectiveInforNow;
    }
    UpdateSceneView();
    Camera::UpdateViewProjection();
}

void Camera::Draw()
{
    DrawStates drawStates;
    drawStates.blendState = BlendStates::Alpha;
    drawStates.depthState = DepthStencilStates::DepthTestOn_DepthWriteOn;
    drawStates.drawType = DrawType::SkinnedMeshObject;
    drawStates.rasterizerState = RasterizerStates::CullBack_DrawTriagles;
    drawStates.samplerState = SamplerStates::Anisotropic;
    GameEngine::get()->DrawSkinnedMesh(meshInfor.mesh_, worldTransform, &meshInfor.materials_, color_, 1, drawStates, nullptr, FrameBufferName::FRAMESKINNEDMESHDEBUG);
    OBJ3D::DrawDebug();
}

void Camera::AssignData(const Camera& data)
{
    position_ = data.position_;
    cameraTarget = data.cameraTarget;
    cameraUp = data.cameraUp;
}

void Camera::PerspectiveFovInforUpdate()
{
}

void Camera::CreateViewPortRay()
{
}



void Camera::UpdateSceneView()
{
    DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&position_);
    DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&cameraTarget);
    DirectX::XMVECTOR Up = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&cameraUp));

    view = { DirectX::XMMatrixLookAtLH(Eye, Focus, Up) };
}

void Camera::UpdateViewProjection()
{
    viewProjection3DOld = viewProjection3DNow;
    viewProjection3DNow = view * projectionNow;
}

void Camera::UpdateProjection()
{
    projectionNow = { DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(perspectiveInforNow.angleFovY), perspectiveInforNow.aspectRatio, perspectiveInforNow.nearDis, perspectiveInforNow.farDis) };

}

VECTOR3 Camera::getCameraRight()
{
    VECTOR3 right = MyMath::get()->Cross(getCameraUp(), getCameraForward());
    return right;
}

VECTOR3 Camera::getCameraForward()
{
    return MyMath::get()->Normalize(getCameraTarget() - position_);
}
