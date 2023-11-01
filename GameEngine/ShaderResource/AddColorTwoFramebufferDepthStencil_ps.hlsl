#include "FullscreenQuad.hlsli"

#define POINT 0
#define LINEAR 1
SamplerState sampler_states[5] : register(s0);
Texture2D texture_map1 : register(t0);
Texture2D depth_map1 : register(t1);
Texture2D texture_map2 : register(t2);
Texture2D depth_map2 : register(t3);
//float getDepth(Texture2DMS<float, 4> depthmap, float2 texcoord)
//{
//	uint width, height, number_of_samples;
//	depth_map1.GetDimensions(width, height, number_of_samples);
//
//	float depth;
//	for (uint j = 0; j < number_of_samples; ++j)
//	{
//		depth += depthmap.Load(uint2(texcoord.x * width, texcoord.y * height), j).r;
//	}
//	depth /= number_of_samples;
//	return depth;
//}
float4 main(VS_OUT pin) :SV_TARGET
{
	float4 color1 = texture_map1.Sample(sampler_states[LINEAR], pin.texcoord);
	float4 color2 = texture_map2.Sample(sampler_states[LINEAR], pin.texcoord);

	float dep1 = depth_map1.Sample(sampler_states[LINEAR], pin.texcoord).r;
	float dep2 = depth_map2.Sample(sampler_states[LINEAR], pin.texcoord).r;

	float4 colorOut = color1;
	if (dep1 > dep2) colorOut = color2;
	colorOut.a = min(colorOut.a, 1.0);
	return colorOut;
}
