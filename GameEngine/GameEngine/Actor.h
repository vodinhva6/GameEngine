#pragma once
#include "DataFormat.h"
#include "Sprite.h"
#include "Sprite3D.h"
#include "GeometricPrimitive.h"
#include "ShaderManager.h"
#include "ControlPad.h"
#include "MyMath.h"
#include "BoundingInfor.h"
#include <map>
#ifdef USE_IMGUI
#include "UsesImGui_h.h"
#endif // USE_IMGUI

class OBJ2D;


enum class ActorType
{
    Object3D,
    Object2D
};

class Actor
{
public:
    Actor() {}

    ActorType getActorType() { return actorType; }
    std::string getActorTypeString();
    std::string getActorName() { return name_; }
    virtual void Update(float elapsedTime);
    virtual void Draw();
    virtual void updateWorldTrans() {}
    virtual void UpdateDebug(float elapsedTime) {}
    virtual void DrawDebug();
protected:
   
    VECTOR3 position_;
    VECTOR3 oldPosition_;
    VECTOR3 rotation_;
    VECTOR3 offSet_;
    VECTOR3 speedRotation_;
    VECTOR3 scale_;

    VECTOR3 speed_;
    VECTOR4 color_;

public:
    VECTOR3& getPosition() { return position_; }
    VECTOR3& getRotation() { return rotation_; }
    VECTOR4& getColor() { return color_; }
    VECTOR3& getScale() { return scale_; }
    VECTOR3& getSpeed() { return speed_; }
    VECTOR3& getSpeedRotation() { return speedRotation_; }

    bool& getDeadFlag() { return deadFlag; }

    std::vector<Actor*>& getChildList() { return childList; }
    Actor* getParent() { return parent_; }

public:
   
    void setPosition(const VECTOR3& pos) { position_ = pos; }
    void setSpeed(const VECTOR3& speed) { this->speed_ = speed; }
    void setRotation(const VECTOR3& rot) { rotation_ = rot; }
    void setColor(const VECTOR4& color) { color_ = color; }
    void setScale(const VECTOR3& scale) { scale_ = scale; }
    void setSpeedRotation(const VECTOR3& speed) { speedRotation_ = speed; }
  
    void setName(const std::string& name) { name_ = name; }

  
    void setDeadFlag(bool flag) { deadFlag = flag; }
    void setParent(Actor* parent) { parent_ = parent; }
public:
    void addPosition(const VECTOR3& add) { position_ += add; }
    void addRotation(const VECTOR3& add) { rotation_ += add; }
    void addSpeed(const VECTOR3& add) { speed_ += add; }
    void addColor(const VECTOR4& add) { color_ += add; }
    void addScale(const VECTOR3& add) { scale_ += add; }
    void addSpeedRotation(const VECTOR3& add) { speedRotation_ += add; }

    void addChild(Actor* child) { childList.push_back(child); }


protected:
    ActorType actorType;
    std::string name_ = "No Name";
    bool deadFlag;
    int timer;
  
    std::vector<Actor*> childList;
    Actor* parent_;


public:
    template < class T>
    void serialize(T& archive)
    {
        archive(position_, rotation_, color_, scale_, name_);
    }
};

