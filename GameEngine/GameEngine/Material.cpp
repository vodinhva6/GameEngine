#include "Material.h"
#include "GameEngine.h"
#include <fstream>
Material::Material()
{
    uniqueId = 0;
    name = "Dummy";
    Ka = { 0.5f, 0.5f, 0.5f, 1.0f };
    Kd = { 0.7f, 0.7f, 0.7f, 1.0f };
    Ks = { 1.0f, 1.0f, 1.0f, 1.0f };
    occlusionStrength = 1;
    smoothness = 1;
    metallicfactor = 1;
    fileLocal = "Dummy";
    textureFilenames.resize(TextureShaderResourceSize);
}
Material::~Material()
{
}

Material::Material(int64_t materialId, const std::string& name, const VECTOR4& Kd, const VECTOR4& Ka,
    const VECTOR4& Ks, std::vector<std::string>& textureFileNames)
{
    uniqueId = materialId;
    this->name = name;
    this->Kd = Kd;
    this->Ks = Ks;
    this->Ka = Ka;
    this->textureFilenames = textureFileNames;
    occlusionStrength = 1;
    smoothness = 1;
    metallicfactor = 1;
}
void Material::CreateNewCereal()
{
    std::ofstream ofs(fileLocal.c_str(), std::ios::binary);
    cereal::BinaryOutputArchive serialization(ofs);
    serialization(fileLocal, uniqueId, name, Ka, Kd, Ks, occlusionStrength, smoothness, metallicfactor, textureFilenames);
}
bool Material::LoadCereal(std::filesystem::path local)
{
    std::ifstream ifs(local.c_str(), std::ios::binary);
    if (ifs.fail())
        return false;
    cereal::BinaryInputArchive deserialization(ifs);
    deserialization(fileLocal, uniqueId, name, Ka, Kd, Ks, occlusionStrength, smoothness, metallicfactor, textureFilenames);
    return true;
}
void Material::setTextureFileName(size_t slot, const std::string name)
{
    textureFilenames[slot] = name;
}
std::string& Material::getTextureFileNames(TextureSlotName slotName)
{
    return textureFilenames[(size_t)slotName];
}

std::string& Material::getTextureFileNames(size_t slotNum)
{
    return textureFilenames[slotNum];
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& Material::getTextureShaderResource(size_t slotNum)
{
    return textureShaderResource[slotNum];
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& Material::getTextureShaderResource(TextureSlotName slotName)
{
    return getTextureShaderResource((int)slotName);
}
