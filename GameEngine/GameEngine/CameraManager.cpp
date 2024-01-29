#include "GameEngine.h"
#include "MyMath.h"
#include "CameraManager.h"
#include "GameCamera.h"
#include "DebugCamera.h"
void CameraManager::Init()
{
    for (auto& camera : listCamera)
    {
        camera.second.reset();
    }
    listCamera.clear();
    CameraName name = CameraName::GameScene;
    std::shared_ptr<GameCamera> gamecamera = std::make_shared<GameCamera>();
    gamecamera.get()->Init();
    gamecamera.get()->setName("Camera");

    std::shared_ptr<DebugCamera> debugcamera = std::make_shared<DebugCamera>();
    debugcamera.get()->AssignData(*gamecamera.get());
    debugcamera.get()->Init();

    listCamera.insert(std::make_pair(name, std::move(gamecamera)));

    name = CameraName::DebugScene;
    listCamera.insert(std::make_pair(name, std::move(debugcamera)));
}


void CameraManager::Update(float elapsedTime)
{
    ControlPad* padP = GetFrom<ControlPad>(GameEngine::get()->getControlPad());
    UpdateViewAndProjectionCamera(elapsedTime);
    if (padP->PressF3(0))
    {
        listCamera.at(CameraName::GameScene).get()->AssignData(*listCamera.at(CameraName::DebugScene).get());
    }
}
void CameraManager::Draw()
{
#ifdef _DEBUG
    listCamera.at(CameraName::GameScene).get()->Draw();
#endif // _DEBUG

   
        
}


void CameraManager::UpdateViewAndProjectionCamera(float elapsedTime)
{
    listCamera.at(CameraName::GameScene).get()->Update(elapsedTime);
    listCamera.at(CameraName::DebugScene).get()->Update(elapsedTime);
}



VECTOR3& CameraManager::getGameCameraTagertVector()
{
    return listCamera.at(CameraName::GameScene).get()->getCameraTarget();
}

DirectX::XMMATRIX& CameraManager::GetSceneView3DNow()
{
    return listCamera.at(CameraName::GameScene).get()->GetSceneView3DNow();
}

DirectX::XMMATRIX& CameraManager::GetSceneView3DOld()
{
    return listCamera.at(CameraName::GameScene).get()->GetSceneView3DOld();
}

DirectX::XMMATRIX& CameraManager::GetGameCameraView3D()
{
    return listCamera.at(CameraName::GameScene).get()->GetView3D();
}

DirectX::XMMATRIX& CameraManager::GetDebugCameraView3D()
{
    return listCamera.at(CameraName::DebugScene).get()->GetView3D();
}

DirectX::XMMATRIX& CameraManager::GetGameCameraProjection3D()
{
    return listCamera.at(CameraName::GameScene).get()->GetProjection3D();
}

DirectX::XMMATRIX& CameraManager::GetDebugProjection3D()
{
    return listCamera.at(CameraName::DebugScene).get()->GetProjection3D();
}

DirectX::XMMATRIX& CameraManager::GetDebugSceneView3D()
{
    return listCamera.at(CameraName::DebugScene).get()->GetSceneView3DNow();
}

const float& CameraManager::getGameCameraNear()
{
    return listCamera.at(CameraName::GameScene)->getPerspectiveInforNow().nearDis;
}

const float& CameraManager::getGameCameraFar()
{
    return listCamera.at(CameraName::GameScene)->getPerspectiveInforNow().farDis;
}

std::shared_ptr<Camera> CameraManager::getCamera(CameraName name)
{
    return listCamera.at(name);
}


VECTOR3& CameraManager::getGameCameraPosition()
{
    return listCamera.at(CameraName::GameScene).get()->getCameraPos();
}

VECTOR3& CameraManager::getGameCameraUpVector()
{
    return listCamera.at(CameraName::GameScene).get()->getCameraUp();
}


VECTOR3& CameraManager::getDebugCameraPosition()
{
    return listCamera.at(CameraName::DebugScene).get()->getCameraPos();
}


VECTOR3 CameraManager::getGameCameraForwardVector()
{
    return MyMath::get()->Normalize(listCamera.at(CameraName::GameScene).get()->getCameraTarget());
}

void CameraManager::setGameCameraTagert(const VECTOR3& tagert)
{
    listCamera.at(CameraName::GameScene).get()->setCameraTarget(tagert);
}
void CameraManager::setGameCameraPosition(const VECTOR3& pos)
{
    listCamera.at(CameraName::GameScene).get()->setCameraPos(pos);
}

void CameraManager::addGameCameraPosition(const VECTOR3& addPos)
{
    listCamera.at(CameraName::GameScene).get()->addCameraPos(addPos);
}
VECTOR3 CameraManager::getGameCameraRightVector()
{
    VECTOR3 right = MyMath::get()->Cross(getGameCameraUpVector(), getGameCameraForwardVector());
    return right;
}



CameraManager::~CameraManager()
{
}

CameraManager::CameraManager()
{

}


