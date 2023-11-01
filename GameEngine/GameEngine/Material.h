#pragma once
#include <string>
#include <DirectXMath.h>
#include <wrl.h>
#include <filesystem>
#include <Cereal.h>
#include "DataFormat.h"
enum class TextureSlotName
{
    ColorMap,
    NormalMap,
    Emmisive,
    Metallic,
    Occlusion
};
const int TextureShaderResourceSize = 5;
class Material
{
public:
    Material();
    Material(int64_t materialId, const std::string& name,
        const VECTOR4& Kd, const VECTOR4& Ka, const VECTOR4& Ks,
        std::vector<std::string>& textureFileNames);
    void CreateNewCereal();
    bool LoadCereal(std::filesystem::path local);
    ~Material();
private:
   


public:
    void setFileLocal(std::string local) { fileLocal = local; }
    void setUniqueId(uint64_t id) { uniqueId = id; }
    void setName(std::string name) { this->name = name; }
    void setKd(const VECTOR4& kd) { Kd = kd; }
    void setKs(const VECTOR4& ks) { Ks = ks; }
    void setKa(const VECTOR4& ka) { Ka = ka; }
    void setTextureFileName(size_t slot, const std::string name);
    void setOcclusionStrength(const float& value) {  occlusionStrength = value; }
    void setSmoothness(const float& value) {  smoothness = value; }
    void setMetallicfactor(const float& value) { metallicfactor = value; }
public:
    std::filesystem::path getFileLocal() { return fileLocal; }

    uint64_t& getUniqueId() { return uniqueId; }
    std::string& getMaterialName() { return name; }
    std::string& getTextureFileNames(TextureSlotName slotName);
    std::string& getTextureFileNames(size_t slotNum);
    std::vector<std::string>& getVectorTextureFileNames() { return textureFilenames; }

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& getTextureShaderResource(size_t slotNum);
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& getTextureShaderResource(TextureSlotName slotName);


    VECTOR4& getKa() { return Ka; }
    VECTOR4& getKd() { return Kd; }
    VECTOR4& getKs() { return Ks; }

    float& getOcclusionStrength() { return occlusionStrength; }
    float& getSmoothness() { return smoothness; }
    float& getMetallicfactor() { return metallicfactor; }
public:
    template < class T>
    void serialize(T& archive)
    {
        archive(fileLocal, uniqueId, name, Ka, Kd, Ks, occlusionStrength, smoothness, metallicfactor, textureFilenames);
    }
private:
    std::string fileLocal;
    uint64_t uniqueId ;
    std::string name  ;
    VECTOR4 Ka;
    VECTOR4 Kd;
    VECTOR4 Ks;
    float occlusionStrength;
    float smoothness;
    float metallicfactor;
    std::vector<std::string> textureFilenames;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureShaderResource[TextureShaderResourceSize];

   
};