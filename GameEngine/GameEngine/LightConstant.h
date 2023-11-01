#pragma once
#include "ConstantBuffer.h"


#define MAX_SUPP_LIGHTS 10
class LightBuffer : public ConstantBuffer
{
public:
    void CreateBuffer(ID3D11Device* device) override;
    void UpdateBuffer(ID3D11DeviceContext* dcContext) override;
    ~LightBuffer() override {}

private:
    void UpdateConstantData();
    void UpdateDirectionLightData();
    void UpdateDirectionLightShadowMapData();

    void UpdatePointLightData();
    void UpdatePointLightShadowMapData();
    void UpdatePointLightVolumetricLightData();

private:
    struct DirectionLightData
    {
        VECTOR4 lightDirection;
        VECTOR4 lightColor;
        struct DirectionLightShadowMapData
        {
            DirectX::XMFLOAT4X4 viewProjection[16];
            float cascadePlaneDistances[16];

            int cascadeCount = 4;
            float shadowDepthBias;
            float shadowColor;
            float shadowFilterRadius;

            int shadowSampleCount;
            int colorizeCascadedLayer;
            VECTOR2 dummy;
        } directionLightShadowMapData;
    };



    struct PointLightShadowMapData
    {
        DirectX::XMFLOAT4X4 viewProjection[6];

        int slideCount;
        float distance;
        float shadowDepthBias;
        float shadowColor;

        float shadowFilterRadius;
        int shadowSampleCount;
        VECTOR2 dummy;
    } ;

    struct VolumetricLightScatteData
    {
        int volumentricLightScatteringSample;
        float Density;
        float Weight;
        float Decay;

        float Exposure;
        VECTOR3 dummy;
    } ;

    struct BasicElementData
    {
        VECTOR4 position;
        VECTOR4 color;

        float range;
        float power;
        VECTOR2 dummy;
    };
    struct PointLightsData
    {
        BasicElementData basicElementData;
        VolumetricLightScatteData volumetricLightScatteData;
        PointLightShadowMapData shadowMapData;
    };
    struct SpotLightsData
    {
        VECTOR4 position{ 0, 0, 0, 0 };
        VECTOR4 direction{ 0, 0, 1, 0 };
        VECTOR4 color{ 1, 1, 1, 1 };
        float range{ 0 };
        float power{ 0 };
        float innerCorn{ 0.99f };
        float outerCorn{ 0.9f };
    };
    struct LightConstants
    {
        DirectionLightData directionLightData;
        PointLightsData pointLights[MAX_SUPP_LIGHTS];
        SpotLightsData	spotLights[MAX_SUPP_LIGHTS];
        int pointLightsSize;
        int spotLightsSize;
        VECTOR2 dummy;
    } lightConstant;

};
