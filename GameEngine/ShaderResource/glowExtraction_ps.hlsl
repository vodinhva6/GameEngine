#include "samplerStateSlotNum.hlsli"
#include "ConstantBuffer.hlsli"


SamplerState sampler_states[5] : register(s0);

Texture2D textureColorMap : register(t0);
float4 main(float4 position : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
    float4 sampled_color = textureColorMap.Sample(sampler_states[POINT], texcoord);
    return float4(step(bloom_extraction_threshold, max(sampled_color.r, max(sampled_color.g, sampled_color.b))) * sampled_color.rgb, sampled_color.a);

}
