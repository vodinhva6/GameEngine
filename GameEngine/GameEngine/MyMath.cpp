#include "MyMath.h"
#include <corecrt_math_defines.h>
VECTOR2 MyMath::Normalize(const VECTOR2& v)
{
    DirectX::XMVECTOR Vec = DirectX::XMLoadFloat2(&v);
    VECTOR2 vec;
    DirectX::XMStoreFloat2(&vec,DirectX::XMVector2Normalize(Vec));

    return vec;
}

VECTOR3 MyMath::Cross(const VECTOR3& v1, const VECTOR3& v2)
{
    VECTOR3 cross = {};
    VECTOR3 norV1 = Normalize(v1);
    VECTOR3 norV2 = Normalize(v2);
    DirectX::XMStoreFloat3(&cross,DirectX::XMVector3Cross(DirectX::XMVectorSet(norV1.x, norV1.y, norV1.z, 0), DirectX::XMVectorSet(norV2.x, norV2.y, norV2.z, 0)));
    return cross;
}

VECTOR3 MyMath::Normalize(const VECTOR3& v)
{
    VECTOR3 norm = {};
    DirectX::XMStoreFloat3(&norm,DirectX::XMVector3Normalize(DirectX::XMVectorSet(v.x, v.y, v.z, 0)));
    return norm;
}

VECTOR3 MyMath::Lerp(const VECTOR3& a, const VECTOR3& b, float t)
{
    DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a);
    DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b);

    A = DirectX::XMVectorLerp(A, B, t);

    VECTOR3 newA;
    DirectX::XMStoreFloat3(&newA, A);

    return newA;
}

VECTOR3 MyMath::ScaleVector3(const VECTOR3& v, float scale)
{
    DirectX::XMVECTOR Vec = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&v));
    Vec = DirectX::XMVectorScale(Vec, scale);
    VECTOR3 vec;
    DirectX::XMStoreFloat3(&vec, Vec);
    return vec;
}

VECTOR3 MyMath::TransformCoordMaxtrix(const VECTOR3& v, const DirectX::XMFLOAT4X4& transform)
{
    DirectX::XMVECTOR V = DirectX::XMLoadFloat3(&v);
    DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&transform);
    VECTOR3 c;
    DirectX::XMStoreFloat3(&c,DirectX::XMVector3TransformCoord(V, M));

    return c;
}

VECTOR3 MyMath::GetFowardVector(const float& x, const float& y, const float& z)
{
    DirectX::XMMATRIX M = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(x), DirectX::XMConvertToRadians(y), DirectX::XMConvertToRadians(z));
    DirectX::XMFLOAT4X4 m;
    DirectX::XMStoreFloat4x4(&m, M);

    return VECTOR3(m._31,m._32,m._33);
}

VECTOR3 MyMath::PointTransform(const VECTOR3& point, const VECTOR3& dir, const float& dis)
{
    DirectX::XMVECTOR Point = DirectX::XMLoadFloat3(&point);
    DirectX::XMVECTOR Dirr = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&dir));
    VECTOR3 transformPoint;
    DirectX::XMStoreFloat3(&transformPoint,DirectX::XMVectorAdd(Point, DirectX::XMVectorScale(Dirr, dis)));
    return transformPoint;
}

VECTOR3 MyMath::Vector3Rotation(const VECTOR3& vec, const VECTOR3& axis, const float& angle)
{
    DirectX::XMVECTOR Vec = DirectX::XMLoadFloat3(&vec);
    DirectX::XMVECTOR Axis = DirectX::XMLoadFloat3(&axis);
    Vec = DirectX::XMVector3Rotate(Vec,DirectX::XMQuaternionRotationAxis(Axis, DirectX::XMConvertToRadians(angle)));
    VECTOR3 newVec;
    DirectX::XMStoreFloat3(&newVec, Vec);
    return newVec;
}


VECTOR4 MyMath::Normalize(const VECTOR4& v)
{
    VECTOR4 norm = {};
    DirectX::XMStoreFloat4(&norm, DirectX::XMVector3Normalize(DirectX::XMLoadFloat4(&v)));
    return norm;
}

float MyMath::Dot(const VECTOR3& v1, const VECTOR3& v2)
{
    float dot = 0.0f;
    DirectX::XMStoreFloat(&dot,DirectX::XMVector3Dot(DirectX::XMVectorSet(v1.x, v1.y, v1.z, 0), DirectX::XMVectorSet(v2.x, v2.y, v2.z, 0)));
    dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    return dot;
}

