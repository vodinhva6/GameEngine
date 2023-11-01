#include "lights.hlsli"
#include "ColorFilterFunctions.hlsli"
#include "ShadingFunctions.hlsli"

cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 view_projectionNow;
};

cbuffer COLOR_FILTER : register(b2)
{
    float hueShift;
    float saturation;
    float brightness;
    float gamma;
};
cbuffer LIGHT_CONSTANT_BUFFER : register(b4)
{
    float4 light_direction;
    float4 light_color;

    PointLights point_lights[20];
    SpotLights	spot_lights[20];
    int pointLightsUsingAt;
    int spotLightsUsingAt;
    float2 dummyLights;
}
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
    float3 shadow_texcoord : TEXCOORD1;
};
static const int MAX_BONES = 256;
cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;
    float4 material_color;
    row_major float4x4 bone_transforms[MAX_BONES];
};
