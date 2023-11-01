
struct CSMSHADOWMAP
{
    row_major float4x4 view_projection_matrices[16];
    float4 cascade_plane_distances[4];
    
    uint cascade_count;
    float shadow_depth_bias;
    float shadow_color;
    float shadow_filter_radius;
    
    uint shadow_sample_count;
    uint colorize_cascaded_layer;
    float2 csmDummy;
};



struct PointLightShadowCastData
{
    row_major float4x4 view_projection_matrices[6];
    
    uint cascade_count;
    float distance;
    float shadow_depth_bias;
    float shadow_color;
    
    float shadow_filter_radius;
    uint shadow_sample_count;
    float2 pdummy;
};