#include "Shadow.hlsli"


struct VolumetricLightScatteData
{
    int volumentricLightScatteringSample;
    float Density;
    float Weight;
    float Decay;
    
    float Exposure;
    float3 dummyVolume;
};

struct BasicElement
{
    float4 position;
    float4 color;
    
    float range;
    float power;
    float2 dummyelement;
};


struct DirectionLightData
{
    float4 light_direction;
    float4 light_color;
    CSMSHADOWMAP CSMShadowMap;
};
    
    
struct PointLightsData
{
    BasicElement basicElement;
    VolumetricLightScatteData volumetricLightScatteData;
    PointLightShadowCastData shadowCastData;
};
struct SpotLightsData
{
    float4 position;
    float4 direction;
    float4 color;
    float range;
    float power;
    float innerCorn;
    float outerCorn;
};