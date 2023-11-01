#include "CSMSkinnedMesh_h.hlsli"
#include "samplerStateSlotNum.hlsli"

Texture2D color_map : register(t0);
SamplerState sampler_states[5] : register(s0);

float main(GS_OUTTO_PS_CSM pin) : SV_DEPTH
{
    float alpha_cutoff = 0.5;
    float alpha = color_map.Sample(sampler_states[ANISOTROPIC], pin.texcoord).a;
    clip(alpha - alpha_cutoff);
    return pin.depth;
}
