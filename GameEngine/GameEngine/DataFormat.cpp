#include "DataFormat.h"
template<class Res>
inline RenderId<Res>::RenderId(Res* res)
{
    res_ = static_cast<void*>(res);
}

template<class Res>
inline RenderId<Res>::~RenderId()
{
}
VECTOR2::VECTOR2(const VECTOR3& v)
{
    x = v.x;
    y = v.y;
}

VECTOR2::VECTOR2(const VECTOR4& v)
{
    x = v.x;
    y = v.y;
}

VECTOR2& VECTOR2::operator=(const VECTOR2& v)
{
    x = v.x; y = v.y; return *this;
}
VECTOR2& VECTOR2::operator=(const VECTOR3& v)
{
    x = v.x; y = v.y; return *this;
}

VECTOR2& VECTOR2::operator+=(const VECTOR2& v)
{
    x += v.x;
    y += v.y;
    return *this;
}


VECTOR2& VECTOR2::operator-=(const VECTOR2& v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

VECTOR2& VECTOR2::operator*=(float f)
{
    x *= f;
    y *= f;
    return *this;
}

VECTOR2& VECTOR2::operator/=(float f)
{
    x /= f;
    y /= f;
    return *this;
}

bool VECTOR2::operator<(const VECTOR2& v)
{
    return (x < v.x && y < v.y);
}

VECTOR2 VECTOR2::operator+() const
{
    return VECTOR2(x, y);
}

VECTOR2 VECTOR2::operator-() const
{
    return VECTOR2(-x, -y);
}

VECTOR2 VECTOR2::operator+(const VECTOR2& v) const
{
    return VECTOR2(x + v.x, y + v.y);
}

VECTOR2 VECTOR2::operator-(const VECTOR2& v) const
{
    return VECTOR2(x - v.x, y - v.y);
}

VECTOR2 VECTOR2::operator*(float f) const
{
    return VECTOR2(x * f, y * f);
}

VECTOR2 operator*(float f, const VECTOR2& v)
{
    return VECTOR2(v.x * f, v.y * f);
}

VECTOR2 VECTOR2::operator/(float f) const
{
    return VECTOR2(x / f, y / f);
}

bool VECTOR2::operator == (const VECTOR2& v) const
{
    return (x == v.x) && (y == v.y);
}

bool VECTOR2::operator != (const VECTOR2& v) const
{
    return (x != v.x) || (y != v.y);
}

VECTOR3::VECTOR3(const VECTOR4& v)
{
    x = v.x; 
    y = v.y;
    z = v.z;
}

VECTOR3& VECTOR3::operator=(const VECTOR3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

VECTOR3& VECTOR3::operator=(const VECTOR4& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}


VECTOR3& VECTOR3::operator+=(const VECTOR3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

VECTOR3& VECTOR3::operator-=(const VECTOR3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

VECTOR3& VECTOR3::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;
    return *this;
}

VECTOR3& VECTOR3::operator/=(float f)
{
    x /= f;
    y /= f;
    z /= f;
    return *this;
}

VECTOR3 VECTOR3::operator+() const
{
    return VECTOR3(x, y, z);
}

VECTOR3 VECTOR3::operator-() const
{
    return VECTOR3(-x, -y, -z);
}

VECTOR3 VECTOR3::operator+(const VECTOR3& v) const
{
    return VECTOR3(x + v.x, y + v.y, z + v.z);
}

VECTOR3 VECTOR3::operator-(const VECTOR3& v) const
{
    return VECTOR3(x - v.x, y - v.y, z - v.z);
}

VECTOR3 VECTOR3::operator*(float f) const
{
    return VECTOR3(x * f, y * f, z * f);
}

VECTOR3 operator*(float f, const VECTOR3& v)
{
    return VECTOR3(v.x * f, v.y * f, v.z * f);
}

VECTOR3 VECTOR3::operator/(float f) const
{
    return VECTOR3(x / f, y / f, z / f);
}

bool VECTOR3::operator == (const VECTOR3& v) const
{
    return (x == v.x) && (y == v.y) && (z == v.z);
}

bool VECTOR3::operator != (const VECTOR3& v) const
{
    return (x != v.x) || (y != v.y) || (z != v.z);
}

VECTOR4& VECTOR4::operator+=(const VECTOR4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

VECTOR4& VECTOR4::operator-=(const VECTOR4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}



VECTOR4& VECTOR4::operator+=(const VECTOR3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}


VECTOR4& VECTOR4::operator=(const VECTOR3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = 1;
    return *this;
}

VECTOR4 VECTOR4::operator-(const VECTOR4& v) const
{
    return VECTOR4(x - v.x, y - v.y, z - v.z, w - v.w);
}

VECTOR4 VECTOR4::operator*(const VECTOR4& v) const
{
    return VECTOR4(x * v.x, y * v.y, z * v.z, w * v.w);
}

VECTOR4& VECTOR4::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;
    w *= f;
    return *this;
}

VECTOR4& VECTOR4::operator/=(float f)
{
    x /= f;
    y /= f;
    z /= f;
    w /= f;

    return *this;
}

VECTOR3::VECTOR3(const VECTOR2& v)
{
    x = v.x;
    y = v.y;
    z = 0;
}

VECTOR3& VECTOR3::operator=(const VECTOR2& v)
{
    x = v.x;
    y = v.y;
    z = 0;
    return *this;
}

//******************************************************************************


