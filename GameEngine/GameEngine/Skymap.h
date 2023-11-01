#pragma once
#include <wrl.h>
#include <memory>
#include <d3d11.h>
#include <filesystem>
#include "DataFormat.h"
#include "FullscreenQuad.h"
class Skymap
{
public:
    enum class TextureName
    {
        Colormap,
        Diffuse,
        Specular,
        LutGGX
    };
    Skymap();
    void Init(ID3D11Device* device);
    void CreateCOM(ID3D11Device* device);
    void Draw(ID3D11DeviceContext* dc, uint32_t startSlot, uint32_t numViews, FullscreenQuad* blitQuad);
    void SetIBL(ID3D11DeviceContext* dc);
    bool LoadTexture();
    ~Skymap();
public:
    template < class T>
    void serialize(T& archive)
    {
        archive(colorFilerConstant, textureMapLocal);
    }
public:
    VECTOR4& getColor() { return colorFilerConstant.materialColor; }
    std::string getTextureLocal() { return textureMapLocal; }
    float getHueShift   ()  {return colorFilerConstant.hueShift;}
    float getSaturation ()  {return colorFilerConstant.saturation;}
    float getBrightness ()  {return colorFilerConstant.brightness;}
    float getGamma()        {return colorFilerConstant.gamma;}

    void setHueShift   (const float& data) {colorFilerConstant.hueShift = data;}
    void setSaturation (const float& data) {colorFilerConstant.saturation = data;}
    void setBrightness (const float& data) {colorFilerConstant.brightness = data;}
    void setGamma      (const float& data) {colorFilerConstant.gamma = data;}
    void setColor(const VECTOR4& color) { colorFilerConstant.materialColor = color; }
    void setTextureLocal(std::filesystem::path local) { textureMapLocal = local.string(); }


    Microsoft::WRL::ComPtr<ID3D11Buffer>& getConstantBuffer() { return constantBuffer; }
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& getTextureShaderResourceView() { return skymapTexture[0]; }
private:
    std::string textureMapLocal;
    struct ConstantBuffer
    {
        VECTOR4 materialColor = { 1,1,1,1 };
        float hueShift = 1;
        float saturation = 1;
        float brightness = 1;
        float gamma = 2.2f;
    public:
        template < class T>
        void serialize(T& archive)
        {
            archive(materialColor, hueShift, saturation, brightness, gamma);
        }
    };
    ConstantBuffer colorFilerConstant;
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skymapTexture[4];


};

