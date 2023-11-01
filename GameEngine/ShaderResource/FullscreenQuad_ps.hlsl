#include "FullscreenQuad.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
#define FXAA_REDUCE_MIN (1.0/128.0)
#define FXAA_REDUCE_MUL (1.0/8.0)
#define FXAA_SPAN_MAX 8.0
#define FXAA_SUBPIX_SHIFT (1.0 / 4.0)
#define FXAA_SAMPLER 4


SamplerState sampler_states[5] : register(s0);

float3 fxaa(Texture2D framebuffer, float4 pos_pos, float2 rcp_frame)
{
	float3 sampled_nw = framebuffer.Sample(sampler_states[FXAA_SAMPLER], pos_pos.zw).xyz;
	float3 sampled_ne = framebuffer.Sample(sampler_states[FXAA_SAMPLER], pos_pos.zw, int2(1, 0)).xyz;
	float3 sampled_sw = framebuffer.Sample(sampler_states[FXAA_SAMPLER], pos_pos.zw, int2(0, 1)).xyz;
	float3 sampled_se = framebuffer.Sample(sampler_states[FXAA_SAMPLER], pos_pos.zw, int2(1, 1)).xyz;
	float3 sampled_m = framebuffer.Sample(sampler_states[FXAA_SAMPLER], pos_pos.xy).xyz;

	float3 luma = float3(0.299, 0.587, 0.114);
	float luma_nw = dot(sampled_nw, luma);
	float luma_ne = dot(sampled_ne, luma);
	float luma_sw = dot(sampled_sw, luma);
	float luma_se = dot(sampled_se, luma);
	float luma_m = dot(sampled_m, luma);

	float luma_min = min(luma_m, min(min(luma_nw, luma_ne), min(luma_sw, luma_se)));
	float luma_max = max(luma_m, max(max(luma_nw, luma_ne), max(luma_sw, luma_se)));

	float2 dir;
	dir.x = -((luma_nw + luma_ne) - (luma_sw + luma_se));
	dir.y = ((luma_nw + luma_sw) - (luma_ne + luma_se));

	float dir_reduce = max((luma_nw + luma_ne + luma_sw + luma_se) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
	float rcp_dir_min = 1.0 / (min(abs(dir.x), abs(dir.y)) + dir_reduce);
	dir = min(float2(FXAA_SPAN_MAX, FXAA_SPAN_MAX), max(float2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * rcp_dir_min)) * rcp_frame;

	float3 rgb_a = (1.0 / 2.0) * (
		framebuffer.Sample(sampler_states[FXAA_SAMPLER], pos_pos.xy + dir * (1.0 / 3.0 - 0.5)).xyz +
		framebuffer.Sample(sampler_states[FXAA_SAMPLER], pos_pos.xy + dir * (2.0 / 3.0 - 0.5)).xyz);
	float3 rgb_b = rgb_a * (1.0 / 2.0) + (1.0 / 4.0) * (
		framebuffer.Sample(sampler_states[FXAA_SAMPLER], pos_pos.xy + dir * (0.0 / 3.0 - 0.5)).xyz +
		framebuffer.Sample(sampler_states[FXAA_SAMPLER], pos_pos.xy + dir * (3.0 / 3.0 - 0.5)).xyz);
	float luma_b = dot(rgb_b, luma);

	return ((luma_b < luma_min) || (luma_b > luma_max)) ? rgb_a : rgb_b;
}

#if 1
//Texture2DMS<float4> texture_map : register(t0);
Texture2D texture_map : register(t0);
float4 main(VS_OUT pin) :SV_TARGET
{
	float4 color = texture_map.Sample(sampler_states[LINEAR], pin.texcoord);

	return color;
}

#else
Texture2D texture_map : register(t0);
Texture2D mask_map : register(t1);

float4 main(VS_OUT pin) :SV_TARGET
{
	


	float2 rcp_frame = float2(1.0 / 1920, 1.0 / 1080);
	float4 pos_pos;
	pos_pos.xy = pin.texcoord.xy;
	pos_pos.zw = pin.texcoord.xy - (rcp_frame * (0.5 + FXAA_SUBPIX_SHIFT));

	


	float4 sampled_color = 1.0;
	sampled_color = fxaa(texture_map, pos_pos, rcp_frame);
	sampled_color.a = 1;
	float mask_value = mask_map.Sample(sampler_states[0], pin.texcoord);
	//sampled_color.a *= mask_value;
	float alpha = step(cos(timer * 3.1415f / 180.0f), mask_value);
	//sampled_color.a *= alpha;
	return sampled_color;//texture_map.Sample(sampler_states[LINEAR], pin.texcoord);


}
#endif