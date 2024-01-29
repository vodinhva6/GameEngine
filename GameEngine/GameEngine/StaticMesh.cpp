#include <StaticMesh.h>

StaticMesh::StaticMesh()
{

}

void StaticMesh::CreateComObjects(ID3D11Device* device)
{
    HRESULT hr = S_OK;



    for (auto& mesh : meshes)
    {
        mesh->CreateCOM(device);
    }
    //

    UpdateVertexMaxMinInfor();
    Meshes::CreateComObjects(device);
}
