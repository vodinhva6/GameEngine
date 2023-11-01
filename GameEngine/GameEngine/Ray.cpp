#include "Ray.h"
#include "GameEngine.h"
#include "MyMath.h"
#include "Misc.h"
Ray::Ray(ID3D11Device* device, const VECTOR3& begin, const VECTOR3& end, const VECTOR4& color, RayType Type): begin(begin),end(end),color(color),type(Type)
{
    Vertex Vertexx[2];
    Vertexx[0].position = begin;
    Vertexx[1].position = end;
    uint32_t indicess[2];
    indicess[0] = 0;
    indicess[1] = 1;

    createCOMBuffers(device, Vertexx, 2, indicess, 2);

   
}
