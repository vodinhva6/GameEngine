#include "FullscreenQuad.hlsli"
#include "samplerStateSlotNum.hlsli"

SamplerState sampler_states[4] : register(s0);
SamplerComparisonState comparison_sampler_state : register(s4);
Texture2DArray shadow_map : register(t2);
TextureCube pointLightShadowmap : register(t3);


#define MAX_POISSON_DISC_SAMPLES 64
static const float2 poisson_samples[MAX_POISSON_DISC_SAMPLES] =
{
    float2(-0.5119625f, -0.4827938f),
	float2(-0.2171264f, -0.4768726f),
	float2(-0.7552931f, -0.2426507f),
	float2(-0.7136765f, -0.4496614f),
	float2(-0.5938849f, -0.6895654f),
	float2(-0.3148003f, -0.7047654f),
	float2(-0.42215f, -0.2024607f),
	float2(-0.9466816f, -0.2014508f),
	float2(-0.8409063f, -0.03465778f),
	float2(-0.6517572f, -0.07476326f),
	float2(-0.1041822f, -0.02521214f),
	float2(-0.3042712f, -0.02195431f),
	float2(-0.5082307f, 0.1079806f),
	float2(-0.08429877f, -0.2316298f),
	float2(-0.9879128f, 0.1113683f),
	float2(-0.3859636f, 0.3363545f),
	float2(-0.1925334f, 0.1787288f),
	float2(0.003256182f, 0.138135f),
	float2(-0.8706837f, 0.3010679f),
	float2(-0.6982038f, 0.1904326f),
	float2(0.1975043f, 0.2221317f),
	float2(0.1507788f, 0.4204168f),
	float2(0.3514056f, 0.09865579f),
	float2(0.1558783f, -0.08460935f),
	float2(-0.0684978f, 0.4461993f),
	float2(0.3780522f, 0.3478679f),
	float2(0.3956799f, -0.1469177f),
	float2(0.5838975f, 0.1054943f),
	float2(0.6155105f, 0.3245716f),
	float2(0.3928624f, -0.4417621f),
	float2(0.1749884f, -0.4202175f),
	float2(0.6813727f, -0.2424808f),
	float2(-0.6707711f, 0.4912741f),
	float2(0.0005130528f, -0.8058334f),
	float2(0.02703013f, -0.6010728f),
	float2(-0.1658188f, -0.9695674f),
	float2(0.4060591f, -0.7100726f),
	float2(0.7713396f, -0.4713659f),
	float2(0.573212f, -0.51544f),
	float2(-0.3448896f, -0.9046497f),
	float2(0.1268544f, -0.9874692f),
	float2(0.7418533f, -0.6667366f),
	float2(0.3492522f, 0.5924662f),
	float2(0.5679897f, 0.5343465f),
	float2(0.5663417f, 0.7708698f),
	float2(0.7375497f, 0.6691415f),
	float2(0.2271994f, -0.6163502f),
	float2(0.2312844f, 0.8725659f),
	float2(0.4216993f, 0.9002838f),
	float2(0.4262091f, -0.9013284f),
	float2(0.2001408f, -0.808381f),
	float2(0.149394f, 0.6650763f),
	float2(-0.09640376f, 0.9843736f),
	float2(0.7682328f, -0.07273844f),
	float2(0.04146584f, 0.8313184f),
	float2(0.9705266f, -0.1143304f),
	float2(0.9670017f, 0.1293385f),
	float2(0.9015037f, -0.3306949f),
	float2(-0.5085648f, 0.7534177f),
	float2(0.9055501f, 0.3758393f),
	float2(0.7599946f, 0.1809109f),
	float2(-0.2483695f, 0.7942952f),
	float2(-0.4241052f, 0.5581087f),
	float2(-0.1020106f, 0.6724468f),
};

