#include "ColorFilterFunctions.hlsli"
struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};
cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;
    float4 material_color;
};
cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 view_projectionNow;
    row_major float4x4 debugView_projection;
    row_major float4x4 view_projectionNowInvers;
    row_major float4x4 view_projectionOld;
    float4 camera_position;
    int motisonBlurSample;
    float3 dummy0;
};
cbuffer TEXTURE_CUSTOM_CONSTANT_BUFFER : register(b5)
{
    float2 scroll;
    float timer;
    float scroll_dummy;
}


