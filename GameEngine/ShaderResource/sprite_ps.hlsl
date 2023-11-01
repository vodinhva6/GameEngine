#include "sprite.hlsli"
Texture2D color_map : register(t0);
SamplerState point_sampler_state : register(s0);
SamplerState linear_sampler_state : register(s1);
SamplerState anisotropic_sampler_state : register(s2);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = color_map.Sample(anisotropic_sampler_state, pin.texcoord) * pin.color;
    //color.rbg = RGB2HSV(color.rgb);
    //color.r += hueShift;
    //color.g *= saturation;
    //color.b *= brightness;
    //color.rgb = HSV2RGB(color.rgb);
    return color;
}