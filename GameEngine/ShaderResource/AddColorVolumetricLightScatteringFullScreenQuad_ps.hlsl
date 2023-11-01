#include "FullscreenQuad.hlsli"

#define POINT 0
#define LINEAR 1
SamplerState sampler_states[5] : register(s0);
Texture2D texture_map1 : register(t0);
Texture2D texture_map2 : register(t1);
float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color1 = texture_map1.Sample(sampler_states[LINEAR], pin.texcoord);
    float4 color2 = texture_map2.Sample(sampler_states[LINEAR], pin.texcoord);

    float4 colorOut = color1 + color2;
    colorOut.a = min(colorOut.a, 1.0);
    return colorOut;
}
