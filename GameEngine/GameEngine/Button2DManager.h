//#pragma once
//#include "SingletonPatternTemplate.h"
//#include "Actor.h"
//#include <vector>
//
//class Button2D;
//
//class Button2DManager : public SingletonPatternTemplate<Button2DManager>
//{
//public:
//    enum class INIT_TYPE
//    {
//        TITLE = 0,
//        GAME,
//    };
//
//    ~Button2DManager();
//
//    void update();
//    void draw();
//    void init(INIT_TYPE initType);
//    void clear();
//    
//    void setIsMouse(bool isMouse) { this->isMouse = isMouse; }
//
//private:
//    std::vector<Button2D> buttons2D;
//    int selectButtonIndex;
//    bool isMouse;
//    INIT_TYPE initType;
//};
//
//class Button2D : public OBJ2D
//{
//public:
//    enum class IS_SELECTING
//    {
//        NO = 0,
//        SELECTING,
//    };
//
//    enum class MY_TYPE
//    {
//        GO_SELECT = 0,
//        EXIT_GAME,
//        BACK_GAME,
//        RETRY,
//        GO_TITLE,
//    };
//
//    Button2D(MY_TYPE myType, VECTOR2 pos);
//    void update(bool isMouse);
//    void decision();
//
//private:
//    IS_SELECTING isSelecting = IS_SELECTING::NO;
//    MY_TYPE myType;
//    bool mouseIsTouching = false;
//};