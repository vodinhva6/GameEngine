#pragma once
#include "Camera.h"
#include <memory>
#include <vector>

enum class CameraName
{
    GameScene,
    DebugScene
};

class CameraManager
{
public:
    CameraManager();
    void Init();
    void Update(float elapsedTime);
    void Draw();
    ~CameraManager();


public:
    void setGameCameraTagert(const VECTOR3& tagert);
    void setGameCameraPosition(const VECTOR3& pos);
    void addGameCameraPosition(const VECTOR3& addPos);

    
   

    

public:
    VECTOR3& getGameCameraPosition();
    VECTOR3& getDebugCameraPosition();
    VECTOR3& getGameCameraUpVector(); 
    VECTOR3 getGameCameraForwardVector();
    VECTOR3 getGameCameraRightVector();
    VECTOR3& getGameCameraTagertVector();

    DirectX::XMMATRIX& GetSceneView3DNow();
    DirectX::XMMATRIX& GetSceneView3DOld();
    DirectX::XMMATRIX& GetGameCameraView3D();
    DirectX::XMMATRIX& GetDebugCameraView3D();
    DirectX::XMMATRIX& GetGameCameraProjection3D();
    DirectX::XMMATRIX& GetDebugProjection3D();
    DirectX::XMMATRIX& GetDebugSceneView3D();


    const float& getGameCameraNear();
    const float& getGameCameraFar();

    std::shared_ptr<Camera> getCamera(CameraName name);
   
private:
 
    void UpdateViewAndProjectionCamera(float elapsedTime);
    
   


private:
    std::map<CameraName,std::shared_ptr<Camera>> listCamera;

private:
    
    
};

