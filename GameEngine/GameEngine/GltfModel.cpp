//#include "GltfModel.h"
//#define TINYGLTF_IMPLEMENTATION
//#include "tiny_gltf.h"
//#include "log.h"
//#include "GameEngine.h"
//#include <stack>
//bool NullLoadImageData(tinygltf::Image*, const int, std::string*, std::string*, int, int, const unsigned char*, int, void*)
//{
//    return true;
//}
//
//GltfModel::GltfModel(ID3D11Device* device, const std::filesystem::path& filename): filename(filename.string())
//{
//    tinygltf::TinyGLTF tinyGltf;
//    tinyGltf.SetImageLoader(NullLoadImageData, nullptr);
//    tinygltf::Model gltfModel;
//    std::string error, warning;
//    bool succeeded = false;
//    if (filename.extension() == ".glb")
//        succeeded = tinyGltf.LoadBinaryFromFile(&gltfModel, &error, &warning, this->filename.c_str());
//    else if (filename.extension() == ".gltf")
//        succeeded = tinyGltf.LoadASCIIFromFile(&gltfModel, &error, &warning, this->filename.c_str());
//#ifdef _DEBUG
//    SUCCEEDEDRESULTTEXT(warning.empty(), warning.c_str());
//    SUCCEEDEDRESULTTEXT(error.empty(), error.c_str());
//    SUCCEEDEDRESULTTEXT(succeeded, "Load GlTF File Failed");
//#endif // _DEBUG
//
// 
//    for (std::vector<tinygltf::Scene>::const_reference fltfScene : gltfModel.scenes)
//    {
//        scene& scene = scenes.emplace_back();
//        scene.name = gltfModel.scenes.at(0).name;
//        scene.nodes = gltfModel.scenes.at(0).nodes;
//    }
//    FetchMeshes(device, gltfModel);
//    FetchNodes(gltfModel);
//    FetchMaterials(device,gltfModel);
//    FetchTextures(device, gltfModel);
//    FetchAnimations(gltfModel);
//    CreateCOM(device);
//}
//
//void GltfModel::FetchNodes(const tinygltf::Model& gltfNode)
//{
//    for (std::vector<tinygltf::Node>::const_reference gltfNode : gltfNode.nodes)
//    {
//        Node& node{ nodes.emplace_back() };
//        node.name = gltfNode.name;
//        node.skin = gltfNode.skin;
//        node.mesh = gltfNode.mesh;
//        node.children = gltfNode.children;
//        if (!gltfNode.matrix.empty())
//        {
//            DirectX::XMFLOAT4X4 matrix = {};
//            for (size_t row = 0; row < 4; row++)
//            {
//                for (size_t column = 0; column < 4; column++)
//                {
//                    matrix(row, column) = static_cast<float>(gltfNode.matrix.at(4 * row + column));
//                }
//            }
//            DirectX::XMVECTOR S, T, R;
//            bool succeed = DirectX::XMMatrixDecompose(&S, &R, &T, DirectX::XMLoadFloat4x4(&matrix));
//#ifdef _DEBUG
//            SUCCEEDEDRESULTTEXT(succeed, "Decompose Matrix Failed");
//#endif // _DEBUG
//          
//            DirectX::XMStoreFloat3(&node.scale, S);
//            DirectX::XMStoreFloat4(&node.rotation, R);
//            DirectX::XMStoreFloat3(&node.translation, T);
//        }
//        else
//        {
//            if (gltfNode.scale.size() > 0)
//            {
//                node.scale.x = static_cast<float>(gltfNode.scale.at(0));
//                node.scale.y = static_cast<float>(gltfNode.scale.at(1));
//                node.scale.z = static_cast<float>(gltfNode.scale.at(2));
//            }
//            if (gltfNode.translation.size() > 0)
//            {
//                node.translation.x = static_cast<float>(gltfNode.translation.at(0));
//                node.translation.y = static_cast<float>(gltfNode.translation.at(1));
//                node.translation.z = static_cast<float>(gltfNode.translation.at(2));
//            }
//            if (gltfNode.rotation.size() > 0)
//            {
//                node.rotation.x = static_cast<float>(gltfNode.rotation.at(0));
//                node.rotation.y = static_cast<float>(gltfNode.rotation.at(1));
//                node.rotation.z = static_cast<float>(gltfNode.rotation.at(2));
//                node.rotation.w = static_cast<float>(gltfNode.rotation.at(3));
//            }
//        }
//    }
//    CumulateTransforms(nodes);
//}
//
//void GltfModel::CumulateTransforms(std::vector<Node>& nodes)
//{
//    std::stack<DirectX::XMFLOAT4X4> parentGlobalTransforms;
//    std::function<void(int)> traverse = [&](int nodeIndex)->void
//    {
//        Node& node = nodes.at(nodeIndex);
//        DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
//        DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotation));
//        DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translation.x, node.translation.y, node.translation.z);
//        DirectX::XMStoreFloat4x4(&node.globalTransform, S * R * T * DirectX::XMLoadFloat4x4(&parentGlobalTransforms.top()));
//        for (int childIndex : node.children)
//        {
//            parentGlobalTransforms.push(node.globalTransform);
//            traverse(childIndex);
//            parentGlobalTransforms.pop();
//        }
//       
//    };
//    for (std::vector<int>::value_type nodeIndex : scenes.at(0).nodes)
//    {
//        parentGlobalTransforms.push({
//            1,0,0,0,
//            0,1,0,0,
//            0,0,1,0,
//            0,0,0,1
//            });
//        traverse(nodeIndex);
//        parentGlobalTransforms.pop();
//    }
//}
//
//void GltfModel::FetchMeshes(ID3D11Device* device, const tinygltf::Model& gltfModel)
//{
//    HRESULT hr;
//    for (std::vector<tinygltf::Mesh>::const_reference gltfMesh : gltfModel.meshes)
//    {
//        Mesh& mesh = meshes.emplace_back();
//        mesh.name = gltfMesh.name;
//        for (std::vector<tinygltf::Primitive>::const_reference gltfPrimitive : gltfMesh.primitives)
//        {
//            Mesh::Primitive& primitive = mesh.primitives.emplace_back();
//            primitive.material = gltfPrimitive.material;
//
//            const tinygltf::Accessor& gltfAccessor = gltfModel.accessors.at(gltfPrimitive.indices);
//            const tinygltf::BufferView& gltfBufferView = gltfModel.bufferViews.at(gltfAccessor.bufferView);
//
//            primitive.indexBufferView = MakeBufferView(gltfAccessor);
//
//            D3D11_BUFFER_DESC bufferDesc{};
//            bufferDesc.ByteWidth = static_cast<UINT>(primitive.indexBufferView.sizeInBytes);
//            bufferDesc.Usage = D3D11_USAGE_DEFAULT;
//            bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//            D3D11_SUBRESOURCE_DATA subresourceData{};
//            subresourceData.pSysMem = gltfModel.buffers.at(gltfBufferView.buffer).data.data() +
//                gltfBufferView.byteOffset + gltfAccessor.byteOffset;
//            hr = device->CreateBuffer(&bufferDesc, &subresourceData, primitive.indexBufferView.buffer.ReleaseAndGetAddressOf());
//#ifdef _DEBUG
//            SUCCEEDEDRESULT(hr);
//#endif // _DEBUG
//         
//
//            for (std::map<std::string, int>::const_reference gltfAttribute : gltfPrimitive.attributes)
//            {
//                const tinygltf::Accessor& gltfAccessor = gltfModel.accessors.at(gltfAttribute.second);
//                const tinygltf::BufferView& gltfBufferView = gltfModel.bufferViews.at(gltfAccessor.bufferView);
//
//                BufferView vertexBufferView = MakeBufferView(gltfAccessor);
//
//                D3D11_BUFFER_DESC bufferDesc{};
//                bufferDesc.ByteWidth = static_cast<UINT>(vertexBufferView.sizeInBytes);
//                bufferDesc.Usage = D3D11_USAGE_DEFAULT;
//                bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//                D3D11_SUBRESOURCE_DATA subresourceData{};
//                subresourceData.pSysMem = gltfModel.buffers.at(gltfBufferView.buffer).data.data()
//                     + gltfBufferView.byteOffset + gltfAccessor.byteOffset;
//                hr = device->CreateBuffer(&bufferDesc, &subresourceData,
//                    vertexBufferView.buffer.ReleaseAndGetAddressOf());
//                _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
//                
//                 primitive.vertexBufferViews.emplace(std::make_pair(gltfAttribute.first, vertexBufferView));
//
//            }
//
//            const std::unordered_map<std::string, BufferView> attributes{
//                { "TANGENT", { DXGI_FORMAT_R32G32B32A32_FLOAT } },
//                { "TEXCOORD_0", { DXGI_FORMAT_R32G32_FLOAT } },
//                { "JOINTS_0", { DXGI_FORMAT_R16G16B16A16_UINT } },
//                { "WEIGHTS_0", { DXGI_FORMAT_R32G32B32A32_FLOAT } },
//            };
//
//            for (std::unordered_map<std::string, BufferView>::const_reference attribute : attributes)
//            {
//                if (primitive.vertexBufferViews.find(attribute.first) == primitive.vertexBufferViews.end())
//                {
//                    primitive.vertexBufferViews.insert(std::make_pair(attribute.first, attribute.second));
//                }
//            }
//        }
//    }
//}
//
//void GltfModel::FetchMaterials(ID3D11Device* device, const tinygltf::Model& gltfModel)
//{
//    for (std::vector<tinygltf::Material>::const_reference gltfMaterial : gltfModel.materials)
//    {
//        std::vector<Material>::reference material = materials.emplace_back();
//
//        material.name = gltfMaterial.name;
//
//        material.data.emissiveFactor.x = static_cast<float>(gltfMaterial.emissiveFactor.at(0));
//        material.data.emissiveFactor.y = static_cast<float>(gltfMaterial.emissiveFactor.at(1));
//        material.data.emissiveFactor.z = static_cast<float>(gltfMaterial.emissiveFactor.at(2));
//
//        material.data.alphaMode = gltfMaterial.alphaMode == "OPAQUE" ?
//           0 : gltfMaterial.alphaMode == "MASK" ? 1 : gltfMaterial.alphaMode == "BLEND" ? 2 : 0;
//        material.data.alphaCutoff = static_cast<float>(gltfMaterial.alphaCutoff);
//        material.data.doubleSided = gltfMaterial.doubleSided ? 1 : 0;
//    
//        material.data.pbrMetallicRoughness.baseColorFactor.x = static_cast<float>(gltfMaterial.pbrMetallicRoughness.baseColorFactor[0]);
//        material.data.pbrMetallicRoughness.baseColorFactor.y = static_cast<float>(gltfMaterial.pbrMetallicRoughness.baseColorFactor[1]);
//        material.data.pbrMetallicRoughness.baseColorFactor.z = static_cast<float>(gltfMaterial.pbrMetallicRoughness.baseColorFactor[2]);
//        material.data.pbrMetallicRoughness.baseColorFactor.w = static_cast<float>(gltfMaterial.pbrMetallicRoughness.baseColorFactor[3]);
//        
//        material.data.pbrMetallicRoughness.baseColorTexture.index = gltfMaterial.pbrMetallicRoughness.baseColorTexture.index;
//        material.data.pbrMetallicRoughness.baseColorTexture.texcoord = gltfMaterial.pbrMetallicRoughness.baseColorTexture.texCoord;
//        
//        material.data.pbrMetallicRoughness.metallicFactor = static_cast<float>(gltfMaterial.pbrMetallicRoughness.metallicFactor);
//        material.data.pbrMetallicRoughness.roughnessFactor = static_cast<float>(gltfMaterial.pbrMetallicRoughness.roughnessFactor);
//        material.data.pbrMetallicRoughness.metallicRoughnessTexture.index = gltfMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index;
//        material.data.pbrMetallicRoughness.metallicRoughnessTexture.texcoord = gltfMaterial.pbrMetallicRoughness.metallicRoughnessTexture.texCoord;
//    
//        material.data.normalTexture.index = gltfMaterial.normalTexture.index;
//        material.data.normalTexture.texcoord = gltfMaterial.normalTexture.texCoord;
//        material.data.normalTexture.scale = static_cast<float>(gltfMaterial.normalTexture.scale);
//        
//        material.data.occlusionTexture.index = gltfMaterial.occlusionTexture.index;
//        material.data.occlusionTexture.texcoord = gltfMaterial.occlusionTexture.texCoord;
//        material.data.occlusionTexture.strength = static_cast<float>(gltfMaterial.occlusionTexture.strength);
//        
//        material.data.emissiveTexture.index = gltfMaterial.emissiveTexture.index;
//        material.data.emissiveTexture.texcoord = gltfMaterial.emissiveTexture.texCoord;
//
//    }
//    std::vector<Material::Cbuffer> materialData;
//    for (std::vector<Material>::const_reference material : materials)
//    {
//        materialData.emplace_back(material.data);
//    }
//
//    HRESULT hr;
//    Microsoft::WRL::ComPtr<ID3D11Buffer> materialBuffer;
//    D3D11_BUFFER_DESC buffer_desc{};
//    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(Material::Cbuffer) * materialData.size());
//    buffer_desc.StructureByteStride = sizeof(Material::Cbuffer);
//    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
//    buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//    buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
//    D3D11_SUBRESOURCE_DATA subresource_data{};
//    subresource_data.pSysMem = materialData.data();
//    hr = device->CreateBuffer(&buffer_desc, &subresource_data, materialBuffer.GetAddressOf());
//#ifdef _DEBUG
//    SUCCEEDEDRESULT(hr);
//#endif // _DEBUG
//    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
//    shaderResourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
//    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
//    shaderResourceViewDesc.Buffer.NumElements = static_cast<UINT>(materialData.size());
//    hr = device->CreateShaderResourceView(materialBuffer.Get(), &shaderResourceViewDesc, materialResourceView.GetAddressOf());
//#ifdef _DEBUG
//    SUCCEEDEDRESULT(hr);
//#endif // _DEBUG
//}
//
//void GltfModel::FetchTextures(ID3D11Device* device, const tinygltf::Model& gltfModel)
//{
//    for (const tinygltf::Texture& gltfTexture : gltfModel.textures)
//    {
//        Texture& texture{ textures.emplace_back() };
//        texture.name = gltfTexture.name;
//        texture.source = gltfTexture.source;
//    }
//
//    for (const tinygltf::Image& gltfImage : gltfModel.images)
//    {
//        Image& image{ images.emplace_back() };
//        image.name = gltfImage.name;
//        image.width = gltfImage.width;
//        image.height = gltfImage.height;
//        image.component = gltfImage.component;
//        image.bits = gltfImage.bits;
//        image.pixelType = gltfImage.pixel_type;
//        image.bufferView = gltfImage.bufferView;
//        image.mimeType = gltfImage.mimeType;
//        image.uri = gltfImage.uri;
//        image.AsIs = gltfImage.as_is;
//        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureResourceView;
//        if (gltfImage.bufferView > -1)
//        {
//            const tinygltf::BufferView& bufferView = gltfModel.bufferViews.at(gltfImage.bufferView);
//            const tinygltf::Buffer & buffer = gltfModel.buffers.at(bufferView.buffer);
//            const byte * data = buffer.data.data() + bufferView.byteOffset;
//           
//            GameEngine::get()->LoadTextureFromMemory(data, bufferView.byteLength, textureResourceView);
//        }
//        else
//        {
//            std::filesystem::path path(filename);
//            D3D11_TEXTURE2D_DESC texture2d_desc;
//            path = path.parent_path().concat(L"/").string() + gltfImage.uri;
//
//            GameEngine::get()->LoadTextureFromFile(path, textureResourceView, &texture2d_desc);
//        }
//        textureResourceViews.push_back(textureResourceView);
//
//    }
//
//
//}
//
//void GltfModel::FetchAnimations(const tinygltf::Model& gltfModel)
//{
//    for (std::vector<tinygltf::Skin>::const_reference transmissionSkin : gltfModel.skins)
//    {
//        Skin& skin = skins.emplace_back();
//        const tinygltf::Accessor& gltfAccessor = gltfModel.accessors.at(transmissionSkin.inverseBindMatrices);
//        const tinygltf::BufferView& gltfBufferView = gltfModel.bufferViews.at(gltfAccessor.bufferView);
//        skin.inverseBindMatrices.resize(gltfAccessor.count);
//        memcpy(skin.inverseBindMatrices.data(), gltfModel.buffers.at(gltfBufferView.buffer).data.data() +
//            gltfBufferView.byteOffset + gltfAccessor.byteOffset, gltfAccessor.count * sizeof(DirectX::XMFLOAT4X4));
//        skin.joints = transmissionSkin.joints;
//    }
//    for (std::vector<tinygltf::Animation>::const_reference gltfAnimation : gltfModel.animations)
//    {
//        Animation& animation { animations.emplace_back() };
//        animation.name = gltfAnimation.name;
//        for (std::vector<tinygltf::AnimationSampler>::const_reference gltfSampler : gltfAnimation.samplers)
//        {
//            Animation::Sampler& sampler{ animation.samplers.emplace_back() };
//            sampler.input = gltfSampler.input;
//            sampler.output = gltfSampler.output;
//            sampler.interpolation = gltfSampler.interpolation;
//
//            const tinygltf::Accessor& gltfAccessor{ gltfModel.accessors.at(gltfSampler.input) };
//            const tinygltf::BufferView& gltfBufferView{ gltfModel.bufferViews.at(gltfAccessor.bufferView) };
//
//            auto timelines = animation.timelines.emplace(gltfSampler.input, gltfAccessor.count);
//            if (timelines.second)
//            {
//                memcpy(timelines.first->second.data(), gltfModel.buffers.at(gltfBufferView.buffer).data.data() +
//                    gltfBufferView.byteOffset + gltfAccessor.byteOffset, gltfAccessor.count * sizeof(FLOAT));
//            }
//        }
//        for (std::vector<tinygltf::AnimationChannel>::const_reference gltfChannel : gltfAnimation.channels)
//        {
//            Animation::Channel& channel{ animation.channels.emplace_back() };
//            channel.sampler = gltfChannel.sampler;
//            channel.targetNode = gltfChannel.target_node;
//            channel.targetPath = gltfChannel.target_path;
//            
//            const tinygltf::AnimationSampler & gltfSampler{ gltfAnimation.samplers.at(gltfChannel.sampler) };
//            const tinygltf::Accessor & gltfAccessor{ gltfModel.accessors.at(gltfSampler.output) };
//            const tinygltf::BufferView & gltfBufferView{ gltfModel.bufferViews.at(gltfAccessor.bufferView) };
//        
//
//            if (gltfChannel.target_path == "scale")
//            {
//                auto scales = animation.scales.emplace(gltfSampler.output, gltfAccessor.count);
//                if (scales.second)
//                {
//                    memcpy(scales.first->second.data(), gltfModel.buffers.at(gltfBufferView.buffer).data.data() +
//                        gltfBufferView.byteOffset + gltfAccessor.byteOffset, gltfAccessor.count * sizeof(DirectX::XMFLOAT3));
//                }
//            }
//            else if (gltfChannel.target_path == "rotation")
//            {
//                auto rotations = animation.rotations.emplace(gltfSampler.output, gltfAccessor.count);
//                if (rotations.second)
//                {
//                    memcpy(rotations.first->second.data(), gltfModel.buffers.at(gltfBufferView.buffer).data.data() +
//                        gltfBufferView.byteOffset + gltfAccessor.byteOffset, gltfAccessor.count * sizeof(DirectX::XMFLOAT4));
//                }
//            }
//            else if (gltfChannel.target_path == "translation")
//            {
//                auto translations = animation.translations.emplace(gltfSampler.output, gltfAccessor.count);
//                if (translations.second)
//                {
//                    memcpy(translations.first->second.data(), gltfModel.buffers.at(gltfBufferView.buffer).data.data() +
//                        gltfBufferView.byteOffset + gltfAccessor.byteOffset, gltfAccessor.count * sizeof(DirectX::XMFLOAT3));
//                }              
//
//            }
//        }
//
//    }
//
//}
//
//void GltfModel::CreateCOM(ID3D11Device* device)
//{
//    const std::map<std::string, BufferView>& vertexBufferViews = meshes[0].primitives[0].vertexBufferViews;
//    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
//    {
//        { "POSITION", 0, vertexBufferViews.at("POSITION").format, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
//        { "NORMAL", 0, vertexBufferViews.at("NORMAL").format, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//        { "TANGENT", 0, vertexBufferViews.at("TANGENT").format, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//        { "TEXCOORD", 0, vertexBufferViews.at("TEXCOORD_0").format, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//        { "JOINTS", 0, vertexBufferViews.at("JOINTS_0").format, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//        { "WEIGHTS", 0, vertexBufferViews.at("WEIGHTS_0").format, 5, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//    };
//
//    GameEngine::get()->CreateVS(device, "./Data/Shaders/gltfModel_vs.cso", vertexShader.ReleaseAndGetAddressOf(),inputLayout.ReleaseAndGetAddressOf(), input_element_desc, _countof(input_element_desc));
//    GameEngine::get()->CreatePS(device, "./Data/Shaders/gltfModel_ps.cso", pixelShader.ReleaseAndGetAddressOf());
//    GameEngine::get()->CreatePS(device, "./Data/Shaders/BlackgltfModel_ps.cso", TemppixelShader.ReleaseAndGetAddressOf());
//    
//    D3D11_BUFFER_DESC buffer_desc{};
//    buffer_desc.ByteWidth = sizeof(PrimitiveConstants);
//    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
//    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//    HRESULT hr;
//    hr = device->CreateBuffer(&buffer_desc, nullptr, primitiveCbuffer.ReleaseAndGetAddressOf());
//#ifdef _DEBUG
//    SUCCEEDEDRESULT(hr);
//#endif // _DEBUG
//}
//
//void GltfModel::Render(ID3D11DeviceContext* immediateContext, const DirectX::XMFLOAT4X4& world, const std::vector<Node>& animatedNodes, bool black)
//{
//    immediateContext->VSSetShader(vertexShader.Get(), nullptr, 0);
//    (black)? immediateContext->PSSetShader(TemppixelShader.Get(), nullptr, 0):
//    immediateContext->PSSetShader(pixelShader.Get(), nullptr, 0);
//    immediateContext->IASetInputLayout(inputLayout.Get());
//    immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//    immediateContext->PSSetShaderResources(0, 1, materialResourceView.GetAddressOf());
//
//    std::function<void(int)> traverse{ [&](int nodeIndex)->void
//    {
//        const Node& node = nodes[nodeIndex];
//        if (node.mesh > -1)
//        {
//            const Mesh& mesh = meshes[node.mesh];
//            for (std::vector<Mesh::Primitive>::const_reference primitive : mesh.primitives)
//            {
//                ID3D11Buffer* vertexBuffers[]
//                {
//                    primitive.vertexBufferViews.at("POSITION").buffer.Get(),
//                    primitive.vertexBufferViews.at("NORMAL").buffer.Get(),
//                    primitive.vertexBufferViews.at("TANGENT").buffer.Get(),
//                    primitive.vertexBufferViews.at("TEXCOORD_0").buffer.Get(),
//                    primitive.vertexBufferViews.at("JOINTS_0").buffer.Get(),
//                    primitive.vertexBufferViews.at("WEIGHTS_0").buffer.Get(),
//                };
//
//                UINT strides[]
//                {
//                    static_cast<UINT>(primitive.vertexBufferViews.at("POSITION").strideInBytes),
//                    static_cast<UINT>(primitive.vertexBufferViews.at("NORMAL").strideInBytes),
//                    static_cast<UINT>(primitive.vertexBufferViews.at("TANGENT").strideInBytes),
//                    static_cast<UINT>(primitive.vertexBufferViews.at("TEXCOORD_0").strideInBytes),
//                    static_cast<UINT>(primitive.vertexBufferViews.at("JOINTS_0").strideInBytes),
//                    static_cast<UINT>(primitive.vertexBufferViews.at("WEIGHTS_0").strideInBytes),
//                };
//
//                UINT offsets[_countof(vertexBuffers)]{ 0 };
//                immediateContext->IASetVertexBuffers(0, _countof(vertexBuffers), vertexBuffers, strides, offsets);
//                immediateContext->IASetIndexBuffer(primitive.indexBufferView.buffer.Get(), primitive.indexBufferView.format, 0);
//
//                PrimitiveConstants data{};
//                data.material = primitive.material;
//                data.has_tangent = primitive.vertexBufferViews.at("TANGENT").buffer != NULL;
//                data.skin = node.skin;
//                DirectX::XMStoreFloat4x4(&data.world, DirectX::XMLoadFloat4x4(&node.globalTransform) * DirectX::XMLoadFloat4x4(&world));
//
//                if (node.skin > -1)
//                {
//                    const Skin& skin = skins.at(node.skin);
//                    for (size_t jointIndex = 0; jointIndex < skin.joints.size(); jointIndex++)
//                    {
//                        DirectX::XMStoreFloat4x4(&data.matrices[jointIndex], DirectX::XMLoadFloat4x4(&skin.inverseBindMatrices.at(jointIndex)) *
//                            DirectX::XMLoadFloat4x4(&animatedNodes.at(skin.joints.at(jointIndex)).globalTransform) *
//                            DirectX::XMMatrixInverse(NULL, DirectX::XMLoadFloat4x4(&node.globalTransform))
//                        );
//                    }
//                }
//
//                immediateContext->UpdateSubresource(primitiveCbuffer.Get(), 0, 0, &data, 0, 0);
//                immediateContext->VSSetConstantBuffers(0, 1, primitiveCbuffer.GetAddressOf());
//                immediateContext->PSSetConstantBuffers(0, 1, primitiveCbuffer.GetAddressOf());
//
//                const Material& material = materials.at(primitive.material);
//                const int textureIndices[]
//                {
//                    material.data.pbrMetallicRoughness.baseColorTexture.index,
//                    material.data.pbrMetallicRoughness.metallicRoughnessTexture.index,
//                    material.data.normalTexture.index,
//                    material.data.emissiveTexture.index,
//                    material.data.occlusionTexture.index,
//                };
//
//                ID3D11ShaderResourceView* nullShaderResourceView{};
//                std::vector<ID3D11ShaderResourceView*> shaderResourceViews(_countof(textureIndices));
//                for (int textureIndex = 0; textureIndex < shaderResourceViews.size(); ++textureIndex)
//                {
//                    shaderResourceViews.at(textureIndex) = textureIndices[textureIndex] > -1 ?
//                        textureResourceViews.at(textures.at(textureIndices[textureIndex]).source).Get() :
//                        nullShaderResourceView;
//                }
//                immediateContext->PSSetShaderResources(1, static_cast<UINT>(shaderResourceViews.size()),
//                    shaderResourceViews.data());
//
//
//                immediateContext->DrawIndexed(static_cast<UINT>(primitive.indexBufferView.count()), 0, 0);
//            }
//        }
//        for (std::vector<int>::value_type childIndex : node.children)
//        {
//            traverse(childIndex);
//        }
//    } };
//    for (std::vector<int>::value_type nodeIndex : scenes[0].nodes)
//    {
//        traverse(nodeIndex);
//    }
//
//}
//
//void GltfModel::Animate(size_t animationIndex, float time, std::vector<Node>& animatedNodes, bool loopback)
//{
//    std::function<size_t(const std::vector<float>&, float, float&, bool)> indexOf
//    {
//        [](const std::vector<float>& timelines, float time, float& interpolationFactor, bool loopback)->size_t
//        {
//            const size_t keyFrameCount = timelines.size();
//            if (time > timelines.at(keyFrameCount - 1))
//            {
//                if (!loopback)
//                {
//                    time = fmodf(time, timelines.at(keyFrameCount - 1));
//                }
//                else
//                {
//                    interpolationFactor = 1.0f;
//                    return keyFrameCount - 2;
//                }
//            }
//            else if (time < timelines.at(0))
//            {
//                interpolationFactor = 0.0f;
//                return 0;
//            }
//            size_t keyFrameIndex = 0;
//            for (size_t timeIndex = 1; timeIndex < keyFrameCount; timeIndex++)
//            {
//                if (time < timelines.at(timeIndex))
//                {
//                    keyFrameIndex = std::max<size_t>(0LL, timeIndex - 1);
//                    break;
//                }
//            }
//            interpolationFactor = (time - timelines.at(keyFrameIndex + 0)) / (timelines.at(keyFrameIndex + 1) - timelines.at(keyFrameIndex + 0));
//            return keyFrameIndex;
//        }
//    };
//    if (animations.size() > 0)
//    {
//        const Animation& animation = animations.at(animationIndex);
//        for (std::vector<Animation::Channel>::const_reference channel : animation.channels)
//        {
//            const Animation::Sampler& sampler = animation.samplers.at(channel.sampler);
//            const std::vector<float>& timeline = animation.timelines.at(sampler.input);
//            if ( timeline.size() == 0)
//            {
//                continue;
//            }
//            float interpolationFactor = 0;
//            size_t keyFrameIndex = indexOf(timeline, time, interpolationFactor, loopback);
//            if (channel.targetPath == "scale")
//            {
//                const std::vector<DirectX::XMFLOAT3>& scales = animation.scales.at(sampler.output);
//                DirectX::XMStoreFloat3(&animatedNodes.at(channel.targetNode).scale, 
//                    DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&scales.at(keyFrameIndex + 0)),
//                        DirectX::XMLoadFloat3(&scales.at(keyFrameIndex + 1)), interpolationFactor));
//            }
//            else if (channel.targetPath == "rotation")
//            {
//                const std::vector<DirectX::XMFLOAT4>& rotations = animation.rotations.at(sampler.output);
//                DirectX::XMStoreFloat4(&animatedNodes.at(channel.targetNode).rotation,
//                    DirectX::XMQuaternionNormalize(DirectX::XMQuaternionSlerp(XMLoadFloat4(&rotations.at(keyFrameIndex + 0)),
//                         XMLoadFloat4(&rotations.at(keyFrameIndex + 1)), interpolationFactor)));
//            }
//            else if (channel.targetPath == "translation")
//            {
//                const std::vector<DirectX::XMFLOAT3>& translations{ animation.translations.at(sampler.output) };
//                DirectX::XMStoreFloat3(&animatedNodes.at(channel.targetNode).translation,
//                    DirectX::XMVectorLerp(XMLoadFloat3(&translations.at(keyFrameIndex + 0)),
//                        XMLoadFloat3(&translations.at(keyFrameIndex + 1)), interpolationFactor));
//            }
//        }
//        CumulateTransforms(animatedNodes);
//    }
//    else
//    {
//        animatedNodes = nodes;
//    }
//  
//
//}
//
//GltfModel::BufferView GltfModel::MakeBufferView(const tinygltf::Accessor& accessor)
//{
//    BufferView bufferView;
//    switch (accessor.type)
//    {
//    case TINYGLTF_TYPE_SCALAR:
//    {
//        switch (accessor.componentType)
//        {
//        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
//            bufferView.format = DXGI_FORMAT_R16_UINT;
//            bufferView.strideInBytes = sizeof(USHORT);
//            break;
//        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
//            bufferView.format = DXGI_FORMAT_R32_UINT;
//            bufferView.strideInBytes = sizeof(UINT);
//            break;
//        default:
//#ifdef _DEBUG
//            SUCCEEDEDRESULTTEXT(FALSE, "This accessor component type is not supported.");
//#endif // _DEBUG
//           
//            break;
//        }
//        break;
//    }
//   
//    case TINYGLTF_TYPE_VEC2:
//    {
//        switch (accessor.componentType)
//        {
//        case TINYGLTF_COMPONENT_TYPE_FLOAT:
//            bufferView.format = DXGI_FORMAT_R32G32_FLOAT;
//            bufferView.strideInBytes = sizeof(FLOAT) * 2;
//            break;
//        default:
//#ifdef _DEBUG
//            SUCCEEDEDRESULTTEXT(FALSE, "This accessor component type is not supported.");
//#endif // _DEBUG
//            break;
//        }
//        break;
//    }
//    case TINYGLTF_TYPE_VEC3:
//    {
//        switch (accessor.componentType)
//        {
//        case TINYGLTF_COMPONENT_TYPE_FLOAT:
//            bufferView.format = DXGI_FORMAT_R32G32B32_FLOAT;
//            bufferView.strideInBytes = sizeof(FLOAT) * 3;
//            break;
//        default:
//#ifdef _DEBUG
//            SUCCEEDEDRESULTTEXT(FALSE, "This accessor component type is not supported.");
//#endif // _DEBUG
//            break;
//        }
//        break;
//    }
//    case TINYGLTF_TYPE_VEC4:
//    {
//        switch (accessor.componentType)
//        {
//        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
//            bufferView.format = DXGI_FORMAT_R16G16B16A16_UINT;
//            bufferView.strideInBytes = sizeof(USHORT) * 4;
//            break;
//        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
//            bufferView.format = DXGI_FORMAT_R32G32B32A32_UINT;
//            bufferView.strideInBytes = sizeof(UINT) * 4;
//            break;
//        case TINYGLTF_COMPONENT_TYPE_FLOAT:
//            bufferView.format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//            bufferView.strideInBytes = sizeof(FLOAT) * 4;
//            break;
//        default:
//#ifdef _DEBUG
//            SUCCEEDEDRESULTTEXT(FALSE, "This accessor component type is not supported.");
//#endif // _DEBUG
//            break;
//             
//        }
//        break;
//    }
//    default:
//#ifdef _DEBUG
//        SUCCEEDEDRESULTTEXT(FALSE, "This accessor type is not supported.");
//#endif // _DEBUG
//        break;
//    }
//    bufferView.sizeInBytes = static_cast<UINT>(accessor.count * bufferView.strideInBytes);
//    return bufferView;
//}
//
