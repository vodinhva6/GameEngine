#ifndef OBJ2D_H
#define OBJ2D_H

#include "Actor.h"



enum ButtonName1
{
    Continue,
    Retry,
    StageMenu,
    Home,
    Next,
    NewGame,
    LoadGame,
    ControlerSetting,
    Exit
};



class OBJ2D : public Actor
{
public:
    OBJ2D();
    virtual void Update(float elapsedTime) override;
    virtual void Draw() override;
    ~OBJ2D();
public:
    VECTOR3& getHSizeMin() { return hSizeMin_; }
    VECTOR3& getHSizeMax() { return hSizeMax_; }

public:
    void setHSizeMin(const VECTOR3& Hmin) { hSizeMin_ = Hmin; }
    void setHSizeMax(const VECTOR3& Hmax) { hSizeMax_ = Hmax; }

    void setSize(const float& size) { size_ = size; }
    void addSize(const float& add) { size_ += add; }
    float& getSize() { return size_; }


private:
    VECTOR3 hSizeMin_;
    VECTOR3 hSizeMax_;
    VECTOR2 hSize_;

    VECTOR2 texturePosition_;
    VECTOR2 textureSize_;


    Sprite* spr_;
    float size_;

};


class TutorialTable : public OBJ2D
{
public:
    TutorialTable();
    void init();
    void Update(float elapsedTime) override;
    void setActive(bool acti) { active = acti; }
    void setUsed(bool use) { used = use; }
    void setGoOut(bool out) { goOut = out; }

    bool getActive() { return active; }
    bool getUsed() { return used; }

    ~TutorialTable() {}
private:
    bool active;
    bool used;
    bool goOut;
};


#endif // !OBJ2D_H
