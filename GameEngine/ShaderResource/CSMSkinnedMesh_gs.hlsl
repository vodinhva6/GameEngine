#include "CSMSkinnedMesh_h.hlsli"


[maxvertexcount(3)]
void main(triangle VS_OUTTO_GS_CSM input[3], inout TriangleStream<GS_OUTTO_PS_CSM> output)
{
    GS_OUTTO_PS_CSM element;
    element.slice = input[0].slice;
    
    element.position = input[0].position;
    element.texcoord = input[0].texcoord;
    element.depth = input[0].position.z / input[0].position.w;
    output.Append(element);
    element.position = input[1].position;
    element.texcoord = input[1].texcoord;
    element.depth = input[1].position.z / input[1].position.w;
    output.Append(element);
    element.position = input[2].position;
    element.texcoord = input[2].texcoord;
    element.depth = input[2].position.z / input[2].position.w;
    output.Append(element);

    output.RestartStrip();
}


//struct GSOutput
//{
//    float4 pos : SV_POSITION;
//};

//[maxvertexcount(3)]
//void main(
//	triangle float4 input[3] : SV_POSITION,
//	inout TriangleStream<GSOutput> output
//)
//{
//    for (uint i = 0; i < 3; i++)
//    {
//        GSOutput element;
//        element.pos = input[i];
//        output.Append(element);
//    }
//}