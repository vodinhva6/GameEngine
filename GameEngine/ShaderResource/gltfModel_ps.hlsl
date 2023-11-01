#include "gltfModel_h.hlsli"
#include "bidirectional_reflectance_distribution_function.hlsli"
#define BASECOLOR_TEXTURE 0
#define METALLIC_ROUGHNESS_TEXTURE 1
#define NORMAL_TEXTURE 2
#define EMISSIVE_TEXTURE 3
#define OCCLUSION_TEXTURE 4
Texture2D<float4> material_textures[5] : register(t1);

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState sampler_states[3] : register(s0);



float4 main(VS_OUT pin) : SV_TARGET
{
    material_constants mat = materials[material];
    
    float4 baseColorFactor = mat.pbr_metallic_roughness.basecolor_factor;
    const int baseColorTexture = mat.pbr_metallic_roughness.basecolor_texture.index;
    if (baseColorTexture > -1)
    {
        float4 sampled = GetColorFromTexture2D(material_textures[BASECOLOR_TEXTURE], sampler_states[ANISOTROPIC], pin.texcoord);
        baseColorFactor *= sampled;
    }
    float3 emmisiveFactor = mat.emissive_factor;
    const int emissiveTexture = mat.emissive_texture.index;
    if (emissiveTexture > -1)
    {
     
        float4 sampled = GetColorFromTexture2D(material_textures[EMISSIVE_TEXTURE], sampler_states[ANISOTROPIC], pin.texcoord);
        emmisiveFactor *= sampled.rgb;
    }

    float roughnessFactor = mat.pbr_metallic_roughness.roughness_factor;
    float metallicFactor = mat.pbr_metallic_roughness.metallic_factor;
    const int metallicRoughnessTexture = mat.pbr_metallic_roughness.metallic_roughness_texture.index;
    if (metallicRoughnessTexture > -1)
    {
        float4 sampled = GetColorFromTexture2D(material_textures[METALLIC_ROUGHNESS_TEXTURE], sampler_states[LINEAR], pin.texcoord);
        roughnessFactor *= sampled.g;
        metallicFactor *= sampled.b;
    }

    float occlusionFactor = 1.0;
    const int occlusionTexture = mat.occlusion_texture.index;
    if (occlusionTexture > -1)
    {
        float4 sampled = GetColorFromTexture2D(material_textures[OCCLUSION_TEXTURE], sampler_states[LINEAR], pin.texcoord);
        occlusionFactor *= sampled.r;
    }

    const float occlusionStrength = mat.occlusion_texture.strength;

    const float3 f0 = lerp(0.04, baseColorFactor.rgb, metallicFactor);
    const float3 f90 = 1.0f;
    const float alphaRoughness = roughnessFactor * roughnessFactor;
    const float3 c_Diff = lerp(baseColorFactor.rgb, 0.0, metallicFactor);

    const float3 P = pin.world_position.xyz;
    const float3 V = normalize(gameCameraPosition.xyz - pin.world_position.xyz);

    float3 N = normalize(pin.world_normal.xyz);
    float3 T = has_tangent ? normalize(pin.world_tangent.xyz) : float3(1, 0, 0);
    float sigma = has_tangent ? pin.world_tangent.w : 1.0f;
    T = normalize(T - N * dot(N, T));
    float3 B = normalize(cross(N, T) * sigma);

    const int normal_texture = mat.normal_texture.index;
    if (normal_texture > -1)
    {
        float4 sampled = GetColorFromTexture2D(material_textures[NORMAL_TEXTURE], sampler_states[LINEAR], pin.texcoord);
        float3 normal_factor = sampled.xyz;
        normal_factor = (normal_factor * 2.0) - 1.0;
        normal_factor = normalize(normal_factor * float3(mat.normal_texture.scale, mat.normal_texture.scale, 1.0));
        N = normalize((normal_factor.x * T) + (normal_factor.y * B) + (normal_factor.z * N));
    }

    float3 diffuse = 0;
    float3 specular = 0;

    float3 L = normalize(-directionLightData.light_direction.xyz);
    float3 Li = float3(1.0, 1.0, 1.0); // Radiance of the light
    const float NoL = max(0.0, dot(N, L));
    const float NoV = max(0.0, dot(N, V));
    if (NoL > 0.0 || NoV > 0.0)
    {
        const float3 R = reflect(-L, N);
        const float3 H = normalize(V + L);
        
        const float NoH = max(0.0, dot(N, H));
        const float HoV = max(0.0, dot(H, V));
        
        diffuse += Li * NoL * brdf_lambertian(f0, f90, c_Diff, HoV);
        specular += Li * NoL * brdf_specular_ggx(f0, f90, alphaRoughness, HoV, NoL, NoV, NoH);
        
    }

    diffuse += ibl_radiance_lambertian(sampler_states[LINEAR], N, V, roughnessFactor, c_Diff, f0);
    specular += ibl_radiance_ggx(sampler_states[LINEAR], N, V, roughnessFactor, f0);
    float3 emmisive = emmisiveFactor;
    diffuse = lerp(diffuse, diffuse * occlusionFactor, occlusionStrength);
    specular = lerp(specular, specular * occlusionFactor, occlusionStrength);
    
    float3 Lo = diffuse + specular + emmisive;
    float4 colorOut = ColorFilter(float4(Lo, baseColorFactor.a));
    
    return colorOut;
}

 