#pragma once
#ifndef BOUNDINGINFOR_H
#define BOUNDINGINFOR_H
#include "BoundingSphere.h"
#include "BoundingCapsule.h"
#include "Ray.h"

struct Bounding
{
    std::shared_ptr<BoundingSphere >sphere_ ;
    std::shared_ptr<BoundingCapsule> capsule_;
    std::vector<std::shared_ptr<Ray>> raylist;
};
#endif // !1