float MyMath::AngleBetweenVectorReturnDegrees(const VECTOR3& v1, const VECTOR3& v2)
{
    DirectX::XMVECTOR V1 = DirectX::XMLoadFloat3(&v1);
    DirectX::XMVECTOR V2 = DirectX::XMLoadFloat3(&v2);
    V1 = DirectX::XMVector3Normalize(V1);
    V2 = DirectX::XMVector3Normalize(V2);
    float angle = DirectX::XMVectorGetX(DirectX::XMVector3AngleBetweenNormals(V1, V2));
    angle = DirectX::XMConvertToDegrees(angle);
    if (DirectX::XMVectorGetY(DirectX::XMVector3Cross(V1,V2)) < 0)
    {
        angle *= -1;// 360 - angle;
    }
    return angle;
}

float MyMath::AngleBetweenVectorReturnDegrees(const VECTOR2& v1, const VECTOR2& v2)
{
    DirectX::XMVECTOR V1 = DirectX::XMLoadFloat2(&v1);
    DirectX::XMVECTOR V2 = DirectX::XMLoadFloat2(&v2);
    V1 = DirectX::XMVector2Normalize(V1);
    V2 = DirectX::XMVector2Normalize(V2);
    float angle = DirectX::XMVectorGetX(DirectX::XMVector2AngleBetweenNormals(V1, V2));
    angle = DirectX::XMConvertToDegrees(angle);
    if (DirectX::XMVectorGetY(DirectX::XMVector2Cross(V1, V2)) < 0)
    {
        angle *= -1;// 360 - angle;
    }
    return angle;
}

float MyMath::AngleBetweenVectorReturnRadian(const VECTOR3& v1, const VECTOR3& v2)
{
    DirectX::XMVECTOR V1 = DirectX::XMLoadFloat3(&v1);
    DirectX::XMVECTOR V2 = DirectX::XMLoadFloat3(&v2);
    V1 = DirectX::XMVector3Normalize(V1);
    V2 = DirectX::XMVector3Normalize(V2);
    return DirectX::XMVectorGetX(DirectX::XMVector3AngleBetweenNormals(V1, V2));
}

float MyMath::AngleBetweenVectorReturnRadian(const VECTOR2& v1, const VECTOR2& v2)
{
    DirectX::XMVECTOR V1 = DirectX::XMLoadFloat2(&v1);
    DirectX::XMVECTOR V2 = DirectX::XMLoadFloat2(&v2);
    V1 = DirectX::XMVector2Normalize(V1);
    V2 = DirectX::XMVector2Normalize(V2);
    return DirectX::XMVectorGetX(DirectX::XMVector2AngleBetweenNormals(V1, V2));
}

float MyMath::Length(const VECTOR3& v)
{
    float length = 0;
    DirectX::XMVECTOR Vec = DirectX::XMLoadFloat3(&v);
    DirectX::XMStoreFloat(&length, DirectX::XMVector3Length(Vec));
    return length;
}

float MyMath::Length(const VECTOR4& v)
{
    float length = 0;
    DirectX::XMVECTOR Vec = DirectX::XMLoadFloat4(&v);
    DirectX::XMStoreFloat(&length, DirectX::XMVector4Length(Vec));
    return length;
}

float MyMath::Length(const VECTOR2& v)
{
    float length = 0;
    DirectX::XMVECTOR Vec = DirectX::XMLoadFloat2(&v);
    DirectX::XMStoreFloat(&length, DirectX::XMVector3Length(Vec));
    return length;
}

float MyMath::floatEquallity(float x)
{
    return roundf(x);
}

int MyMath::toInt(float x)
{
    x = roundf(x);
    return int(x);
}

void MyMath::toZero(float& x, int dozen)
{
    
    if (x > (-1.0f / powf(10, (float)dozen)) && x < (1.0f / powf(10, (float)dozen)))
        x = 0.0f;
}

bool MyMath::Vector3AndVector3Equal(const VECTOR3& vec1, const VECTOR3& vec2, int dozen)
{
    VECTOR3 Vec = vec1 - vec2;
    toZero(Vec.x,dozen);
    toZero(Vec.y,dozen);
    toZero(Vec.z,dozen);
    return(Vec.x == 0 && Vec.y == 0 && Vec.z == 0);
    
}

