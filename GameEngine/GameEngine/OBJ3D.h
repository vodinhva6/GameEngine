#ifndef OBJ3D_H
#define OBJ3D_H
#include <Mesh.h>
#include "Actor.h"
enum class OBJ3DType
{
    NON,
    CHARATER,
    PLAYER,
    ENEMY,
    BLOCK,
    EEND,
    ALLY,
    POINTLIGHT,
};

class OBJ3D : public Actor
{
public:
    OBJ3D();
    virtual ~OBJ3D();
    void CreateBoundingCapsule(const float& height, const float& weight, const VECTOR3& offset);
    void CreateBoundingSphere(const float& radius, const VECTOR3& offset);
    //void CreateRay(const VECTOR3& begin, const VECTOR3& end, const VECTOR4& color, RayType Type);
public:
    template < class T>
    void serialize(T& archive)
    {
        archive(cereal::base_class<Actor>(this), meshInfor,/* boundingHit_,*/ drawStates_, gravityValue, drawTurn_);
    }
public:
    virtual void Update(float elapsedTime) override;
    void UpdateDebug(float elapsedTime) override;
    virtual void Draw() override;
    void updateWorldTrans() override;
    virtual void UpdateAnimation(float elapsedTime) {}
    virtual void SaveKeyAnimation(float elapsedTime) {}
    virtual OBJ3DType getTypeOBJ3D();
    virtual void DrawDebug();
    DirectX::XMFLOAT4X4& getWorldTransform() { return worldTransform; }
    void setWorldTransform(DirectX::XMFLOAT4X4& transform) { worldTransform = transform; }
private:


public:


    int drawTurn_;
    float gravityValue;
    bool damageFlag;

    DirectX::XMFLOAT4X4 worldTransform =
    { 1,0,0,0,
     0,1,0,0,
     0,0,1,0,
     0,0,0,1 };

public:

    struct Animat
    {
        size_t AnimationNow;
        float timeLine;
        Animation::Keyframe* key;
    };
    Animat animation;
    DrawStates drawStates_;
    //Bounding boundingHit_;

    struct MeshInfor
    {
        std::shared_ptr<Meshes> mesh_;
        std::shared_ptr<Animator> animator_;
        std::unordered_map<int64_t, std::shared_ptr<Material>> materials_;

    public:
        template < class T>
        void serialize(T& archive)
        {
            archive(mesh_, animator_, materials_);
        }

    } meshInfor;


    std::shared_ptr<Sprite3D> sprite3D_;

};
CEREAL_REGISTER_TYPE(OBJ3D);

#endif // !OBJ3D_H

