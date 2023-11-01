#pragma once
#include "GeometricPrimitive.h"
class BoundingSphere : public GeometricPrimitive
{
public:
    BoundingSphere(ID3D11Device* device, const float& radius, const VECTOR3& offset);

    const float& getRadius() { return radius_; }

private:

    float radius_;
    VECTOR3 offset_;
};