bool MyMath::Vector4AndVector4Equal(const VECTOR4& vec1, const VECTOR4& vec2, int dozen)
{
    VECTOR4 Vec = vec1 - vec2;
    toZero(Vec.x,dozen);
    toZero(Vec.y,dozen);
    toZero(Vec.z,dozen);
    toZero(Vec.w,dozen);
    return(Vec.x == 0 && Vec.y == 0 && Vec.z == 0 && Vec.w == 0);
}

void MyMath::MatrixDecompose(DirectX::XMMATRIX& matrix, VECTOR3& pos, VECTOR3& rot, VECTOR3& sca)
{
    DirectX::XMVECTOR scaling, rotationQuat, translation;
    DirectX::XMMatrixDecompose(&scaling, &rotationQuat, &translation, matrix);

    DirectX::XMMATRIX RMatrix = DirectX::XMMatrixRotationQuaternion(rotationQuat);
    DirectX::XMFLOAT4X4 R;
    DirectX::XMStoreFloat4x4(&R, DirectX::XMMatrixTranspose(RMatrix));
  
    rot.x = MyMath::get()->convertToDegrees((float)asin(-R._23));
    rot.y = MyMath::get()->convertToDegrees((float)atan2(R._13, R._33));
    rot.z = MyMath::get()->convertToDegrees((float)atan2(R._21, R._22));

    DirectX::XMStoreFloat3(&pos, translation);
    DirectX::XMStoreFloat3(&sca, scaling);

}

double MyMath::getLambertAlEvaluator(int l)
{
    if (l < 0)       // bogus case
        return 0.;

    if ((l & 1) == 1)
    {
        if (l == 1)
            return 2. * M_PI / 3.;
        else
            return 0.;
    }
    else  // l is even
    {
        double l_fac = factorial((unsigned int)l);
        double l_over2_fac = factorial((unsigned int)(l >> 1));
        double denominator = (l + 2) * (l - 1);
        double sign = ((l >> 1) & 1) ? 1.f : -1.f;  // -1^(l/2 - 1) = -1 when l is a multiple of 4, 1 for other multiples of 2
        double exp2_l = (1 << static_cast<unsigned int>(l));
        return (sign * 2. * M_PI * l_fac) / (denominator * exp2_l * l_over2_fac);
    }
    
}

double MyMath::factorial(unsigned int y)
{
    const double table[16] = { 1., 1., 2., 6., 24., 120., 720., 5040., 40320., 362880.,
        3628800., 39916800., 479001600., 6227020800., 87178291200., 1307674368000. };

    double result = table[(y > 15) ? 15 : y];

    while (y >= 16)
    {
        result = result * double(y);
        y--;
    }

    return result;
}

float MyMath::convertToRadian(float angle)
{
    return DirectX::XMConvertToRadians(angle);
}

float MyMath::convertToDegrees(float angle)
{
    return DirectX::XMConvertToDegrees(angle);
}

VECTOR3 MyMath::convertToDegrees(VECTOR3 angle)
{
    return { DirectX::XMConvertToDegrees(angle.x), DirectX::XMConvertToDegrees(angle.y), DirectX::XMConvertToDegrees(angle.z) };
}

float MyMath::convertToRadian(int angle)
{
    return convertToRadian((float)angle);
}

float MyMath::Clamp(float v, float lo, float hi)
{
    return (std::max)((std::min)(v, hi), lo);
}

VECTOR3 MyMath::Clamp(const VECTOR3& v, float lo, float hi)
{
    float x = (std::max)((std::min)(v.x, hi), lo);
    float y = (std::max)((std::min)(v.y, hi), lo);
    float z = (std::max)((std::min)(v.z, hi), lo);
    return VECTOR3(x,y,z);
}

VECTOR2 MyMath::Clamp(const VECTOR2& v, float lo, float hi)
{
    float x = (std::max)((std::min)(v.x, hi), lo);
    float y = (std::max)((std::min)(v.y, hi), lo);
    return VECTOR2(x, y);
}

float MyMath::EqualTo10(const float& x)
{
    int tX = int(x);
    if ((tX % 10) != 0)
    {
        float t = tX / 10.0f;
        return roundf(t) * 10;
    }
    return x;
}