float3 apply_shadow(inout float3 color, in float4 position_world_space, in float depth_view_space, in float2 shadow_map_dimensions, in float3 rand_seed)
{
    float shadow_factor = 0.0;
	
    CSMSHADOWMAP csmShadowMap = directionLightData.CSMShadowMap;
	// Find a layer of cascaded view frustum volume 
    int cascade_index = -1;
    for (uint layer = 0; layer < csmShadowMap.cascade_count; ++layer)
    {
        float distance = ((float[4]) (csmShadowMap.cascade_plane_distances[layer / 4]))[layer % 4];
        if (distance > depth_view_space)
        {
            cascade_index = layer;
            break;
        }
    }
	// outside far panel
    if (cascade_index == -1)
    {
        return color;
    }
   
	// world space to light view clip space, and to ndc
    float4 position_light_space = mul(position_world_space, csmShadowMap.view_projection_matrices[cascade_index]);
    position_light_space /= position_light_space.w;
	// ndc to texture space
    position_light_space.x = position_light_space.x * +0.5 + 0.5;
    position_light_space.y = position_light_space.y * -0.5 + 0.5;

#if 0
	// Hard shadows
	shadow_factor = shadow_map.SampleCmpLevelZero(comparison_sampler_state, float3(position_light_space.xy, cascade_index), position_light_space.z - effect_data.shadow_depth_bias).x;
#else
	// Soft shadows
    const float2 sample_scale = (0.5 * csmShadowMap.shadow_filter_radius) / shadow_map_dimensions;
    float accum = 0.0;
    for (uint sample_index = 0; sample_index < csmShadowMap.shadow_sample_count; ++sample_index)
    {
        float2 sample_offset;
        float4 seed = float4(rand_seed, sample_index);
        uint random = (uint) (64.0 * frac(sin(dot(seed, float4(12.9898, 78.233, 45.164, 94.673))) * 43758.5453)) % 64;
        sample_offset = poisson_samples[random] * sample_scale;

        float2 sample_position = position_light_space.xy + sample_offset;
        accum += shadow_map.SampleCmpLevelZero(comparison_sampler_state, float3(sample_position, cascade_index), position_light_space.z - csmShadowMap.shadow_depth_bias).x;
    }
    shadow_factor = accum / csmShadowMap.shadow_sample_count;
#endif
	
#if 0
    if (colorize_cascaded_layer)
    {
        const float3 colors[4] =
        {
            { 1, 0, 0 },
            { 0, 1, 0 },
            { 0, 0, 1 },
            { 1, 1, 0 },
        };
        return color * lerp(shadow_color, 1.0, shadow_factor) * colors[cascade_index];
    }
#endif
	
    return color * lerp(csmShadowMap.shadow_color, 1.0, shadow_factor);
}


