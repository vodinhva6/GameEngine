#pragma once
#ifndef BOUNDINGINFOR_H
#define BOUNDINGINFOR_H
#include "BoundingSphere.h"
#include "BoundingCapsule.h"
#include "Ray.h"

struct Bounding
{
    BoundingSphere* sphere_ = nullptr;
    BoundingCapsule* capsule_ = nullptr;
    std::vector<std::shared_ptr<Ray>> raylist;
};
#endif // !1

