//#pragma once
//#define NOMINMAX
//#include <d3d11.h>
//#include <wrl.h>
//#include <directxmath.h>
//#include <string>
//#include <vector>
//#include <filesystem>
//#define TINYGLTF_NO_EXTERNAL_IMAGE
//#define TINYGLTF_NO_STB_IMAGE
//#define TINYGLTF_NO_STB_IMAGE_WRITE
//#include "tiny_gltf.h"
//#include "DataFormat.h"
//class GltfModel
//{
//   
//public:
//    struct Node;
//    struct BufferView;
//    GltfModel(ID3D11Device* device, const std::filesystem::path& filename);
//    virtual ~GltfModel() = default;
//
//    void FetchNodes(const tinygltf::Model& gltfNode);
//    void CumulateTransforms(std::vector<Node>& nodes);
//    void FetchMeshes(ID3D11Device* device, const tinygltf::Model& gltfModel);
//    void FetchMaterials(ID3D11Device* device, const tinygltf::Model& gltfModel);
//    void FetchTextures(ID3D11Device* device, const tinygltf::Model& gltfModel);
//    void FetchAnimations(const tinygltf::Model& gltfModel);
//
//    void CreateCOM(ID3D11Device* device);
//    void Render(ID3D11DeviceContext* immediateContext, const DirectX::XMFLOAT4X4& world, const std::vector<Node>& animatedNodes, bool black = false);
//    void Animate(size_t animationIndex, float time, std::vector<Node>& animatedNodes, bool loopback = true);
//
//    BufferView MakeBufferView(const tinygltf::Accessor& accessor);
//
//
//
//public:
//
//
//    struct scene
//    {
//        std::string name;
//        std::vector<int> nodes; // Array of 'root' nodes
//    };
//    std::vector<scene> scenes;
//
//    struct Node
//    {
//        std::string name;
//        int skin{ -1 };
//        int mesh{ -1 };
//
//        std::vector<int> children;
//
//        DirectX::XMFLOAT4 rotation{ 0, 0, 0, 1 };
//        DirectX::XMFLOAT3 scale{ 1, 1, 1 };
//        DirectX::XMFLOAT3 translation{ 0, 0, 0 };
//
//        DirectX::XMFLOAT4X4 globalTransform{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
//
//    };
//    std::vector<Node> nodes;
//
//    struct BufferView
//    {
//        DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
//        Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
//        size_t strideInBytes = 0;
//        size_t sizeInBytes = 0;
//        size_t count() const
//        {
//            return sizeInBytes / strideInBytes;
//        }
//    };
//    struct Mesh
//    {
//        std::string name;
//        struct Primitive
//        {
//            int material;
//            std::map<std::string, BufferView> vertexBufferViews;
//            BufferView indexBufferView;
//        };
//        std::vector<Primitive> primitives;
//    };
//    std::vector<Mesh> meshes;
//
//    struct TextureInfor
//    {
//        int index = -1;
//        int texcoord = 0;
//    };
//    struct NormalTextureInfo
//    {
//        int index = -1;
//        int texcoord = 0;
//        float scale = 1;
//    };
//    struct OcclusionTextureInfo
//    {
//        int index = -1;
//        int texcoord = 0;
//        float strength = 1;
//    };
//    struct PBRMetallicRoughness
//    {
//        VECTOR4 baseColorFactor = { 1,1,1,1 };
//        TextureInfor baseColorTexture;
//        float metallicFactor = 1;
//        float roughnessFactor = 1;
//        TextureInfor metallicRoughnessTexture;
//    };
//    struct Material
//    {
//        std::string name;
//        struct Cbuffer
//        {
//            VECTOR3 emissiveFactor = { 0,0,0 };
//            int alphaMode = 0;// "OPAQUE" : 0, "MASK" : 1, "BLEND" : 2
//            float alphaCutoff = 0.5f;
//            bool doubleSided = false;
//            PBRMetallicRoughness pbrMetallicRoughness;
//            NormalTextureInfo normalTexture;
//            OcclusionTextureInfo occlusionTexture;
//            TextureInfor emissiveTexture;
//        };
//        Cbuffer data;
//    };
//    std::vector<Material> materials;
//    struct Texture
//    {
//        std::string name;
//        int source = 1;
//    };
//    std::vector<Texture> textures;
//    struct Image
//    {
//        std::string name;
//        int width{ -1 };
//        int height{ -1 };
//        int component{ -1 };
//        int bits{ -1 };
//        int pixelType{ -1 };
//        int bufferView;
//        std::string mimeType;
//        std::string uri;
//        bool AsIs{ false };
//    };
//    std::vector<Image> images;
//    struct Skin
//    {
//        std::vector<DirectX::XMFLOAT4X4> inverseBindMatrices;
//        std::vector<int> joints;
//    };
//    std::vector<Skin> skins;
//
//    struct Animation
//    {
//        std::string name;
//        struct Channel
//        {
//            int sampler = -1;
//            int targetNode = -1;
//            std::string targetPath;
//        };
//        std::vector<Channel> channels;
//        struct Sampler
//        {
//            int input = -1;
//            int output = -1;
//            std::string interpolation;
//        };
//        std::vector<Sampler> samplers;
//
//        std::unordered_map<int, std::vector<float>> timelines;
//        std::unordered_map<int, std::vector<DirectX::XMFLOAT3>> scales;
//        std::unordered_map<int, std::vector<DirectX::XMFLOAT4>> rotations;
//        std::unordered_map<int, std::vector<DirectX::XMFLOAT3>> translations;
//
//    };
//    std::vector<Animation> animations;
//
//    static const size_t PRIMITIVIE_MAX_JOINTS = 512;
//private:
//    std::string filename;
//    
//
//
//private:
//    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
//    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
//    Microsoft::WRL::ComPtr<ID3D11PixelShader> TemppixelShader;
//    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
//    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> materialResourceView;
//    std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textureResourceViews;
//
//    struct PrimitiveConstants
//    {
//        DirectX::XMFLOAT4X4 matrices[PRIMITIVIE_MAX_JOINTS];
//        DirectX::XMFLOAT4X4 world;
//        int material{ -1 };
//        int has_tangent{ 0 };
//        int skin{ -1 };
//        int dummy;
//    };
//    Microsoft::WRL::ComPtr<ID3D11Buffer> primitiveCbuffer;
//
//};
//
