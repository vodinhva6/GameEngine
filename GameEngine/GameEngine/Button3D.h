//#pragma once
//#include "Block.h"
//
//
//class Button3D
//{
//public:
//    Button3D() {}
//    ~Button3D() {}
//
//    void update(float elapsed_time);
//    void draw();
//    void pushBlock(Block* block);
//    void clear();
//    void turn(float turnAngleY);
//    void deformationSquare();
//    bool RayCastReturnBool(const VECTOR3& begin, const VECTOR3& end);
//
//    void setPos(VECTOR3 pos) { this->pos = pos; }
//    void setRot(VECTOR3 rot) { this->rot = rot; }
//    void setInitRot(VECTOR3 initRot) { this->initRot = initRot; }
//    void setNextRot(VECTOR3 nextRot) { this->nextRot = nextRot; }
//    void setSca(VECTOR3 sca) { this->sca = sca; }
//    void setNextSca(VECTOR3 nextSca) { this->nextSca = nextSca; }
//    void setMyNumber(int myNumber) { this->myNumber = myNumber; }
//
//    VECTOR3 getPos() { return pos; }
//    VECTOR3 getRot() { return rot; }
//    VECTOR3 getSca() { return sca; }
//    int  getMyNumber() { return myNumber; }
//
//    enum class CLICK_FLAG
//    {
//        NO = -1,
//        BACK,
//        MIDLE, 
//        NEXT
//    };
//
//    enum class TURN
//    {
//        NO = -1,
//        RIGHT,
//        LEFT
//    };
//
//private:
//    void updateTrans();
//    TURN isTurn = TURN::NO;
//    float turnAngleY = 0;
//
//    DirectX::XMFLOAT4X4 transform = {};
//    VECTOR3 pos = { 0.0f, 0.0f, 0.0f };
//    VECTOR3 rot = { 0.0f, 0.0f, 0.0f };
//    VECTOR3 initRot = {};
//    VECTOR3 nextRot = {};
//    VECTOR3 sca = { 1.0f, 1.0f, 1.0f };
//    VECTOR3 nextSca = { 1.0f, 1.0f, 1.0f };
//
//    CLICK_FLAG clickFlag = CLICK_FLAG::NO;
//    int myNumber;
//
//    std::vector<Block*> buttonBlocks;
//};