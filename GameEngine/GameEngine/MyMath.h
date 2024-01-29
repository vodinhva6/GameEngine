#pragma once
#include "SingletonPatternTemplate.h"
#include "DataFormat.h"
#include <algorithm>
class MyMath:public SingletonPatternTemplate<MyMath>
{
    friend class SingletonPatternTemplate<MyMath>;
public:
    VECTOR2 Normalize(const VECTOR2& v);

    VECTOR3 Cross(const VECTOR3& v1, const VECTOR3& v2);
    VECTOR3 Normalize(const VECTOR3& v);
    VECTOR3 Lerp(const VECTOR3& a, const VECTOR3& b, float t);
    VECTOR3 ScaleVector3(const VECTOR3& v, float scale);
    VECTOR3 TransformCoordMaxtrix(const VECTOR3& v, const DirectX::XMFLOAT4X4& transform);
    VECTOR3 GetFowardVector(const float& x, const float& y, const float& z);
    VECTOR3 PointTransform(const VECTOR3& point, const VECTOR3& dir, const float& dis);
    VECTOR3 Vector3Rotation(const VECTOR3& vec, const VECTOR3& axis, const float& angle);
    VECTOR4 Normalize(const VECTOR4& v);


    float Dot(const VECTOR3& v1, const VECTOR3& v2);
    float AngleBetweenVectorReturnDegrees(const VECTOR3& v1, const VECTOR3& v2);
    float AngleBetweenVectorReturnDegrees(const VECTOR2& v1, const VECTOR2& v2);
    float AngleBetweenVectorReturnRadian(const VECTOR3& v1, const VECTOR3& v2);
    float AngleBetweenVectorReturnRadian(const VECTOR2& v1, const VECTOR2& v2);
    float Length(const VECTOR3& v);
    float Length(const VECTOR4& v);
    float Length(const VECTOR2& v);
    float floatEquallity(float x);
    float convertToRadian(float angle);
    float convertToDegrees(float angle);
    VECTOR3 convertToDegrees(VECTOR3 angle);
    float convertToRadian(int angle);
    float Clamp(float v, float lo, float hi);
    VECTOR3 Clamp(const VECTOR3& v, float lo, float hi);
    VECTOR2 Clamp(const VECTOR2& v, float lo, float hi);
    float EqualTo10(const float& x);

    int toInt(float x);



    void toZero(float& x, int dozen);


    bool Vector3AndVector3Equal(const VECTOR3& vec1, const VECTOR3& vec2, int dozen);
    bool Vector4AndVector4Equal(const VECTOR4& vec1, const VECTOR4& vec2, int dozen);


    void MatrixDecompose(DirectX::XMMATRIX& matrix, VECTOR3& pos, VECTOR3& rot, VECTOR3& sca);


    double getLambertAlEvaluator(int l);
private:
    double factorial(unsigned int y);


};

