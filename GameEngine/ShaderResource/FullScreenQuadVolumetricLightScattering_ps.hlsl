#include "FullscreenQuad.hlsli"

cbuffer SCATTERING_AT : register(b0)
{
    int slot;
};

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
#define LINEARCLAMP 3
#define SHADOW 4
SamplerState samplerStates[5] : register(s0);
Texture2D sceneBlackMap : register(t0);

float4 PointLightColor(float2 ScreenLightPos, float2 texcoord)
{   
    VolumetricLightScatteData volumetricLightScatteData = pointLights[slot].volumetricLightScatteData;
    
    half2 deltaTexCoord = (texcoord - ScreenLightPos.xy);
    deltaTexCoord *= 1.0f / volumetricLightScatteData.volumentricLightScatteringSample * volumetricLightScatteData.Density;
    half3 color = sceneBlackMap.Sample(samplerStates[LINEAR], texcoord).rgb;
    half illuminationDecay = 1.0f;
    for (int i = 1; i < volumetricLightScatteData.volumentricLightScatteringSample; i++)
    {
        texcoord -= deltaTexCoord;
        half3 sample = sceneBlackMap.Sample(samplerStates[LINEAR], texcoord).rgb;
        sample *= illuminationDecay * volumetricLightScatteData.Weight;
        color += sample;
        illuminationDecay *= volumetricLightScatteData.Decay;
    } 
    return float4(color * volumetricLightScatteData.Exposure, 1);
}

float4 main(VS_OUT pin) : SV_TARGET
{
    BasicElement basicElement = pointLights[slot].basicElement;
    float4 colorOut = sceneBlackMap.Sample(samplerStates[LINEAR], pin.texcoord)/* * color*/;
    float4 screenLightPos = mul(basicElement.position, view_projectionNow);
    screenLightPos /= screenLightPos.w;
    screenLightPos.y = -screenLightPos.y;
    screenLightPos.xy = 0.5f * screenLightPos.xy + 0.5f;
    colorOut += PointLightColor(screenLightPos.xy, pin.texcoord) * basicElement.color;
    return colorOut;
} 
