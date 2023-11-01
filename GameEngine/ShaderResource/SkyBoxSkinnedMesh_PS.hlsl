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
    //float alpha = color.a;
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
    //
    //colorOut = float4(diffuse/* + specular*/, alpha) * pin.color;
    //
    //
    //float3 point_diffuse = 0;
    //float3 spot_diffuse = 0;



    //colorOut.rgb = RGB2HSV(colorOut.rgb );
    //colorOut.r += hueShift;
    //colorOut.g *= saturation;
    //colorOut.b *= brightness;
    //colorOut.rgb = HSV2RGB(colorOut.rgb);
    //
    //colorOut = CalcFog(colorOut, fogColor, fogRange.xy, length(pin.world_position.xyz /*- camera_position.xyz*/));
    //clip(colorOut.a < 0.5f ? -1:1);
    return colorOut;

}