#include "MaterialManager.h"
#include <fstream>
#include "GameEngine.h"
#include "TextureManager.h"
#include "MeshLoaderManager.h"


void MaterialManager::Init()
{
    TextureManager* textureManager = GetFrom<TextureManager>(GameEngine::get()->getTextureManager());
    dummyMaterial.reset(new Material);
    textureManager->getColorDummyTexture(dummyMaterial->getTextureShaderResource(TextureSlotName::ColorMap));
    textureManager->getNormalDummyTexture(dummyMaterial->getTextureShaderResource(TextureSlotName::NormalMap));
    textureManager->getEmissiveDummyTexture(dummyMaterial->getTextureShaderResource(TextureSlotName::Emmisive));
    textureManager->getOcclusionDummyTexture(dummyMaterial->getTextureShaderResource(TextureSlotName::Occlusion));
    textureManager->getMetallicDummyTexture(dummyMaterial->getTextureShaderResource(TextureSlotName::Metallic));
    materials.insert(std::make_pair(dummyMaterial->getFileLocal(), dummyMaterial));
}

std::shared_ptr<Material> MaterialManager::CreateNewDummyMaterial(std::filesystem::path local)
{
    
    std::shared_ptr<Material> material;
    material.reset(new Material);
    material->setName(GetNotUsedName(local, "NewMaterial"));


    TextureManager* textureManager = GetFrom<TextureManager>(GameEngine::get()->getTextureManager());
    textureManager->getColorDummyTexture(material->getTextureShaderResource(TextureSlotName::ColorMap));
    textureManager->getNormalDummyTexture(material->getTextureShaderResource(TextureSlotName::NormalMap));
    textureManager->getEmissiveDummyTexture(material->getTextureShaderResource(TextureSlotName::Emmisive));
    textureManager->getOcclusionDummyTexture(material->getTextureShaderResource(TextureSlotName::Occlusion));
    textureManager->getMetallicDummyTexture(material->getTextureShaderResource(TextureSlotName::Metallic));


    std::filesystem::path cerealFileLocal = local.string() + std::string("/");
    cerealFileLocal.replace_filename(material->getMaterialName());
    cerealFileLocal.replace_extension("material");
    material->setFileLocal(cerealFileLocal.string());
    material->CreateNewCereal();

    materials.insert(std::make_pair(cerealFileLocal, material));
    return material;
}

void MaterialManager::RegisterAllMaterialsFromMeshData(pSmartVoid& mesh, std::unordered_map<int64_t, std::shared_ptr<Material>>& materialMap)
{
    std::shared_ptr<SkinnedMesh> pSkinnedMesh = GetFromPoint<SkinnedMesh>(mesh);
    std::filesystem::path fixed;
    CreateFolder(pSkinnedMesh->getLocalPath(), fixed);
    for (MeshRaw& mesh : pSkinnedMesh->getMeshRawList())
        for (auto& subset : mesh.subsets)
        {
            auto it = materialMap.find(subset.material_unique_id);
            if (it == materialMap.end())
            {
                continue;
            }
            std::shared_ptr<Material> material = it->second;
            if (material.get())
            {
                if (material->getMaterialName() == "")
                    fixed.replace_filename("NoName");
                else fixed.replace_filename(material->getMaterialName());
                fixed.replace_extension("material");
                material->setFileLocal(fixed.string());
                LoadTextureShaderResource(material);

                BuildNewMaterialCereal(material);
                
                materials.insert(std::make_pair(fixed, material));
            }
             
        }
}

bool MaterialManager::LoadMaterial(std::filesystem::path dir, std::shared_ptr<Material>& material)
{
    if (CheckHadMaterial(dir, material))
        return true;
    bool result = true;
    material.reset(new Material);
    result = material->LoadCereal(dir);
    if (!result)
    {
        material = dummyMaterial;
        return result;
    }
    LoadTextureShaderResource(material);
    materials.insert(std::make_pair(dir, material));
    return result;
}

bool MaterialManager::ReleaseMaterial(std::shared_ptr<Material>& material)
{
    for (auto& it : materials)
        if (it.second.get() == material.get())
        {
            int count = it.second.use_count();
            if (count == 1)
            {
                it.second.reset();
                materials.erase(it.first);
                return true;
            }
            break;
        }
    return false;
}

bool MaterialManager::ReleaseMaterial(std::filesystem::path local)
{
    auto it = materials.find(local);
    if (it != materials.end())
    {
        int count = it->second.use_count();
        if (count == 1)
        {
            it->second.reset();
            it = materials.erase(it);
        }

        return true;
    }
    return false;
}

bool MaterialManager::DeleteMaterial(std::filesystem::path local)
{
    auto it = materials.find(local);
    if (it != materials.end())
    {
        it->second.reset();
        it = materials.erase(it);
        return true;
    }
    return false;
}

