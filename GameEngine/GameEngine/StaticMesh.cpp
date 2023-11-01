#include "StaticMesh.h"
#include "GameEngine.h"
#include <filesystem>
#include "Misc.h"
using namespace DirectX;

HRESULT make_dummy_texture(ID3D11Device* device, ID3D11ShaderResourceView** shader_resource_view, DWORD value/*0xAABBGGRR*/, UINT dimension)
{
    HRESULT hr{ S_OK };

    D3D11_TEXTURE2D_DESC texture2d_desc{};
    texture2d_desc.Width = dimension;
    texture2d_desc.Height = dimension;
    texture2d_desc.MipLevels = 1;
    texture2d_desc.ArraySize = 1;
    texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texture2d_desc.SampleDesc.Count = 1;
    texture2d_desc.SampleDesc.Quality = 0;
    texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
    texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    size_t texels = dimension * dimension;
    std::unique_ptr<DWORD[]> sysmem{ std::make_unique< DWORD[]>(texels) };
    for (size_t i = 0; i < texels; ++i) sysmem[i] = value;

    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = sysmem.get();
    subresource_data.SysMemPitch = sizeof(DWORD) * dimension;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
    hr = device->CreateTexture2D(&texture2d_desc, &subresource_data, &texture2d);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    
    D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{};
    shader_resource_view_desc.Format = texture2d_desc.Format;
    shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shader_resource_view_desc.Texture2D.MipLevels = 1;
    hr = device->CreateShaderResourceView(texture2d.Get(), &shader_resource_view_desc,
        shader_resource_view);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    return hr;

}

StaticMesh::StaticMesh(ID3D11Device* device, const wchar_t* obj_filename, ShaderData shaderData)
{
    std::vector<vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t current_index{ 0 };
    
    std::vector<XMFLOAT3> positions;
    std::vector<XMFLOAT3> normals;
    std::vector<XMFLOAT2> texcoords;
    std::vector<std::wstring> mtl_filenames;
    float xMax = -100, xMin = 100, yMax = -100, yMin = 100, zMax = -100, zMin = 100;
    std::wifstream fin(obj_filename);
    int i = 0;
    _ASSERT_EXPR(fin, L"'OBJ file not found.");
    wchar_t command[256];
    while (fin)
    {
        i++;
        fin >> command;
        if (0 == wcscmp(command, L"v"))
        {
            float x, y, z;
            fin >> x >> y >> z;
            if (x > xMax)
                xMax = x;
            if (x < xMin)
                xMin = x;
            if (y > yMax)
                yMax = y;
            if (y < yMin)
                yMin = y;
            if (z > zMax)
                zMax = z;
            if (z < zMin)
                zMin = z;
            positions.push_back({ x, y, z });
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"vn"))
        {
            FLOAT i, j, k;
            fin >> i >> j >> k;
            normals.push_back({ i, j, k });
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"vt"))
        {
            float u, v;
            fin >> u >> v;
            texcoords.push_back({ u,1.0f - v });
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"usemtl"))
        {
            wchar_t usemtl[MAX_PATH]{ 0 };
            fin >> usemtl;
            subsets.push_back({ usemtl, static_cast<uint32_t>(indices.size()), 0 });
        }
        else if (0 == wcscmp(command, L"f"))
        {
            for (size_t i = 0; i < 3; i++)
            {
                vertex vertex;
                size_t v, vt, vn;

                fin >> v;
                vertex.position = positions.at(v - 1);
                if (L'/' == fin.peek())
                {
                    fin.ignore(1);
                    if (L'/' != fin.peek())
                    {
                        fin >> vt;
                        vertex.texcoord = texcoords.at(vt - 1);
                    }
                    if (L'/' == fin.peek())
                    {
                        fin.ignore(1);
                        fin >> vn;
                        vertex.normal = normals.at(vn - 1);
                    }
                }
                vertices.push_back(vertex);
                indices.push_back(current_index++);
            }
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"mtllib"))
        {
            wchar_t mtllib[256];
            fin >> mtllib;
            mtl_filenames.push_back(mtllib);
        }

        else
        {
            fin.ignore(1024, L'\n');
        }
    }

    boundingBox.sizeMin.x = xMin;
    boundingBox.sizeMin.y = yMin;
    boundingBox.sizeMin.z = zMin;

    boundingBox.sizeMax.x = xMax;
    boundingBox.sizeMax.y = yMax;
    boundingBox.sizeMax.z = zMax;


    fin.close();
    std::vector<subset>::reverse_iterator iterator = subsets.rbegin();
    iterator->index_count = static_cast<uint32_t>(indices.size()) - iterator->index_start;
    for (iterator = subsets.rbegin() + 1; iterator != subsets.rend(); ++iterator)
    {
        iterator->index_count = (iterator - 1)->index_start - iterator->index_start;
    }


    create_com_buffers(device, vertices.data(), vertices.size(), indices.data(), indices.size());

    

    std::filesystem::path mtl_filename(obj_filename);
    mtl_filename.replace_filename(std::filesystem::path(mtl_filenames[0]).filename());

    fin.open(mtl_filename);
    //_ASSERT_EXPR(fin, L"'MTL file not found.");

    while (fin)
    {
        fin >> command;
        if (0 == wcscmp(command, L"map_Kd"))
        {
            fin.ignore();
            wchar_t map_Kd[256];
            fin >> map_Kd;

            std::filesystem::path path(obj_filename);
            path.replace_filename(std::filesystem::path(map_Kd).filename());
            //texture_filename = path;
            materials.rbegin()->texture_filename[0] = path;
            fin.ignore(1024, L'\n');

        }
        else if (0 == wcscmp(command, L"map_bump") || 0 == wcscmp(command, L"bump"))
            {
            fin.ignore();
            wchar_t map_bump[256];
            fin >> map_bump;
           std::filesystem::path path(obj_filename);
            path.replace_filename(std::filesystem::path(map_bump).filename());
            materials.rbegin()->texture_filename[1] = path;
            fin.ignore(1024, L'\n');
           
        }
        else if (0 == wcscmp(command, L"newmtl"))
        {
            fin.ignore();
            wchar_t newmtl[256];
            material material;
            fin >> newmtl;
            material.name = newmtl;
            materials.push_back(material);
        }
        else if (0 == wcscmp(command, L"Kd"))
        {
            float r, g, b;
            fin >> r >> g >> b;
            materials.rbegin()->Kd = { r, g, b, 1 };
            fin.ignore(1024, L'\n');

        }
        else if (0 == wcscmp(command, L"Ka"))
        {
            float r, g, b;
            fin >> r >> g >> b;
            materials.rbegin()->Ka = { r, g, b, 1 };
            fin.ignore(1024, L'\n');

        }
        else if (0 == wcscmp(command, L"Ks"))
        {
            float r, g, b;
            fin >> r >> g >> b;
            materials.rbegin()->Ks = { r, g, b, 1 };
            fin.ignore(1024, L'\n');

        }
        else
        {
            fin.ignore(1024, L'\n');
        }

    }
    fin.close();

   
    HRESULT hr{ S_OK };
    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
       D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
       D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
         { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    //ShaderManager* shaderManager = GameEngine::get()->getGraphicEngine()->getShaderManager();
    //hr = shaderManager->createVSFromFile_cso(device, shaderData.vsfile, vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
    //_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    //hr = shaderManager->createPSFromFile_cso(device, shaderData.psfile, pixel_shader.GetAddressOf());
    //_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    //
    //GraphicEngine* graphicEngine = GameEngine::get()->getGraphicEngine();
    D3D11_TEXTURE2D_DESC texture2d_desc{};
    if (materials.size() == 0)
    {
        for (const subset& subset : subsets)
        {
            materials.push_back({ subset.usemtl });
        }
        make_dummy_texture(device, materials.begin()->shader_resource_view[0].GetAddressOf(), 0xFFFFFFFF, 16);
        make_dummy_texture(device, materials.begin()->shader_resource_view[1].GetAddressOf(), 0xFFFF7F7F, 16);
    }
    else
    for (material& material : materials)
    {
        for (int i = 0; i < 2; i++)
        {
            if (!material.texture_filename[i].empty())
                GameEngine::get()->LoadTextureFromFile(material.texture_filename[i].c_str(),
                material.shader_resource_view[i], &texture2d_desc);
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        }
        
    }
    //hr = TextureManager::get().load_texture_from_file(device, texture_filename.c_str(),
    //    shader_resource_view.GetAddressOf(), &texture2d_desc);
    
   



    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(constants);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    

}


