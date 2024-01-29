#pragma once
#include "Camera.h"
class GameCamera : public Camera
{
public:
    void Init() override;
    void Update(float elapsedTime) override;
    void PerspectiveFovInforUpdate() override;
    void CreateViewPortRay() override;
    void Draw() override;
<<<<<<< HEAD

=======
    void SetAimmingCamera(float disUp, float disScreenWight, float disNear);
    void SetDefault();
>>>>>>> parent of a31de18 (dda)
//public:
//    template < class T>
//    void serialize(T& archive)
//    {
//        archive(a);
//    }
private:
    void GameCameraFirstPerson();
    void GameCameraThirdPerson();
    void Change3DTo2D(float speed);
    void Change2DTo3D(float speed);
    void InitSceneGame(float speed);
    void ChangeProjection(int projectionNum, float speed);
    void ChangeProjectionSlerp(DirectX::XMMATRIX&);
    bool GetChangedProjection2DView();
    void ResetProjections();
    void UpdateChangeProjetion();
    void ChangeSize2DProjection();

    void CameraVibrate();

private:
    bool projection2DViewNow;
    float speedChange;
    float project2DSize;
    float projectionChangeSpeedUpRate;
    float distanceToCameraPos;
    float distanceFromPlayer;
    float distanceUpFromPlayerPos;

    VECTOR3 spherePivot;
    VECTOR3 vectorToSpherePivot;
    VECTOR3 vectorSphereToCameraPos;

private:
    DirectX::XMMATRIX* changeToProjection;
};