float3 applyPointLightShadow(inout float3 color, in float4 position_world_space, in float depth_view_space, in float2 shadow_map_dimensions, in float3 rand_seed)
{
    BasicElement basicElement = pointLights[0].basicElement;
    PointLightShadowCastData pointLightShadowCastData = pointLights[0].shadowCastData;
    int slide = -1;
    float lengthLightToPixel = length(position_world_space.xyz - basicElement.position.xyz);
    float3 normalizedLightToPixel = normalize(position_world_space.xyz - basicElement.position.xyz);
    
    float dotXPos = dot(normalizedLightToPixel, float3(1.0, 0.0, 0.0));
    float dotXNeg = dot(normalizedLightToPixel, float3(-1.0, 0.0, 0.0));
    float dotYPos = dot(normalizedLightToPixel, float3(0.0, 1.0, 0.0));
    float dotYNeg = dot(normalizedLightToPixel, float3(0.0, -1.0, 0.0));
    float dotZPos = dot(normalizedLightToPixel, float3(0.0, 0.0, 1.0));
    float dotZNeg = dot(normalizedLightToPixel, float3(0.0, 0.0, -1.0));
    
    float maxDot = max(max(max(dotXPos, dotXNeg), max(dotYPos, dotYNeg)), max(dotZPos, dotZNeg));
    
    if (maxDot == dotXPos)
        slide = 0;
    else if (maxDot == dotXNeg)
        slide = 1;
    else if (maxDot == dotYPos)
        slide = 2;
    else if (maxDot == dotYNeg)
        slide = 3;
    else if (maxDot == dotZPos)
        slide = 4;
    else if (maxDot == dotZNeg)
        slide = 5;
    
    if (slide == -1)
        return color;
    
    
    float4 position_light_space = 0;
    position_light_space = mul(position_world_space, pointLightShadowCastData.view_projection_matrices[slide]);
    position_light_space /= position_light_space.w;
    position_light_space.x = position_light_space.x * +0.5 + 0.5;
    position_light_space.y = position_light_space.y * -0.5 + 0.5;
    
    float shadow_factor = 0.0;
    
    
    const float2 sample_scale = (0.5 * pointLightShadowCastData.shadow_filter_radius) / shadow_map_dimensions;
    float accum = 0.0;
    for (uint sample_index = 0; sample_index < pointLightShadowCastData.shadow_sample_count; ++sample_index)
    {
        float2 sample_offset;
        float4 seed = float4(rand_seed, sample_index);
        uint random = (uint) (64.0 * frac(sin(dot(seed, float4(12.9898, 78.233, 45.164, 94.673))) * 43758.5453)) % 64;
        sample_offset = poisson_samples[random] * sample_scale;

        float2 sample_position = normalize(position_light_space.xy + sample_offset);
        
        accum += pointLightShadowmap.SampleCmpLevelZero(comparison_sampler_state,
        //float3(sample_position, slide),
        normalizedLightToPixel,
        position_light_space.z - pointLightShadowCastData.shadow_depth_bias).x ;
        
        
    }
    shadow_factor = accum / pointLightShadowCastData.shadow_sample_count;

    return color * lerp(clamp(pointLightShadowCastData.shadow_color + lengthLightToPixel / pointLights[0].basicElement.range, 0, 1), 1.0, clamp(shadow_factor + lengthLightToPixel / pointLights[0].basicElement.range, 0, 1));
    
    
}


Texture2DMS<float, 4> colorDepthMap : register(t1);
Texture2D texture_map : register(t0);
float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = texture_map.Sample(sampler_states[POINT], pin.texcoord);
	
    uint width, height, number_of_samples;
    colorDepthMap.GetDimensions(width, height, number_of_samples);

    float depth_ndc;
    for (uint j = 0; j < number_of_samples; ++j)
    {
        depth_ndc += colorDepthMap.Load(uint2(pin.texcoord.x * width, pin.texcoord.y * height), j).r;
    }
    depth_ndc /= number_of_samples;
	
    uint2 shadow_map_dimensions;
    uint shadow_map_mip_level = 0, shadow_map_number_of_samples, levels;
    shadow_map.GetDimensions(shadow_map_mip_level, shadow_map_dimensions.x, shadow_map_dimensions.y, shadow_map_number_of_samples, levels);

   
    
    float4 position_ndc;
	// texture space to ndc
    position_ndc.x = pin.texcoord.x * +2 - 1;
    position_ndc.y = pin.texcoord.y * -2 + 1;
    position_ndc.z = depth_ndc;
    position_ndc.w = 1;

    float4 position_world_space = mul(position_ndc, view_projectionNowInvers);
    position_world_space = position_world_space / position_world_space.w;
	
    float4 position_view_space = mul(position_ndc, projectionNowInvers);
    position_view_space = position_view_space / position_view_space.w;
	
	
    color.rgb = apply_shadow(color.rgb, position_world_space, position_view_space.z, shadow_map_dimensions, position_ndc.xyz);
	
    uint2 pointLight_shadow_map_dimensions;
    uint pointLight_shadow_map_mip_level = 0, levelss = 0, shadow_map_number_of_sampless;
    pointLightShadowmap.GetDimensions(pointLight_shadow_map_mip_level, pointLight_shadow_map_dimensions.x, pointLight_shadow_map_dimensions.y,levelss);
    color.rgb = applyPointLightShadow(color.rgb, position_world_space, position_view_space.z, pointLight_shadow_map_dimensions, position_ndc.xyz);
    
 
    
	
    return color;
}