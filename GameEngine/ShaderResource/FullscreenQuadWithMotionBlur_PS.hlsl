#include "FullscreenQuad.hlsli"
#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
#define LINEARCLAMP 3
#define SHADOW 4
SamplerState sampler_states[5] : register(s0);
Texture2DMS<float,4> DepthMap : register(t1);
Texture2D SceneSample : register(t0);

float4 main(VS_OUT pin) : SV_TARGET
{
    uint width, height, number_of_samples;
    DepthMap.GetDimensions(width, height, number_of_samples);

    float zOverW;
    for (uint j = 0; j < number_of_samples; ++j)
    {
        zOverW += DepthMap.Load(uint2(pin.texcoord.x * width, pin.texcoord.y * height), j).r;
    }
    zOverW /= number_of_samples;
   
    float4 H = float4(pin.texcoord.x * 2 - 1, (1 - pin.texcoord.y) * 2 - 1, zOverW, 1);
    float4 D = mul(H, view_projectionNowInvers);
    float4 worldPos = D / D.w;
    float4 currentPos = H;
    float4 previousPos = mul(worldPos, view_projectionOld);
    previousPos /= previousPos.w;
    float2 velocity = (currentPos - previousPos).xy / motisonBlurSample;
    velocity.y *= -1;
    float2 texCoord = pin.texcoord.xy;
    float4 colorb = SceneSample.Sample(sampler_states[LINEAR], texCoord);
    texCoord += velocity;
    for (int i = 1; i < motisonBlurSample; ++i, texCoord += velocity)
    { 
        float3 currentColor = SceneSample.Sample(sampler_states[LINEAR], texCoord).rgb;
        colorb.rgb += currentColor;
    } 
    return float4(colorb.rgb / motisonBlurSample, colorb.a);
}
