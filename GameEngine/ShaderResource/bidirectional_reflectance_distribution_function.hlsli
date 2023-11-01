#ifndef __IMAGEBASEDLIGHTING__
#define __IMAGEBASEDLIGHTING__
TextureCube diffuse_iem : register(t33);
TextureCube specular_pmrem : register(t34);
Texture2D lut_ggx : register(t35);
Texture2D skybox : register(t32);

float4 sample_lut_ggx(SamplerState samplerState, float2 brdf_sample_point)
{
    return lut_ggx.Sample(samplerState, brdf_sample_point);
}
float4 sample_skybox(SamplerState samplerState, float3 v, float roughness)
{
    const float PI = 3.14159265358979;
    uint width, height, number_of_levels;
    skybox.GetDimensions(0, width, height, number_of_levels);

    float lod = roughness * float(number_of_levels - 1);

    v = normalize(v);
    // Blinn/Newell Latitude Mapping
    float2 sample_point;
    sample_point.x = (atan2(v.z, v.x) + PI) / (PI * 2.0);
    sample_point.y = 1.0 - ((asin(v.y) + PI * 0.5) / PI);
    return skybox.SampleLevel(samplerState, sample_point, lod);
}
float4 sample_diffuse_iem(SamplerState samplerState, float3 v)
{
    return diffuse_iem.Sample(samplerState, v);
}
float4 sample_specular_pmrem(SamplerState samplerState, float3 v, float roughness)
{
    uint width, height, number_of_levels;
    specular_pmrem.GetDimensions(0, width, height, number_of_levels);

    float lod = roughness * float(number_of_levels - 1);

    return specular_pmrem.SampleLevel(samplerState, v, lod);
}

#endif


#ifndef __BIDIRECTIONAL_REFLECTANCE_DISTRIBUTION_FUNCTION_HLSL__
#define __BIDIRECTIONAL_REFLECTANCE_DISTRIBUTION_FUNCTION_HLSL__


float4x4 inverse(float4x4 m)
{
    float4 c0 = m[0];
    float4 c1 = m[1];
    float4 c2 = m[2];
    float4 c3 = m[3];

    float4 r0 = c2.yzww * c3.wwwy - c2.wwwy * c3.yzww;
    float4 r1 = c2.wwwy * c3.xxyy - c2.xxyy * c3.wwwy;
    float4 r2 = c1.yzww * c3.wwwy - c1.wwwy * c3.yzww;
    float4 r3 = c1.wwwy * c3.xxyy - c1.xxyy * c3.wwwy;

    float4 d = c1.xxyy * r0 - c1.yzww * r1 + c2.xxyy * r2 - c2.yzww * r3;
    float det = dot(c0, d);

    float4 s0 = c1.xxyy * c2.wwwy - c1.wwwy * c2.xxyy;
    float4 s1 = c1.wwwy * c2.yzww - c1.yzww * c2.wwwy;
    float4 s2 = c0.yzww * c2.xxyy - c0.xxyy * c2.yzww;
    float4 s3 = c0.wwwy * c2.yzww - c0.yzww * c2.wwwy;

    float4x4 inv;
    inv[0] = d / det;
    inv[1] = -(c0.yzww * r0 - c0.wwwy * r1 + c2.wwwy * r3) / det;
    inv[2] = (c0.xxyy * r0 - c0.wwwy * r2 + c1.wwwy * r3) / det;
    inv[3] = -(c0.xxyy * r1 - c0.yzww * r2 + c1.yzww * r3) / det;
    return inv;
}

float3 f_schlick(float3 f0, float3 f90, float VoH)
{
    return f0 + (f90 - f0) * pow(clamp(1.0 - VoH, 0.0, 1.0), 5.0);
}
float v_ggx(float NoL, float NoV, float alpha_roughness)
{
    float alpha_roughness_sq = alpha_roughness * alpha_roughness;

    float ggxv = NoL * sqrt(NoV * NoV * (1.0 - alpha_roughness_sq) + alpha_roughness_sq);
    float ggxl = NoV * sqrt(NoL * NoL * (1.0 - alpha_roughness_sq) + alpha_roughness_sq);

    float ggx = ggxv + ggxl;
    return (ggx > 0.0) ? 0.5 / ggx : 0.0;
}
float d_ggx(float NoH, float alpha_roughness)
{
    const float PI = 3.14159265358979;
    float alpha_roughness_sq = alpha_roughness * alpha_roughness;
    float f = (NoH * NoH) * (alpha_roughness_sq - 1.0) + 1.0;
    return alpha_roughness_sq / (PI * f * f);
}
float3 brdf_lambertian(float3 f0, float3 f90, float3 diffuse_color, float VoH)
{
    const float PI = 3.14159265358979;
    return (1.0 - f_schlick(f0, f90, VoH)) * (diffuse_color / PI);
}
float3 brdf_specular_ggx(float3 f0, float3 f90, float alpha_roughness,
    float VoH, float NoL, float NoV, float NoH)
{
    float3 F = f_schlick(f0, f90, VoH);
    float Vis = v_ggx(NoL, NoV, alpha_roughness);
    float D = d_ggx(NoH, alpha_roughness);

    return F * Vis * D;
}


float3 ibl_radiance_lambertian(SamplerState samplerState, float3 N, float3 V, float roughness, float3 diffuse_color, float3 f0)
{
    float NoV = clamp(dot(N, V), 0.0, 1.0);

    float2 brdf_sample_point = clamp(float2(NoV, roughness), 0.0, 1.0);
    float2 f_ab = sample_lut_ggx(samplerState, brdf_sample_point).rg;

    float3 irradiance = sample_diffuse_iem(samplerState, N).rgb;

    float3 fr = max(1.0 - roughness, f0) - f0;
    float3 k_s = f0 + fr * pow(1.0 - NoV, 5.0);
    float3 fss_ess = k_s * f_ab.x + f_ab.y;

    float ems = (1.0 - (f_ab.x + f_ab.y));
    float3 f_avg = (f0 + (1.0 - f0) / 21.0);
    float3 fms_ems = ems * fss_ess * f_avg / (1.0 - f_avg * ems);
    float3 k_d = diffuse_color * (1.0 - fss_ess + fms_ems);

    return (fms_ems + k_d) * irradiance;
}


float3 ibl_radiance_ggx(SamplerState samplerState, float3 N, float3 V, float roughness, float3 f0)
{
    float NoV = clamp(dot(N, V), 0.0, 1.0);

    float2 brdf_sample_point = clamp(float2(NoV, roughness), 0.0, 1.0);
    float2 f_ab = sample_lut_ggx(samplerState, brdf_sample_point).rg;

    float3 R = normalize(reflect(-V, N));
    float3 specular_light = sample_specular_pmrem(samplerState, R, roughness).rgb;

    float3 fr = max(1.0 - roughness, f0) - f0;
    float3 k_s = f0 + fr * pow(1.0 - NoV, 5.0);
    float3 fss_ess = k_s * f_ab.x + f_ab.y;

    return specular_light * fss_ess;
}

#endif
