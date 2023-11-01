#pragma once
#include <DirectXMath.h>
#include "OBJ3D.h"
#include "DataFormat.h"
enum class PROJECTIONNAME
{
    ProjectionOrthographicLH,
    ProjectionPerspectiveFovLH
};
class Camera : public OBJ3D
{
public:
    struct PerspectiveFovInfor
    {
    public:

        float angleFovY;
        float nearDis;
        float farDis;
        float aspectRatio;
        bool operator !=(const PerspectiveFovInfor& data)
        {
            return (angleFovY != data.angleFovY || nearDis != data.nearDis || farDis != data.farDis || aspectRatio != data.aspectRatio);
        }
    };

public:
    virtual void Init();
    virtual void Update(float elapsedTime) override;
    virtual void Draw() override;
    virtual void AssignData(const Camera& data);
    virtual void PerspectiveFovInforUpdate();
    virtual void CreateViewPortRay();
protected:
    virtual void UpdateSceneView();
    void UpdateViewProjection();

//public:
//    template < class T>
//    void serialize(T& archive)
//    {
//        archive(a);
//    }
protected:
    VECTOR3 cameraTarget;
    VECTOR3 cameraUp;
    VECTOR2 FBSVelocity;


private:
    void UpdateProjection();

public:
    void setCameraPos(const VECTOR3& pos) { position_ = pos; }
    void setCameraTarget(const VECTOR3& target) { cameraTarget = target; }
    void setCameraUp(const VECTOR3& up) { cameraUp = up; }
public:
    void addCameraPos(const VECTOR3& add) { position_ += add; }
    void addCameraTarget(const VECTOR3& add) { cameraTarget += add; }
    void addCameraUp(const VECTOR3& add) { cameraUp += add; }


    void setFPSLock(const bool& lock) { FPSCameraLock = lock; }

public:
    VECTOR3& getCameraPos() { return position_; }
    VECTOR3& getCameraTarget() { return cameraTarget; }
    VECTOR3& getCameraUp() { return cameraUp; }
    VECTOR3 getCameraRight();
    VECTOR3 getCameraForward();
    bool getFPSLock() { return FPSCameraLock; }


    DirectX::XMMATRIX& GetSceneView3DNow() { return viewProjection3DNow; }
    DirectX::XMMATRIX& GetSceneView3DOld() { return viewProjection3DOld; }
    DirectX::XMMATRIX& GetView3D() { return view; }
    DirectX::XMMATRIX& GetProjection3D() { return projectionNow; }

    PerspectiveFovInfor& getPerspectiveInforNow() { return perspectiveInforNow; }

    float& getSensitivity() { return sensitivity; }
protected:
    DirectX::XMMATRIX viewProjection3DNow;
    DirectX::XMMATRIX viewProjection3DOld;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projectionNow;

protected:
    bool FPSCameraLock;
    float sensitivity;
    float downRateFBSVelocity;
    bool firstPersonGameCameraMode;
    PerspectiveFovInfor perspectiveInforNow;
    PerspectiveFovInfor perspectiveInforOld;

   
};

