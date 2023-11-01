#include "skinned_mesh.hlsli"
#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
#define LINEARCLAMP 3
#define SHADOW 4
SamplerState sampler_states[5] : register(s0);
Texture2D texture_maps[4] : register(t0);
Texture2D shadow_map : register(t4);
Texture2D DepthMap : register(t5);


float4 main(VS_OUT pin) : SV_TARGET
{
   float4 color = texture_maps[0].Sample(sampler_states[ANISOTROPIC], pin.texcoord);
   
   
   float4 colorOut = color * pin.color;
   
   float3 N = normalize(pin.world_normal.xyz);
   float3 T = normalize(pin.world_tangent.xyz);
   float sigma = pin.world_tangent.w;
   T = normalize(T - dot(N, T));
   float3 B = normalize(cross(N, T) * sigma);
   
   float4 normal = texture_maps[1].Sample(sampler_states[LINEAR], pin.texcoord);
   normal = (normal * 2.0) - 1.0;
   N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));
   
   
   float3 lightDir = { 0.5,0.5,0.5 };
   float3 L = normalize(-lightDir.xyz);
   float3 diffuse = ClacHalfLambert(N, L, directionLightData.light_color.rbg, color.rgb, 0.6,0.2);
   float3 V = normalize(pin.world_position.xyz);
   float3 specular = pow(max(0, dot(N, normalize(V - L))), 200);
   float3 am = { 0.3f,0.3f,0.3f };
   specular *= am;
   
   
   colorOut.rgb *= diffuse + specular;
   colorOut = ColorFilter(colorOut);
   return colorOut;
}