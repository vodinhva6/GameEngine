#include "geometric_primitive.hlsli"
float4 main(VS_OUT pin) : SV_TARGET
{
    pin.color *= material_color;
    return pin.color;
}