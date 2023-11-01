#pragma once
#include "OBJ3D.h"

class ActorDebug
{
public:
    void Init();
    Actor* getActorTagert() { return tagert; }
    void setActorTagert(Actor* actor) { tagert = actor; }
    void getTagert();
    bool setTagert();
    void Update();
    void Draw();
    ~ActorDebug();
public:
    template < class T>
    void serialize(T& archive)
    {
        archive(operation);
    }



public:
    void SetDebugWindowPos(const VECTOR2& pos) { windowPos = pos; }
private:
    
    bool Transforming(DirectX::XMVECTOR&);
    bool Scaling(DirectX::XMVECTOR&);
    bool Rotating(DirectX::XMVECTOR&);
    void UpdateDebugMaterial(OBJ3D* obj);
    void UpdateDebugMeshes(OBJ3D* obj);

    void Debug3DOBJ3D(OBJ3D* obj);
    void LocalOfResource(OBJ3D* obj);
    void DebugTexturing(OBJ3D* obj);
    void DebugTransform(OBJ3D* obj);
    void DebugAnimation(OBJ3D* obj);
    void EditName(OBJ3D* obj);
    void ChangeClass(OBJ3D* obj);
    void DebugPointLight(OBJ3D* obj);
    void DisplayNode();
    bool SelectNode(Actor* actor);
    void ActorNodeTable();
    void CreatePointLight();
    Actor* tagert;
    OBJ3D* tagert3D;
    OBJ2D* tagert2D;
    VECTOR2 windowPos;


private:
    float tagertTransform[16];
    int operation;
};