bool MaterialManager::RenameMaterial(std::filesystem::path oldLocal, std::filesystem::path newLocal)
{
    auto it = materials.find(oldLocal);
    std::shared_ptr<Material> pMaterial;
    if (it != materials.end())
    {
        pMaterial = it->second;
        it = materials.erase(it);
        pMaterial->setName(newLocal.filename().string());
        pMaterial->setFileLocal(newLocal.string());
        materials.insert(std::make_pair(newLocal, pMaterial));
        BuildNewMaterialCereal(pMaterial);
        return true;
    }
  
    return false;
}

void MaterialManager::GetDummyTexture(TextureSlotName name, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResource)
{
    TextureManager* textureManager = GetFrom<TextureManager>(GameEngine::get()->getTextureManager());
    switch (name)
    {
    case TextureSlotName::ColorMap:
        textureManager->getColorDummyTexture(shaderResource);
        break;
    case TextureSlotName::NormalMap:
        textureManager->getNormalDummyTexture(shaderResource);
        break;
    case TextureSlotName::Emmisive:
        textureManager->getEmissiveDummyTexture(shaderResource);
        break;
    case TextureSlotName::Metallic:
        textureManager->getMetallicDummyTexture(shaderResource);
        break;
    case TextureSlotName::Occlusion:
        textureManager->getOcclusionDummyTexture(shaderResource);
        break;
    default:
        break;
    }
}

void MaterialManager::GetDummyTexture(size_t slot, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResource)
{
    GetDummyTexture((TextureSlotName)slot, shaderResource);
}


void MaterialManager::LoadTextureShaderResource(std::shared_ptr<Material>& material)
{
    for (size_t texture_index = 0; texture_index < TextureShaderResourceSize; ++texture_index)
    {
        if (material->getTextureFileNames(texture_index).size() > 0)
        {
            std::filesystem::path path(material->getTextureFileNames(texture_index));
            D3D11_TEXTURE2D_DESC texture2d_desc;
            if (!GameEngine::get()->LoadTextureFromFile(path.wstring().c_str(), material->getTextureShaderResource(texture_index), &texture2d_desc))
                if (!material->getTextureShaderResource(texture_index))
                {
                    material->getTextureFileNames(texture_index) = "Dummy";
                    GetDummyTexture(texture_index, material->getTextureShaderResource(texture_index));
                }
        }
        else
            if (!material->getTextureShaderResource(texture_index))
            {
                material->getTextureFileNames(texture_index) = "Dummy";
                GetDummyTexture(texture_index, material->getTextureShaderResource(texture_index));
            }
    }
}

std::string MaterialManager::GetNotUsedName(std::filesystem::path local, std::string fileName)
{
    int count = 0;
    std::string checkName = fileName;
    local += "/" + checkName;
    local.replace_extension(".material");
  
    while (std::filesystem::exists(local))
    {
        count++;
        checkName = fileName + "(" + std::to_string(count) + ")";
        local.replace_filename(checkName);
        local.replace_extension(".material");
    }
    
    return checkName;

}

std::filesystem::path MaterialManager::CreateLocalDirection(std::filesystem::path local)
{
    std::filesystem::path cerealFileLocal = local;
    cerealFileLocal.replace_filename("Materials\\");
    if (!std::filesystem::exists(cerealFileLocal))
    {
        std::filesystem::create_directory(cerealFileLocal);
    }
    return cerealFileLocal;
}

void MaterialManager::CreateFolder(std::filesystem::path local, std::filesystem::path& fixLocal)
{
    fixLocal = local;
    fixLocal.replace_filename("Materials/");
    if (!std::filesystem::exists(fixLocal))
    {
        std::filesystem::create_directory(fixLocal);
    }
}
std::mutex fileMutex;
void MaterialManager::BuildNewMaterialCereal(std::shared_ptr<Material>& material)
{
    material->CreateNewCereal();
    //if (rebuildMaterial.get())
    //    rebuildMaterial->join();
    //rebuildMaterial.reset();
    //rebuildMaterial = std::make_unique<std::thread>(&Material::CreateNewCereal, material.get());
}

std::shared_ptr<Material>& MaterialManager::getDummyMaterial()
{
    return dummyMaterial;
}

MaterialManager::~MaterialManager()
{
    if (rebuildMaterial.get())
        rebuildMaterial->join();
    rebuildMaterial.reset();
    dummyMaterial.reset();
    materials.clear();
}

bool MaterialManager::CheckHadMaterial(std::filesystem::path dir, std::shared_ptr<Material>& material)
{
    auto it = materials.find(dir);
    if (it != materials.end())
    {
        material = it->second;
        return true;
    }
    return false;
}
