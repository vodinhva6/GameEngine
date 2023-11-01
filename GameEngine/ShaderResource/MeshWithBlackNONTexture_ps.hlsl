#include "skinned_mesh.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
#define LINEARCLAMP 3
#define SHADOW 4
SamplerState sampler_states[5] : register(s0);
Texture2D texture_maps[4] : register(t0);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = GetColorFromTexture2D(texture_maps[0],sampler_states[ANISOTROPIC],pin.texcoord);
    color.rgb = 0;
    return color;
}