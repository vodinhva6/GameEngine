#include "geometric_primitive.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
#define LINEARCLAMP 3
#define SHADOW 4
SamplerState sampler_states[5] : register(s0);
Texture2D skyboxMap : register(t0);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = skyboxMap.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
    return color * material_color;
}