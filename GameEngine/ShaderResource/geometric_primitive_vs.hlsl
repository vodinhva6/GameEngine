#include "geometric_primitive.hlsli"
VS_OUT main(float4 position : POSITION, float4 normal : NORMAL)
{
    VS_OUT vout;
    vout.position = mul(position, mul(world, view_projectionNow));

    normal.w = 0;
    float4 N = normalize(mul(normal, world));
    float4 L = normalize(-directionLightData.light_direction);
    vout.normal = normal;
    vout.color.rgb = material_color.rgb * max(0, dot(L, N));
    vout.color.a = material_color.a;
    vout.texcoord = 1;
    return vout;
}
