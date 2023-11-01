#include "skinned_mesh.hlsli"
#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
#define LINEARCLAMP 3
#define SHADOW 4
SamplerState sampler_states[5] : register(s0);
Texture2D texture_maps[4] : register(t0);
Texture2D shadow_map : register(t4);
float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = texture_maps[0].Sample(sampler_states[ANISOTROPIC], pin.texcoord);

    float4 colorOut = color * pin.color;
    //colorOut.rgb = RGB2HSV(colorOut.rgb);
    //colorOut.r += hueShift;
    //colorOut.g *= saturation;
    //colorOut.b *= brightness;
    //colorOut.b = pow(colorOut.b, 1 / gamma);
    //colorOut.rgb = HSV2RGB(colorOut.rgb);


    return colorOut;
}