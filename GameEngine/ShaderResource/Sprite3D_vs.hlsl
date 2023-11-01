#include "Sprite3D.hlsli"
VS_OUT main(float4 position : POSITION, float2 texcoord : TEXCOORD)
{
    VS_OUT vout;
    vout.position = mul(position, mul(world, debugView_projection));
    vout.texcoord = texcoord;
    return vout;
}