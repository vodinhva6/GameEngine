#pragma once
#include "GeometricPrimitive.h"
class BoundingCapsule : public GeometricPrimitive
{
public:
    BoundingCapsule(ID3D11Device* device, const float& height, const float& weight, const VECTOR3& offset);

    const float& getHeight() { return height_; }
    const float& getWeight() { return weight_; }
    const VECTOR3& getOffset() { return offset_; }
private:

    float height_;
    float weight_;
    VECTOR3 offset_;

};

