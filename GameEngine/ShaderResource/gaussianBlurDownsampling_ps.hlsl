#include "samplerStateSlotNum.hlsli"

SamplerState sampler_states[7] : register(s0);

Texture2D textureColorMap : register(t0);

float4 main(float4 position : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
    return textureColorMap.Sample(sampler_states[LINEAR], texcoord, 0.0);
}
