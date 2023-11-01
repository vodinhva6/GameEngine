#include "geometric_primitive.hlsli"
VS_OUT main(float4 position : POSITION, float4 normal : NORMAL, in uint vertexid : SV_VERTEXID)
{
    VS_OUT vout;
    vout.position = mul(position, mul(world, debugView_projection));
    vout.normal = 1;
    vout.color = 1;
    const float2 texcoords[4] =
    {
        {0,0},{1,0},{0,1},{1,1} ,
    };
    vout.texcoord = texcoords[vertexid % 4];
    return vout;
}
