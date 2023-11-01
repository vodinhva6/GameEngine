#pragma once
#include "Material.h"
#include <unordered_map>
#include <thread>
class MaterialManager
{
public:
    void Init();
    std::shared_ptr<Material> CreateNewDummyMaterial(std::filesystem::path local);
    void RegisterAllMaterialsFromMeshData(pSmartVoid& mesh, std::unordered_map<int64_t, std::shared_ptr<Material>>& materialMap);
    bool LoadMaterial(std::filesystem::path dir, std::shared_ptr<Material>& material);
    bool ReleaseMaterial(std::shared_ptr<Material>& material);
    bool ReleaseMaterial(std::filesystem::path local);
    bool DeleteMaterial(std::filesystem::path local);
    bool RenameMaterial(std::filesystem::path oldLocal, std::filesystem::path newLocal);
    void GetDummyTexture(TextureSlotName name, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResource);
    void GetDummyTexture(size_t slot, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResource);
    void BuildNewMaterialCereal(std::shared_ptr<Material>&);
    void LoadTextureShaderResource(std::shared_ptr<Material>& material);
    std::shared_ptr<Material>& getDummyMaterial();
    ~MaterialManager();
private:
    bool CheckHadMaterial(std::filesystem::path dir, std::shared_ptr<Material>& material);
    
    std::string GetNotUsedName(std::filesystem::path local, std::string fileName);
    std::filesystem::path CreateLocalDirection(std::filesystem::path local);

    void CreateFolder(std::filesystem::path, std::filesystem::path&);
 
private:
    std::unordered_map<std::filesystem::path, std::shared_ptr<Material>> materials;
    std::shared_ptr<Material> dummyMaterial;
    std::unique_ptr<std::thread> rebuildMaterial;
};

