#pragma once
#include "DataFormat.h"
#include <vector>

class SpotLight
{
public:
    SpotLight(const VECTOR3& pos, const VECTOR3& dir, const VECTOR4& colr, const float& rag, const float& pow, const float& inn, const float& out);
    SpotLight();
    ~SpotLight();


public:
    VECTOR3& getPosition() { return position; }
    VECTOR3& getDirection() { return direction; }
    VECTOR4& getColor() { return color; }



    float& getRange() { return range; }
    float& getPower() { return power; }
    float& getInnerCorn() { return innerCorn; }
    float& getOuterCorn() { return outerCorn; }


public:
    void setPosition(const VECTOR3& pos) { position = pos; }
    void setDirection(const VECTOR3& dir) { direction = dir; }
    void setColor(const VECTOR4& color) { this->color = color; }


    void setRange(const float& range) { this->range = range; }
    void setPower(const float& power) { this->power = power; }
    void setInnerCorn(const float& innCorn) { innerCorn = innCorn; }
    void setOuterCorn(const float& outCorn) { outerCorn = outerCorn; }

private:

    VECTOR3 position{ 0, 0, 0 };
    VECTOR3 direction{ 0, -1, 0 };
    VECTOR4 color{ 1, 1, 1, 1 };
    float range{ 1 };
    float power{ 1 };
    float innerCorn{ 0.99f };
    float outerCorn{ 0.9f };
};