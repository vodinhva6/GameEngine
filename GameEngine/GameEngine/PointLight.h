#pragma once
#include "DataFormat.h"
#include "OBJ3D.h"
#include <vector>

class PointLight : public OBJ3D
{
public:
    PointLight(const VECTOR3& pos, const VECTOR4& colr, const float& rag, const float& pow);
    PointLight() {}
    void Init();
    void Update(float elapsedTime) override;
    void Draw() override {}
    void DrawSphere(ID3D11DeviceContext* dc);
    void DrawDebug() override;
    OBJ3DType getTypeOBJ3D() { return OBJ3DType::POINTLIGHT; }
    ~PointLight();

public:
    template < class T>
    void serialize(T& archive)
    {
        archive(cereal::base_class<OBJ3D>(this),
            basicElementData, shadowCastData, volumetricLightScatteData);
    }


    VECTOR3& getPosition() { return position_; }
    VECTOR4& getColor() { return color_; }

    float& getRange() { return basicElementData.range; }
    float& getPower() { return basicElementData.power; }

    void setPosition(const VECTOR3& pos) { position_ = pos; }
    void setColor(const VECTOR4& color) { this->color_ = color; }

    void setRange(const float& range) { basicElementData.range = range; }
    void setPower(const float& power) { basicElementData.power = power; }

public:
    bool& getShadowCast() { return shadowCastData.shadowCast; }
    float& getShadowDepthBias() { return shadowCastData.shadowDepthBias; };
    float& getShadowColor() { return shadowCastData.shadowColor; };
    float& getShadowFilterRadius() { return shadowCastData.shadowFilterRadius; };
    int& getShadowSampleCount() { return shadowCastData.shadowSampleCount; };

    void setShadowCast(const bool& cast) { shadowCastData.shadowCast = cast; }
    void setShadowDepthBias(const float& value) { shadowCastData.shadowDepthBias = value; };
    void setShadowColor(const float& value) { shadowCastData.shadowColor = value; };
    void setShadowFilterRadius(const float& value) { shadowCastData.shadowFilterRadius = value; };
    void setShadowSampleCount(const int& value) { shadowCastData.shadowSampleCount = value; };


public:


    bool& getRenderWithSphere() { return volumetricLightScatteData.renderWithSphere; }
    int& getVolumentricLightScatteSample() { return volumetricLightScatteData.volumentricLightScatteringSample; }
    float& getDensity()  { return volumetricLightScatteData.Density; }
    float& getWeight()   { return volumetricLightScatteData.Weight; }
    float& getDecay()    { return volumetricLightScatteData.Decay; }
    float& getExposure() { return volumetricLightScatteData.Exposure; }

    void setRenderWithSphere(const float& rws) { volumetricLightScatteData.renderWithSphere = rws; }
    void setVolumentricLightScatteSample(const int& sampler) { volumetricLightScatteData.volumentricLightScatteringSample; }

   
private:
    
   
    struct ShadowCast
    {
        bool shadowCast = false;
        float shadowDepthBias = 0.00002f;
        float shadowColor = 0.25f;
        float shadowFilterRadius = 4.000f;
        int shadowSampleCount = 16;

    public:
        template < class T>
        void serialize(T& archive)
        {
            archive(shadowCast, shadowDepthBias, shadowColor, shadowFilterRadius, shadowSampleCount);
        }

    } shadowCastData;
   
    
    struct VolumetricLightScatte
    {
        bool renderWithSphere = false;
        float Density = 1;
        float Weight = 1;
        float Decay = 1;
        float Exposure = 0.05f;
        int volumentricLightScatteringSample = 100;

    public:
        template < class T>
        void serialize(T& archive)
        {
            archive(renderWithSphere, Density, Weight, Decay, Exposure, volumentricLightScatteringSample);
        }
    } volumetricLightScatteData;
    
    struct BasicElement
    {
        float range = 1 ;
        float power = 1 ;
    public:
        template < class T>
        void serialize(T& archive)
        {
            archive(range, power);
        }
    } basicElementData;


    
};

CEREAL_REGISTER_TYPE(PointLight);