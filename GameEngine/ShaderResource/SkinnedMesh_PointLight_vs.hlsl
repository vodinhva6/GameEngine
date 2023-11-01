#include "CSMSkinnedMesh_h.hlsli"

VS_OUTTO_GS_CSM main(VS_IN vin, uint instance_id : SV_INSTANCEID)
{
    vin.normal.w = 0;
    float sigma = vin.tangent.w;
    vin.tangent.w = 0;
#if 0
    float4 blended_position = { 0, 0, 0, 1 };
    float4 blended_normal = { 0, 0, 0, 0 };
    float4 blended_tangent = { 0, 0, 0, 0 };
    float t = vin.bone_weights.x + vin.bone_weights.y + vin.bone_weights.z + vin.bone_weights.w;
    vin.bone_weights /= t;
    for (int bone_index = 0; bone_index < 4; ++bone_index)
    {
        blended_position += vin.bone_weights[bone_index] * mul(vin.position, bone_transforms[vin.bone_indices[bone_index]]);
        blended_normal += vin.bone_weights[bone_index] * mul(vin.normal, bone_transforms[vin.bone_indices[bone_index]]);
        blended_tangent += vin.bone_weights[bone_index] * mul(vin.tangent, bone_transforms[vin.bone_indices[bone_index]]);
    }
   
    vin.position = float4(blended_position.xyz, 1.0f);
    vin.normal = float4(blended_normal.xyz, 0.0f);
    vin.tangent = float4(blended_tangent.xyz, 0.0f);

#endif
    PointLightShadowCastData shadowCastData = pointLights[0].shadowCastData;
    
    VS_OUTTO_GS_CSM vout;
    vout.position = mul(vin.position, mul(world, shadowCastData.view_projection_matrices[instance_id]));
    vout.texcoord = vin.texcoord;
    vout.slice = instance_id;
    return vout;
}
