#include "Sprite3D.hlsli"
Texture2D textureMap : register(t0);
SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);
SamplerState anisotropicSampler : register(s2);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = textureMap.Sample(anisotropicSampler, pin.texcoord) * material_color;
    //color.rbg = RGB2HSV(color.rgb);
    //color.r += hueShift;
    //color.g *= saturation;
    //color.b *= brightness;
    //color.rgb = HSV2RGB(color.rgb);
    return color;
}