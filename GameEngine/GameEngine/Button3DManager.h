//#pragma once
//
//#include "Button3D.h"
//class Button3DManager : public SingletonPatternTemplate<Button3DManager>
//{
//    friend class SingletonPatternTemplate<Button3DManager>;
//
//public:
//    enum class IS_SELECTING
//    {
//        SELECTING = 0,
//        DEFORMING,
//        GO_STAGE,
//    };
//
//    Button3DManager();
//    ~Button3DManager();
//    void update(float elapsed_time);
//    void draw();
//    void init();
//    void clear();
//    void turn(float angleY);
//    void deformationSquare(int buttonIndex);
//
//    void setFrontNumber(int frontNumber) { this->frontNumber = frontNumber; }
//    void setIsSelecting(IS_SELECTING isGoStage) { this->isSelecting = isGoStage; }
//    int getFrontNumber() { return frontNumber; }
//    VECTOR3 getButton3DSpase() { return button3DSpase; }
//    Button3D& getButtonsStage3D(int index) { return buttonsStage3D[index]; }
//    size_t getButtonsStage3DSize() { return buttonsStage3D.size(); }
//    IS_SELECTING getIsSelecting() { return isSelecting; }
//    VECTOR3 getSquarePos(int index) { return squarePos[index]; }
//    size_t getSquarePosSize() { return squarePos.size(); }
//
//private:
//    bool managerOn;
//    void inportFile();
//
//    int frontNumber;
//    const VECTOR3 button3DSpase = { -70, 0, 0};
//    std::vector<Button3D> buttonsStage3D;
//
//    IS_SELECTING isSelecting = IS_SELECTING::SELECTING;
//    std::vector<VECTOR3> squarePos;
//    const float squareComingSpeed = 90.0f;
//    const float squareRotationSpeed = 180.0f;
//};
//
