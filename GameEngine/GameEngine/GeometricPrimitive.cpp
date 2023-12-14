//#include "GeometricPrimitive.h"
//#include "GameEngine.h"
//#include "ShaderManager.h"
//#include "Misc.h"
//GeometricPrimitive::GeometricPrimitive(ID3D11Device* device, 
//    const VECTOR3& sizeMin, const VECTOR3& sizeMax,
//    ShaderData shaderData)
//{
//   
//    {
//        const VECTOR2& sizeX = { sizeMin.x,sizeMax.x };
//        const VECTOR2& sizeY = { sizeMin.y,sizeMax.y };
//        const VECTOR2& sizeZ = { sizeMin.z,sizeMax.z };
//        Vertex verticesCube[24]
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
//
//
//
//        };
//
//        uint32_t indicesCube[36]
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
//        createCOMBuffers(device, verticesCube, 24, indicesCube, 36);
//    }
//    HRESULT hr{ S_OK };
//
//    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
//    {
//        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
//       D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
//       D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//    };
//    ShaderManager* shaderManager = GetFrom<ShaderManager>(GameEngine::get()->getShaderManager());
//    hr = shaderManager->createVSFromFile_cso(device, shaderData.vsfile, vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
//    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
//    hr = shaderManager->createPSFromFile_cso(device, shaderData.psfile, pixel_shader.GetAddressOf());
//    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
//
//   
//
//    
//}
//
//void GeometricPrimitive::renderCube(ID3D11DeviceContext* immediate_context, DirectX::XMFLOAT4X4& world, const VECTOR4& color)
//{
//    uint32_t stride{ sizeof(Vertex) };
//    uint32_t offset{ 0 };
//    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
//    immediate_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
//    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
//    immediate_context->IASetInputLayout(input_layout.Get());
//
//    immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
//    immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
//
//
//    constants data{ world, color };
//    immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
//    immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
//
//    D3D11_BUFFER_DESC buffer_desc{};
//    index_buffer.Get()->GetDesc(&buffer_desc);
//    immediate_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(uint32_t), 0, 0);
//}
//void GeometricPrimitive::renderBoudingBox(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& color)
//{
//    
//    uint32_t stride{ sizeof(Vertex) };
//    uint32_t offset{ 0 };
//    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
//    immediate_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
//    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//    immediate_context->IASetInputLayout(input_layout.Get());
//
//    immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
//    immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
//
//
//    constants data{ world, color };
//    immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
//    immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
//    immediate_context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
//
//    D3D11_BUFFER_DESC buffer_desc{};
//    index_buffer.Get()->GetDesc(&buffer_desc);
//    immediate_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(uint32_t), 0, 0);
//}
//
//void GeometricPrimitive::renderPointList(ID3D11DeviceContext* immediate_context, DirectX::XMFLOAT4X4& world, const VECTOR4& color)
//{
//    uint32_t stride{ sizeof(Vertex) };
//    uint32_t offset{ 0 };
//    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
//    immediate_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
//    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
//
//    //immediate_context->IASetInputLayout(input_layout.Get());
//    //immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
//    //immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
//
//    constants data{ world, color };
//    immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
//    immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
//    immediate_context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
//
//    D3D11_BUFFER_DESC buffer_desc{};
//    index_buffer.Get()->GetDesc(&buffer_desc);
//    immediate_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(uint32_t), 0, 0);
//}
//
//void GeometricPrimitive::renderPointStrip(ID3D11DeviceContext* immediate_context, DirectX::XMFLOAT4X4& world, const VECTOR4& color)
//{
//    uint32_t stride{ sizeof(Vertex) };
//    uint32_t offset{ 0 };
//    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
//    immediate_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
//    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
//
//    //immediate_context->IASetInputLayout(input_layout.Get());
//    //immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
//    //immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
//
//    constants data{ world, color };
//    immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
//    immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
//    immediate_context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
//
//    D3D11_BUFFER_DESC buffer_desc{};
//    index_buffer.Get()->GetDesc(&buffer_desc);
//    immediate_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(uint32_t), 0, 0);
//}
//
//void GeometricPrimitive::createCOMBuffers(ID3D11Device* device, Vertex* vertices, size_t vertex_count, uint32_t* indices, size_t index_count)
//{
//    HRESULT hr{ S_OK };
//
//    D3D11_BUFFER_DESC buffer_desc{};
//    D3D11_SUBRESOURCE_DATA subresource_data{};
//    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertex_count);
//    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
//    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//    buffer_desc.CPUAccessFlags = 0;
//    buffer_desc.MiscFlags = 0;
//    buffer_desc.StructureByteStride = 0;
//    subresource_data.pSysMem = vertices;
//    subresource_data.SysMemPitch = 0;
//    subresource_data.SysMemSlicePitch = 0;
//    hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.ReleaseAndGetAddressOf());
//    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
//
//    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * index_count);
//    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
//    buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//    subresource_data.pSysMem = indices;
//    hr = device->CreateBuffer(&buffer_desc, &subresource_data, index_buffer.ReleaseAndGetAddressOf());
//    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
//
//    {
//        D3D11_BUFFER_DESC buffer_desc{};
//        buffer_desc.ByteWidth = sizeof(constants);
//        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
//        buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//        hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
//        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
//    }
//}
//
//void GeometricPrimitive::create_com_buffers_line(ID3D11Device* device, Vertex* vertices, size_t vertex_count, uint32_t* indices, size_t index_count)
//{
//}
