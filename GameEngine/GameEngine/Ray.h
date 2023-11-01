#pragma once
#ifndef RAY_H
#define RAY_H
#include "GeometricPrimitive.h"
enum RayType
{
    NON,
    GroundRay,
    WallRay,
    UptopHeadRay,
    TakingBlockWallRay,
    TakingBlockUpRay,
};
class Ray : public GeometricPrimitive
{
public:
    Ray(ID3D11Device* device, const VECTOR3& begin, const VECTOR3& end, const VECTOR4& color, RayType Type);
    VECTOR4 color;
    const RayType& getType() { return type; }
    const VECTOR3& getBeginPos() { return begin; }
    const VECTOR3& getEndPos() { return end; }
private:
    RayType type;
    VECTOR3 begin;
    VECTOR3 end;
};


#endif // !RAY


