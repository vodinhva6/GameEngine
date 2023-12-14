#include "OBJ3D.h"
#include "GameEngine.h"
#include "DataBase.h"
#include "MaterialManager.h"
OBJ3D::OBJ3D():Actor()
{
    deadFlag = false;
    damageFlag = false;
    scale_ = { 1,1,1 };
    MaterialManager* materialManager = GetFrom<MaterialManager>(GameEngine::get()->getMaterialManager());
    auto material = materialManager->getDummyMaterial();
    meshInfor.materials_.insert(std::make_pair(material->getUniqueId(), material));
    color_ = { 1,1,1,1 };
    position_ = { 0,0,0 };
    animation.timeLine = 0;
    drawStates_.blendState = BlendStates::Alpha;
    drawStates_.depthState = DepthStencilStates::DepthTestOn_DepthWriteOn;
    drawStates_.drawType = DrawType::SkinnedMeshObject;
    drawStates_.rasterizerState = RasterizerStates::CullNON_DrawTriagles;
    drawStates_.samplerState = SamplerStates::Anisotropic;
}

void OBJ3D::Update(float elapsedTime)
{
    updateWorldTrans();
    Actor::Update(elapsedTime);
}

void OBJ3D::UpdateDebug(float elapsedTime)
{
    OBJ3D::Update(elapsedTime);
}

void OBJ3D::Draw()
{
    if(meshInfor.mesh_.get())
        GameEngine::get()->DrawSkinnedMesh(meshInfor.mesh_, worldTransform, &meshInfor.materials_, color_, drawTurn_, drawStates_, animation.key, FrameBufferName::MSAABUFFER1);
    if (sprite3D_.get())
        GameEngine::get()->DrawSprite3D(SetToPoint(sprite3D_), worldTransform, color_, drawTurn_, FrameBufferName::MSAABUFFER1);
}


void OBJ3D::updateWorldTrans()
{
    DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(scale_.x, scale_.y, scale_.z) };
    DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotation_.x), DirectX::XMConvertToRadians(rotation_.y), DirectX::XMConvertToRadians(rotation_.z)) };
    DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(position_.x, position_.y, position_.z) };
     
    DirectX::XMStoreFloat4x4(&worldTransform, S * R * T); 
 
}


OBJ3DType OBJ3D::getTypeOBJ3D()
{
    return OBJ3DType::NON;
}

void OBJ3D::DrawDebug()
{
    //if (boundingHit_.sphere_)
    //    GameEngine::get()->DrawDebugBounding(boundingHit_.sphere_, worldTransform, color_,2);
    //if (boundingHit_.capsule_)
    //    GameEngine::get()->DrawDebugBounding(boundingHit_.capsule_, worldTransform, color_,3);
    //for (auto& ray : boundingHit_.raylist)
    //{
    //    if (ray.get())
    //    {
    //        GameEngine::get()->DrawDebugBounding(ray, worldTransform, ray->color, 3);
    //    }
    //}
}

OBJ3D::~OBJ3D()
{
}


//void OBJ3D::CreateBoundingCapsule(const float& height, const float& weight, const VECTOR3& offset)
//{
//    pSmartVoid p = SetToPoint(boundingHit_.capsule_);
//    GameEngine::get()->CreateCapsuleDebug(p, height, weight, offset);
//    boundingHit_.capsule_ = GetFromPoint<BoundingCapsule>(p);
//}
//
//void OBJ3D::CreateBoundingSphere(const float& radius, const VECTOR3& offset)
//{
//    pSmartVoid p = SetToPoint(boundingHit_.sphere_);
//    GameEngine::get()->createSphereDebug(p, radius, offset);
//    boundingHit_.sphere_ = GetFromPoint<BoundingSphere>(p);
//}

//void OBJ3D::CreateRay(const VECTOR3& begin, const VECTOR3& end, const VECTOR4& color, RayType Type)
//{
//    std::shared_ptr<void> pRayVoid;
//    GameEngine::get()->CreateRay(pRayVoid, begin, end, color, Type);
//    std::shared_ptr<Ray> ray(nullptr);
//    ray = std::static_pointer_cast<Ray>(pRayVoid);
//    boundingHit_.raylist.push_back(ray);
//}
