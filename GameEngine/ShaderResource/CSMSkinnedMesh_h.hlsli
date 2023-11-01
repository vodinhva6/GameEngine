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
    
struct VS_OUTTO_GS_CSM
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD;
    uint slice : SLICE;
};

struct GS_OUTTO_PS_CSM
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float depth : DEPTH;
    uint slice : SV_RENDERTARGETARRAYINDEX;
};

static const int MAX_BONES = 256;
cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;
    row_major float4x4 bone_transforms[MAX_BONES];
    float4 ka;
    float4 kd;
    float4 ks;
    float occlusionStrength;
    float smoothness;
    float2 dummyObjConstant;
};
