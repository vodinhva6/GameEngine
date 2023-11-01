#include "LightManager.h"

SpotLight::SpotLight(const VECTOR3& pos, const VECTOR3& dir, const VECTOR4& colr, const float& rag, const float& pow, const float& inn, const float& out)
{
    position.x = pos.x;
    position.y = pos.y;
    position.z = pos.z;

    direction.x = dir.x;
    direction.y = dir.y;
    direction.z = dir.z;
    
    color = colr;

    range = rag;

    power = pow;

    innerCorn = inn;

    outerCorn = out;
}

SpotLight::SpotLight()
{
}

SpotLight::~SpotLight()
{
}
