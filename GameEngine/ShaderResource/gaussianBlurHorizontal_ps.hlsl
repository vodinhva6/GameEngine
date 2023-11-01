#include "samplerStateSlotNum.hlsli"
SamplerState sampler_states[7] : register(s0);

Texture2D textureColorMap : register(t0);

float4 main(float4 position : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
    uint mip_level = 0, width, height, number_of_levels;
    textureColorMap.GetDimensions(mip_level, width, height, number_of_levels);
    const float aspect_ratio = width / height;
    
	//http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
    const float offset[3] = { 0.0, 1.3846153846, 3.2307692308 };
    const float weight[3] = { 0.2270270270, 0.3162162162, 0.0702702703 };

    float4 sampled_color = textureColorMap.Sample(sampler_states[LINEARCLAMP], texcoord) * weight[0];
    for (int i = 1; i < 3; i++)
    {
        sampled_color += textureColorMap.Sample(sampler_states[LINEARCLAMP], texcoord + float2(offset[i] / width, 0.0)) * weight[i];
        sampled_color += textureColorMap.Sample(sampler_states[LINEARCLAMP], texcoord - float2(offset[i] / width, 0.0)) * weight[i];
    }

    return sampled_color;
}
