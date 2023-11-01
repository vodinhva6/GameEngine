Texture2DMS<float> depth_map : register(t0);
float main(float4 position : SV_POSITION, float2 texcoord : TEXCOORD0) : SV_DEPTH
{
    uint width, height, number_of_samples;
    depth_map.GetDimensions(width, height, number_of_samples);
    return depth_map.Load(uint2(texcoord.x * width, texcoord.y * height), 0);
}
