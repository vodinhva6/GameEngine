#include "ConstantBuffer.hlsli"
struct VS_IN
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float2 texcoord : TEXCOORD;
    float4 bone_weights : WEIGHTS;
    uint4 bone_indices : BONES;
};
struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 world_position : POSITION;
    float4 world_normal : NORMAL;
    float4 world_tangent : TANGENT;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};
static const int MAX_BONES = 512;
cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;
    row_major float4x4 bone_transforms[MAX_BONES];
    float4 ka;
    float4 kd;
    float4 ks;
    float occlusionStrength;
    float smoothness;
    float metallicFactor;
    float dummyObjConstant;
};
