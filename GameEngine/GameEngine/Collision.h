#pragma once
#include "DataFormat.h"
#include "SingletonPatternTemplate.h"
#include "SkinnedMesh.h"
#include "OBJ3D.h"
#include "BoundingInfor.h"
#include <string>


class Collision : public SingletonPatternTemplate<Collision>
{
    friend class SingletonPatternTemplate<Collision>;
public:
    //VECTOR3 ObjectAVsObjectBRaycastReturnPoint(OBJ3D& objectA, OBJ3D& objectB, RayType rayType, bool& hit) const;
    //VECTOR3 ObjectAVsObjectBRaycastReturnPoint(OBJ3D& objectA, OBJ3D& objectB, Ray& ray, bool& hit) const;
    VECTOR3 RayCastReturnPoint(OBJ3D& obj, const VECTOR3& Begin, const VECTOR3& End, bool& hit) const;
    VECTOR3 SphereVsSphereReturnPoint(OBJ3D& objectA, OBJ3D& objectB, bool& hit) const;
    VECTOR3 CapsuleVsCapsuleReturnPoint(OBJ3D& objectA, OBJ3D& objectB, bool& hit) const;
    VECTOR3 RayCastMousePointReturnPoint(const VECTOR2& mousePosition, OBJ3D& obj, bool& hit) const;
    VECTOR3 ScreenToWorldTransition(const VECTOR2& mousePosition, const float& maxDis);
    bool RayCastMousePointReturnBool(const VECTOR2& mousePosition, OBJ3D& obj) const;
    
private:

};