void StaticMesh::render(ID3D11DeviceContext* immediate_context, DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color)
{
    uint32_t stride{ sizeof(vertex) };
    uint32_t offset{ 0 };
    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
    immediate_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    immediate_context->IASetInputLayout(input_layout.Get());
    immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
    
    for (const material& material : materials)
    {
        for (int i = 0; i < 2; i++)
        {
            if(material.shader_resource_view[i] != NULL)
            immediate_context->PSSetShaderResources(i, 1, material.shader_resource_view[i].GetAddressOf());
        }
        //constants data{ world, material_color };
        //XMStoreFloat4(&data.material_color, XMLoadFloat4(&material_color) * XMLoadFloat4(&material.Kd));
        constants data{ world, material.Ka, material.Kd, material.Ks };
        XMStoreFloat4(&data.Kd, XMLoadFloat4(&material_color) * XMLoadFloat4(&material.Kd));
        
        immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
        immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
        immediate_context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
            
        
        for (const subset& subset : subsets)
        {
            if (material.name == subset.usemtl)
            {
                immediate_context->DrawIndexed(subset.index_count, subset.index_start, 0);
            }
        }
        
    }
    
    

}

void StaticMesh::create_com_buffers(ID3D11Device* device, vertex* vertices, size_t vertex_count, uint32_t* indices, size_t index_count)
{
    HRESULT hr{ S_OK };

    D3D11_BUFFER_DESC buffer_desc{};
    D3D11_SUBRESOURCE_DATA subresource_data{};
    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(vertex) * vertex_count);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;
    subresource_data.pSysMem = vertices;
    subresource_data.SysMemPitch = 0;
    subresource_data.SysMemSlicePitch = 0;
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * index_count);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    subresource_data.pSysMem = indices;
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, index_buffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}
