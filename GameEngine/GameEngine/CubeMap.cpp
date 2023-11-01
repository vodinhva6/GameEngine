//#include "CubeMap.h"
//#include "Log.h"
//#include "GameEngine.h"
//SkyBox::SkyBox(ID3D11Device* device, float farD, std::filesystem::path textureLocal)
//{
//    {
//        const VECTOR2& sizeX = { -farD,farD };
//        const VECTOR2& sizeY = { -farD,farD };
//        const VECTOR2& sizeZ = { -farD,farD };
//        std::vector<Vertex> verticesCube = 
//        {
//            {DirectX::XMFLOAT3(sizeX.x, sizeY.x,sizeZ.x),    DirectX::XMFLOAT3(0,0,-1)},
//            {DirectX::XMFLOAT3(sizeX.x, sizeY.y,sizeZ.x),     DirectX::XMFLOAT3(0,0,-1)},
//            {DirectX::XMFLOAT3(sizeX.y, sizeY.y,sizeZ.x),      DirectX::XMFLOAT3(0,0,-1)},
//            {DirectX::XMFLOAT3(sizeX.y, sizeY.x,sizeZ.x),     DirectX::XMFLOAT3(0,0,-1)},
//
//            {DirectX::XMFLOAT3(sizeX.y, sizeY.x,sizeZ.y),      DirectX::XMFLOAT3(0,0,1)},
//            {DirectX::XMFLOAT3(sizeX.y, sizeY.y,sizeZ.y),       DirectX::XMFLOAT3(0,0,1)},
//            {DirectX::XMFLOAT3(sizeX.x, sizeY.y,sizeZ.y),      DirectX::XMFLOAT3(0,0,1)},
//            {DirectX::XMFLOAT3(sizeX.x, sizeY.x,sizeZ.y),     DirectX::XMFLOAT3(0,0,1)},
//
//            {DirectX::XMFLOAT3(sizeX.x, sizeY.y,sizeZ.x),     DirectX::XMFLOAT3(0,1,0)},    //top
//            {DirectX::XMFLOAT3(sizeX.x, sizeY.y,sizeZ.y),      DirectX::XMFLOAT3(0,1,0)},    //top
//            {DirectX::XMFLOAT3(sizeX.y, sizeY.y,sizeZ.y),       DirectX::XMFLOAT3(0,1,0)},    //top
//            {DirectX::XMFLOAT3(sizeX.y, sizeY.y,sizeZ.x),      DirectX::XMFLOAT3(0,1,0)},    //top
//
//            {DirectX::XMFLOAT3(sizeX.x, sizeY.x,sizeZ.x),    DirectX::XMFLOAT3(0,-1,0)},   //bot
//            {DirectX::XMFLOAT3(sizeX.y, sizeY.x,sizeZ.x),     DirectX::XMFLOAT3(0,-1,0)},   //bot
//            {DirectX::XMFLOAT3(sizeX.y, sizeY.x,sizeZ.y),      DirectX::XMFLOAT3(0,-1,0)},   //bot
//            {DirectX::XMFLOAT3(sizeX.x, sizeY.x,sizeZ.y),     DirectX::XMFLOAT3(0,-1,0)},   //bot
//
//            {DirectX::XMFLOAT3(sizeX.x, sizeY.x,sizeZ.x),    DirectX::XMFLOAT3(-1,0,0)},
//            {DirectX::XMFLOAT3(sizeX.x, sizeY.x,sizeZ.y),     DirectX::XMFLOAT3(-1,0,0)},
//            {DirectX::XMFLOAT3(sizeX.x, sizeY.y,sizeZ.y),      DirectX::XMFLOAT3(-1,0,0)},
//            {DirectX::XMFLOAT3(sizeX.x, sizeY.y,sizeZ.x),     DirectX::XMFLOAT3(-1,0,0)},
//
//            {DirectX::XMFLOAT3(sizeX.y, sizeY.x,sizeZ.x),     DirectX::XMFLOAT3(1,0,0)},
//            {DirectX::XMFLOAT3(sizeX.y, sizeY.y,sizeZ.x),      DirectX::XMFLOAT3(1,0,0)},
//            {DirectX::XMFLOAT3(sizeX.y, sizeY.y,sizeZ.y),       DirectX::XMFLOAT3(1,0,0)},
//            {DirectX::XMFLOAT3(sizeX.y, sizeY.x,sizeZ.y),      DirectX::XMFLOAT3(1,0,0)},
//        };
//
//        std::vector<uint32_t> indicesCube = 
//        {
//            //FRONT SIDE
//            0,1,2,
//            2,3,0,
//            //BACK SIDE
//            4,5,6,
//            6,7,4,
//            //TOP SIDE
//            8,9,10,
//            10,11,8,
//            //BOTTOM SIDE
//            12,13,14,
//            14,15,12,
//            //RIGHT SIDE
//            16,17,18,
//            18,19,16,
//            //LEFT SIDE
//            20,21,22,
//            22,23,20
//        };
//
//
//        createCOMBuffersSkyBox(device, verticesCube, indicesCube);
//    }
//    color = { 1,1,1,1 };
//
//    D3D11_TEXTURE2D_DESC texture2d_desc;
//    GameEngine::get()->LoadTextureFromFile(textureLocal, skyboxMap[0], &texture2d_desc);
//
//}
//
//void SkyBox::Render(ID3D11DeviceContext* immediate_context, DirectX::XMFLOAT4X4& world)
//{
//    uint32_t stride{ sizeof(Vertex) };
//    uint32_t offset{ 0 };
//    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
//    immediate_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
//    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//
//    constants data{ world, color };
//    immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
//    immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
//    immediate_context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
//    immediate_context->PSSetShaderResources(0, 1, skyboxMap[0].GetAddressOf());
//    D3D11_BUFFER_DESC buffer_desc{};
//    index_buffer.Get()->GetDesc(&buffer_desc);
//    immediate_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(uint32_t), 0, 0);
//}
//
//SkyBox::~SkyBox()
//{
//}
//
//void SkyBox::createCOMBuffersSkyBox(ID3D11Device* device, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
//{
//    HRESULT hr{ S_OK };
//
//    D3D11_BUFFER_DESC buffer_desc{};
//    D3D11_SUBRESOURCE_DATA subresource_data{};
//    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertices.size());
//    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
//    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//    buffer_desc.CPUAccessFlags = 0;
//    buffer_desc.MiscFlags = 0;
//    buffer_desc.StructureByteStride = 0;
//    subresource_data.pSysMem = &vertices.at(0);
//    subresource_data.SysMemPitch = 0;
//    subresource_data.SysMemSlicePitch = 0;
//    hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.ReleaseAndGetAddressOf());
//    SUCCEEDEDRESULT(hr);
//
//    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * indices.size());
//    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
//    buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//    subresource_data.pSysMem = &indices.at(0);
//    hr = device->CreateBuffer(&buffer_desc, &subresource_data, index_buffer.ReleaseAndGetAddressOf());
//    SUCCEEDEDRESULT(hr);
//
//    {
//        D3D11_BUFFER_DESC buffer_desc{};
//        buffer_desc.ByteWidth = sizeof(constants);
//        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
//        buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//        hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
//        SUCCEEDEDRESULT(hr);
//    }
//}
