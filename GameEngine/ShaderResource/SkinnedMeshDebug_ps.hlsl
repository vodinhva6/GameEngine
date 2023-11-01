#include "skinned_mesh.hlsli"
#include "bidirectional_reflectance_distribution_function.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
#define LINEARCLAMP 3
#define SHADOW 4




#define COLORMAP 0
#define NORMALMAP 1
#define EMISSIVEMAP 2
#define METALLICMAP 3
#define OCCLUSIONMAP 4




SamplerState sampler_states[5] : register(s0);
Texture2D texture_maps[5] : register(t0);
Texture2D environmentMap : register(t4);
Texture2D shadow_map : register(t5);
Texture2D DepthMap : register(t6);


float4 main(VS_OUT pin) : SV_TARGET
{
    float4 emmisiveColor = GetColorFromTexture2D(texture_maps[EMISSIVEMAP], sampler_states[ANISOTROPIC], pin.texcoord);
    float occlusion = GetColorFromTexture2D(texture_maps[OCCLUSIONMAP], sampler_states[ANISOTROPIC], pin.texcoord).r;
    float4 textureColor = GetColorFromTexture2D(texture_maps[COLORMAP], sampler_states[ANISOTROPIC], pin.texcoord);
    float4 normalColor = GetColorFromTexture2D(texture_maps[NORMALMAP], sampler_states[POINT], pin.texcoord);
    float4 metallicColor = GetColorFromTexture2D(texture_maps[METALLICMAP], sampler_states[ANISOTROPIC], pin.texcoord);
    metallicColor.rgb *= metallicColor.a;
    
    textureColor = textureColor * pin.color;
    float roughness = smoothness;
    float metallic = metallicColor.b * metallicFactor;
    
    
    float3 L = normalize(-directionLightData.light_direction.xyz);
    float3 E = normalize(debugCameraPosition.xyz - pin.world_position.xyz);
    float3 N = normalize(pin.world_normal.xyz);
    float3 T = normalize(pin.world_tangent.xyz);
    float sigma = pin.world_tangent.w;
    T = normalize(T - dot(N, T));
    float3 B = normalize(cross(N, T) * sigma);
    float4 normal = texture_maps[NORMALMAP].Sample(sampler_states[LINEAR], pin.texcoord);
    normal = (normal * 2.0) - 1.0;
    N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));
    
    
   
    
    
    const float3 f0 = lerp(0.04, textureColor.rgb, metallic);
    const float3 f90 = 1.0f;
    const float alphaRoughness = roughness * roughness;
    const float3 c_Diff = lerp(textureColor.rgb, 0.0, metallic);
    
    float3 Li = float3(1.0, 1.0, 1.0); // Radiance of the light
    const float NoL = max(0.0, dot(N, L));
    const float NoV = max(0.0, dot(N, E));
    
    float3 specular = 0;
    float3 diffuse  = 0;
    
    if (NoL > 0.0 || NoV > 0.0)
    {
        const float3 R = reflect(-L, N);
        const float3 H = normalize(E + L);
        
        const float NoH = max(0.0, dot(N, H));
        const float HoV = max(0.0, dot(H, E));
        
        diffuse +=  Li * NoL * brdf_lambertian(f0, f90, c_Diff, HoV);
        specular += Li * NoL * brdf_specular_ggx(f0, f90, alphaRoughness, HoV, NoL, NoV, NoH) * ks.rgb;
        
    }
    diffuse += ibl_radiance_lambertian(sampler_states[LINEAR], N, E, roughness, c_Diff, f0);
    specular += ibl_radiance_ggx(sampler_states[LINEAR], N, E, roughness, f0) * ks.rgb;
    
    
   
    
    float3 point_diffuse = 0;
    float3 point_specular = 0;
  
    for (int i = 0; i < pointLightsUsingAt; i++)
    {
        BasicElement basicElement = pointLights[i].basicElement;
        float3 LP = pin.world_position.xyz - basicElement.position.xyz;
        float len = length(LP);
        if (len >= basicElement.range)
            continue;
        float attenuateLength = saturate(1.0f - len / basicElement.range);
        float attenuation = attenuateLength * attenuateLength;
        LP /= len;
        point_diffuse += (CalcLambert(N, LP, basicElement.color.rgb, diffuse) * attenuation) * basicElement.power;
        point_specular += CalcPhongSpecular(N, LP, E, basicElement.color.rgb, diffuse) * attenuation;
    }
    
    diffuse += point_diffuse;
    specular += point_specular;
    
    diffuse = lerp(diffuse, diffuse * occlusion, occlusionStrength);
    specular = lerp(specular, specular * occlusion, occlusionStrength);
  
    
    //float3 R = reflect(E, N);
    //diffuse.rgb = CalcSphereEnvironment(environmentMap, sampler_states[LINEAR], diffuse.rgb, R.rgb, environmentValue) * ka.rgb;
    float4 colorOut = float4((diffuse + specular + emmisiveColor.rgb) * textureColor.rgb, textureColor.a);
    //float4 colorOut = float4(((diffuse + point_diffuse) + specular + emmisiveColor.rgb) * occlusion, textureColor.a);
    colorOut = ColorFilter(colorOut);
    return colorOut;
}