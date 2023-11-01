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
    float alpha = color.a;
    float4 colorOut = color * pin.color;
    //colorOut.a = alpha;
    //
    //float3 N = normalize(pin.world_normal.xyz);
    //float3 T = normalize(pin.world_tangent.xyz);
    //float sigma = pin.world_tangent.w;
    //T = normalize(T - dot(N, T));
    //float3 B = normalize(cross(N, T) * sigma);
    //
    //float4 normal = texture_maps[1].Sample(sampler_states[LINEAR], pin.texcoord);
    //normal = (normal * 2.0) - 1.0;
    //N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));
    //
    //
    //
    //float3 L = normalize(light_direction.xyz);
    //float3 diffuse = ClacHalfLambert(N, L, light_color.rbg, color.rgb);
    //float3 V = normalize(/*camera_position.xyz -*/ pin.world_position.xyz);
    //float3 specular = pow(max(0, dot(N, normalize(V - L))), 200);
    //float3 am = { 0.3f,0.3f,0.3f };
    //specular *= am;
    //
    //return 1;
    //float depth = shadow_map.Sample(sampler_states[SHADOW], pin.shadow_texcoord.xy).r;
    //if(depth > gamma)
    //    return 1;
    //return colorOut;
    //colorOut = float4(diffuse/* + specular*/, alpha) * pin.color;
    //
    //if (pin.shadow_texcoord.z - depth > bias)
    //{
    //    colorOut.rgb *= 0.7f;
    //}
    //float3 point_diffuse = 0;
    ////float3 point_specular = 0;
    //for (int i = 0; i < pointLightsUsingAt; i++)
    //{
    //    float3 LP = pin.world_position.xyz - point_lights[i].position.xyz;
    //    float len = length(LP);
    //    if (len >= point_lights[i].range)
    //        continue;
    //    float attenuateLength = saturate(1.0f - len / point_lights[i].range);
    //    float attenuation = attenuateLength * attenuateLength;
    //    LP /= len;
    //    point_diffuse += (CalcLambert(N, LP, point_lights[i].color.rgb, colorOut.rgb) * attenuation) * point_lights[i].power;
    //    //point_specular += CalcPhongSpecular(N, LP, E, point_lights[i].color.rgb, ks.rgb) * attenuation;
    //}
    //float3 spot_diffuse = 0;
    ////float3 spot_specular = 0;
    //for (int j = 0; j < spotLightsUsingAt; ++j)
    //{
    //    float3 LP = pin.world_position.xyz - spot_lights[j].position.xyz;
    //    float len = length(LP);
    //    if (len >= spot_lights[j].range)
    //        continue;
    //    float attenuateLength = saturate(1.0f - len / spot_lights[j].range);
    //    float attenuation = attenuateLength * attenuateLength;
    //    LP /= len;
    //    float3 spotDirection = normalize(spot_lights[j].direction.xyz);
    //    float angle = dot(spotDirection, LP);
    //    float area = spot_lights[j].innerCorn - spot_lights[j].outerCorn;
    //    attenuation *= saturate(1.0f - (spot_lights[j].innerCorn - angle) / area);
    //    spot_diffuse += CalcLambert(N, LP, spot_lights[j].color.rgb, color.rgb) * attenuation * spot_lights[j].power;
    //    //spot_specular += CalcPhongSpecular(N, LP, E, spot_lights[j].color.rgb, ks.rgb) * attenuation;
    //}



    colorOut.rgb = RGB2HSV(colorOut.rgb /*+ point_diffuse + spot_diffuse*/);
    colorOut.r += hueShift;
    colorOut.g *= saturation;
    colorOut.b *= brightness;
    colorOut.rgb = HSV2RGB(colorOut.rgb);

    colorOut = CalcFog(colorOut, fogColor, fogRange.xy, 1);
    //clip(colorOut.a < 0.5f ? -1:1);
    return colorOut;

}