#pragma once
#include "SpotLight.h"
#include "PointLight.h"
#include "ShadowManager.h"
#include <Mesh.h>
#include <memory>
#define MAX_SUPP_LIGHTS 10

class LightManager 
{
public:

    LightManager();
    void Init();
    void Update();
    void setLightPos(const VECTOR3& pos);
    void setLightDir(const VECTOR3& dir);
    ShadowManager* getShadowManager() { return shadowManager.get(); }
    void DeleteList();
    void UpdateRemovePointLight();
    ~LightManager();


public:
    template < class T>
    void serialize(T& archive)
    {
        archive(lightDirection, lightColor, vitualPosition, listPointLights, shadowManager);
    }


    VECTOR4& getVitualPos() { return vitualPosition; }
    VECTOR4& getLightDirection() { return lightDirection; }
    VECTOR4& getLightColor() { return lightColor; }

    size_t getSizePointLightList() { return listPointLights.size(); }
    size_t getSizeSpotLightList() { return listSpotLights.size(); }

    PointLight* getPointLightAt(size_t i) { return listPointLights[i].get(); }
public:
    void CreatePointLight();
    void Spr3DForLight();
private:
    VECTOR4 lightDirection;
    VECTOR4 lightColor;
    VECTOR4 vitualPosition;
    std::vector<std::shared_ptr<PointLight>> listPointLights;
    std::vector< std::shared_ptr<SpotLight>> listSpotLights;
    std::unique_ptr<ShadowManager> shadowManager;
    std::shared_ptr<Sprite3D> pointLightSpr3D;
    std::shared_ptr<Meshes> shpereMesh_;
};

