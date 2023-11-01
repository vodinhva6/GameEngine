#include "lights.hlsli"
#include "ShadingFunctions.hlsli"

cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 view_projectionNow;
    row_major float4x4 debugView_projection;
    row_major float4x4 debugView_projectionInvers;
    row_major float4x4 view_projectionNowInvers;
    row_major float4x4 view_projectionOld;
    row_major float4x4 projectionNowInvers;
    float4 gameCameraPosition;
    float4 debugCameraPosition;
    int motisonBlurSample;
    float3 dummy;
   
};


cbuffer SHADOW_CONSTANT_BUFFER : register(b3)
{
    row_major float4x4 shadowmap_view_projection;
    float bias;
    float3 dummyShadow;
};

cbuffer LIGHT_CONSTANT_BUFFER : register(b4)
{
    DirectionLightData directionLightData;
    PointLightsData pointLights[10];
    SpotLightsData	spotLights[10];
    
    int pointLightsUsingAt;
    int spotLightsUsingAt;
    float2 dummyLights;
}
cbuffer TEXTURE_CUSTOM_CONSTANT_BUFFER : register(b5)
{
    float2 scroll_direction;
    float timer;
    float scroll_dummy;
    float2 positionOfFadeOut;
    float2 sizeOfFadeOut;
}
cbuffer FOG_CONSTANT_BUFFER : register(b6)
{
    float4 fogRange;
    float4 fogColor;
}

cbuffer BLUR_CONSTANT_BUFFER : register(b7)
{
    float3 extraction_color;
    int gaussian_half_kernel_size_t;
    float gaussian_sigma;
    float bloom_intensity_temp;
    float2 dummyBlur;
}

cbuffer BLOOM_CONSTANT_BUFFER : register(b8)
{
    float bloom_extraction_threshold;
    float bloom_intensity;
    float2 bloomDummy;
};



