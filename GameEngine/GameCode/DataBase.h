//#pragma once
//#include "DataFormat.h"
//#include "SingletonPatternTemplate.h"
//#include "Player.h"
//#include "Actor.h"
//#include "Button.h"
//#include "Background.h" 
//#include "LightManager.h"
//
//class DataBase :public SingletonPatternTemplate<DataBase>
//{
//    friend class SingletonPatternTemplate<DataBase>;
//public:
//    DataBase();
//
//    void CreateAllObject();
//    //Background* CreateBackground();
//   // Player* CreatePlayer(int stage);
//   
//
//    void CreateSpotLight(SpotLight* spotlgt[10], int stageNum);
//    void CreatePointLight(PointLight* pointlgt[10], int stageNum);
//    ~DataBase();
//private:
//    Player* player_;
//    Background* skyBox_;
//  
//    VECTOR3 playerPos[10];
//    float blockSize;
//    PointLight* pointLights[MAX_SUPP_LIGHTS];
//    SpotLight* spotLights[MAX_SUPP_LIGHTS];
//};
//
