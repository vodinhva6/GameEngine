#include "ColorFilterFunctions.hlsli"
//--------------------------------------------
// �����o�[�g�g�U���ˌv�Z�֐�
//--------------------------------------------
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// K:���˗�
float3 CalcLambert(float3 N, float3 L, float3 C, float3 K)
{
    float power = saturate(dot(N, -L));
    return C * power * K;
}
//--------------------------------------------
// �t�H���̋��ʔ��ˌv�Z�֐�
//--------------------------------------------
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// E:�����x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// K:���˗�
float3 CalcPhongSpecular(float3 N, float3 L, float3 E, float3 C, float3 K)
{
    float3 R = reflect(L, N);
    float power = max(dot(E, R), 0);
    power = pow(power, 100);
    return C * power * K;
}
//--------------------------------------------
// �n�[�t�����o�[�g�g�U���ˌv�Z�֐�
//--------------------------------------------
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// K:���˗�
float3 ClacHalfLambert(float3 N, float3 L, float3 C, float3 K)
{
    float D = saturate(dot(N, -L) * 0.5f + 0.5f);
    return C * D * K;
}
float3 ClacHalfLambert(float3 N, float3 L, float3 C, float3 K, float A, float B)
{
    float D = saturate(dot(N, -L) * A + B);
    return C * D * K;
}
//--------------------------------------------
// �t�H�O
//--------------------------------------------
//color:���݂̃s�N�Z���F
//fog_color:�t�H�O�̐F
//fog_range:�t�H�O�͈̔͏��
//eye_length:���_����̋���
float4 CalcFog(in float4 color, float4 fog_color, float2 fog_range, float eye_length)
{
    float fogAlpha = saturate((eye_length - fog_range.x) / (fog_range.y - fog_range.x));
    return lerp(color, fog_color, fogAlpha);
}

// ���}�b�s���O 
//-------------------------------------------- 
// tex:�����v�V�F�[�f�B���O�p�e�N�X�`�� 
// samp:�����v�V�F�[�f�B���O�p�T���v���X�e�[�g 
// color: ���݂̃s�N�Z���F 
// N:�@��(���K���ς�) 
// C:���ˌ�(�F�E����) 
// value:�K���� 

float3 CalcSphereEnvironment(Texture2D tex, SamplerState samp, in float3 color, float3 N, float3 E, float value)
{

    float3 R = reflect(E, N);

    float2 texcoord = R.xy * 0.5f + 0.5f;

    return	lerp(color.rgb, tex.Sample(samp, texcoord).rgb, value);

}

float3 CalcSphereEnvironment(Texture2D tex, SamplerState samp, in float3 color, float3 R, float value)
{
    const float PI = 3.14159265358979;
    R = normalize(R);
    float2 texcoord;
    texcoord.x = (atan2(R.z, R.x) + PI) / (PI * 2.0);
    texcoord.y = 1.0 - ((asin(R.y) + PI * 0.5) / PI);

    uint width, height, number_of_levels;
    tex.GetDimensions(0, width, height, number_of_levels);
    float roughness = 0;
    float lod = roughness * float(number_of_levels - 1);

    return	lerp(color.rgb, tex.SampleLevel(samp, texcoord, lod).rgb, value);

}


float4 GetColorFromTexture2D(Texture2D map, SamplerState samplerState, float2 texcoord)
{
    float4 color = map.Sample(samplerState, texcoord);
    const float GAMMA = 2.2;
    //color.rgb = pow(abs(color.rgb), GAMMA);

    return color;
}


float4 GetColorLevelFromTexture2D(Texture2D map, SamplerState samplerState, float2 texcoord, float level)
{
    
    float4 color = map.SampleLevel(samplerState, texcoord, level);
    const float GAMMA = 2.2;
    //color.rgb = pow(abs(color.rgb), GAMMA);

    return color;
}


float4 ColorFilter(float4 color)
{
   color.rgb = RGB2HSV(color.rgb);
   color.r += hueShift;
   color.g *= saturation;
   color.b *= brightness;
   color.b = pow(color.b, 1 / gamma);
   color.rgb = HSV2RGB(color.rgb);
   return color;
}


float3 ReinhardToneMapping(float3 color)
{
    float luma = dot(color, float3(0.2126, 0.7152, 0.0722));
    float tone_mapped_luma = luma / (1. + luma);
    color *= tone_mapped_luma / luma;
    return color;
}
//#define N = 10
//float4 GPUImportanceSampling(float3 viewing : TEXCOORD1, uniform sampler2D env) : COLOR
//{ 
//    float4 c = 0;   
//    // Sample loop    
//    for (int k=0; k < N; k++) 
//    {     
//        float2 xi = hammersley_seq(k);     
//        float3 u = sample_material(xi);     
//        float pdf = p(u, viewing);     
//        float lod = compute_lod(u, pdf);     
//        float2 texcoord = convert_to_dual_paraboloid(u);     
//        float3 L = tex2Dlod(env, float4(texcoord, lod.xy));     
//        c += L*f(u,viewing)/pdf;   
//    }   
//    return c/N; 
//} 