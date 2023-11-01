#pragma once
#include "Camera.h"
class DebugCamera : public Camera
{
public:
    void Init() override;
    void Update(float elapsedTime) override;
    void PerspectiveFovInforUpdate() override;
    void Draw() override;

public:
    template < class T>
    void serialize(T& archive)
    {
        archive(a);
    }
private:
    int a;
private:
   
private:
    DirectX::XMFLOAT4X4 worldTransform;
};