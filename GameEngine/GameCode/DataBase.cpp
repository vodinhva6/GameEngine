//#include "DataBase.h"
//#include "GameEngine.h"
//void DataBase::CreateAllObject()
//{
//    //GraphicEngine* graphicEngine = GameEngine::get()->getGraphicEngine();
//    
//    playerPos[0] = {0,5,0};
//    playerPos[1] = {0,5,0};
//    playerPos[2] = {0,5,0};
//    playerPos[3] = {0,5,0};
//    playerPos[4] = {0,5,0};
//    playerPos[5] = { 9.8f, -15, 0 }; 
//    playerPos[6] = { 0,5,0 };
//    playerPos[7] = {0,5,0};
//    playerPos[8] = {0,5,0};
//    playerPos[9] = {0,5,0};
//
//    ShaderData shaderData = { "./Data/Shaders/skinned_mesh_vs.cso", "./Data/Shaders/skinned_mesh_ps.cso" };
//    
//    
//
//    //skyBox_ = new Background;
//    //skyBox_->mesh_ = graphicEngine->createSkinnedMesh("./Data/Asset/3DObject/SkyBox/sky.fbx", true, SkinnedMeshObjectAxis::DEFAULT, shaderData);
//    //skyBox_->drawStates_.blendState = BlendStates::Alpha;
//    //skyBox_->drawStates_.depthState = DepthStencilStates::DepthTestOn_DepthWriteOn;
//    //skyBox_->drawStates_.rasterizerState = RasterizerStates::DrawTriangles_BackTrianglesOff_AntialiasedLineOn;
//    //skyBox_->drawStates_.samplerState = SamplerStates::Anisotropic;
//    //skyBox_->drawStates_.drawType = DrawType::SKYBOX;
//    //skyBox_->position_ = { 0,0,0 };
//    //skyBox_->speedRotation_ = { 0,0,0 };
//    //skyBox_->offSet_ = { 0,0.1f,0 };
//    //skyBox_->rotation_ = { 0,0,0 };
//    //skyBox_->color_ = { 1,1,1,1 };
//    //skyBox_->drawTurn_ = 0;
//    //skyBox_->scale_ = { 1,1,1 };
//    //skyBox_->size_ = 5;
//    //skyBox_->gravityValue = 0;
//    //
//    //
//    //
//    //
//    //player_ = new Player;
//    //player_->mesh_ = graphicEngine->createSkinnedMesh("./Data/Asset/3DObject/Player/player.fbx", false, SkinnedMeshObjectAxis::DEFAULT, shaderData, { 0,0.0f,0 });
//    //player_->position_ = { 0,10,0 };
//    //player_->speedRotation_ = { 0,0,0 };
//    //player_->offSet_ = { 0,0.1f,0 };
//    //player_->rotation_ = { 0,0,0 };
//    //player_->color_ = { 1,1,1,1 };
//    //player_->drawTurn_ = 9;
//    //player_->scale_ = { 1,1,1 };
//    //player_->size_ = 5.7f;
//    //player_->drawStates_.blendState = BlendStates::Alpha;
//    //player_->drawStates_.depthState = DepthStencilStates::DepthTestOn_DepthWriteOn;
//    //player_->drawStates_.rasterizerState = RasterizerStates::CullingOpposition;
//    //player_->drawStates_.samplerState = SamplerStates::Anisotropic;
//    //player_->gravityValue = -0.07f; 
//    //player_->animation.animat = &player_->mesh_->animation_clips.at(0);
//    //player_->animation.oldanimat = player_->animation.animat;
//    //player_->animation.key_ = &player_->animation.animat->sequence.at(0);
//    //player_->animation.frameStepSpeed = 1;
//    //player_->animation.oldFrameStepSpeed = 1;
//    //player_->animation.animationFrame = player_->animation.animat->sequence.size();
//    //player_->animation.trigger = false;
//    //player_->setAnimat(AnimatNum::Idle, 0.01f);
//    //VECTOR3 minVertex = player_->mesh_->VertexMinMaxInfor[0];
//    //VECTOR3 maxVertex = player_->mesh_->VertexMinMaxInfor[1];
//    //VECTOR3 Hsize = { fabsf(minVertex.x - maxVertex.x), fabsf(minVertex.y - maxVertex.y) ,fabsf(minVertex.z - maxVertex.z) };
//    //
//    //    
//    //player_->CreateRay({ 0,minVertex.y + Hsize.y / 2, minVertex.z / 3 },
//    //    { 0, minVertex.y - 0.01f,minVertex.z / 3 }, { 1,0,0,1 }, RayType::GroundRay);
//    //    
//    //player_->CreateRay({ 0,minVertex.y + Hsize.y / 2, maxVertex.z / 4 },
//    //    { 0, minVertex.y - 0.01f,maxVertex.z / 4 }, {1,0,0,1}, RayType::GroundRay);
//    //    
//    //player_->CreateRay({ 0,minVertex.y + Hsize.y / 8.0f,0 },
//    //    { 0,minVertex.y + Hsize.y / 8.0f,maxVertex.z + maxVertex.z / 8 }, { 1,1,1,1 }, RayType::WallRay);
//    //    
//    //player_->CreateRay({ 0,Hsize.y / 3,0 },
//    //    { 0,Hsize.y / 3,maxVertex.z + maxVertex.z / 8 }, { 1,1,1,1 }, RayType::WallRay);
//    //   
//    //player_->CreateRay({ 0,maxVertex.y - Hsize.y / 3,0 },
//    //    { 0,maxVertex.y - Hsize.y / 3,maxVertex.z + maxVertex.z / 8 }, { 1,1,1,1 }, RayType::WallRay);
//    //   
//    //player_->CreateRay({ 0,maxVertex.y - Hsize.y / 10.0f,0 },
//    //    { 0,maxVertex.y - Hsize.y / 10.0f,maxVertex.z + maxVertex.z / 8 }, { 1,1,1,1 }, RayType::WallRay);
//    //
//    //player_->CreateRay({ 0,maxVertex.y + 1,0 },
//    //    { 0,maxVertex.y + 1 ,maxVertex.z + 0.4f }, { 0,1,0,1 }, RayType::TakingBlockWallRay);
//    //
//    //player_->CreateRay({ 0,maxVertex.y + 1,0 },
//    //    { 0,maxVertex.y + 1 ,minVertex.z - 0.4f }, { 0,1,0,1 }, RayType::TakingBlockWallRay);
//    //
//    //player_->CreateRay({ 0,maxVertex.y + 0.9f,-0.8f },
//    //    { 0,maxVertex.y + 1.9f ,-0.8f }, { 0,1,0,1 }, RayType::TakingBlockUpRay);
//    //
//    //player_->CreateRay({ 0,maxVertex.y + 0.9f, 0.8f},
//    //    { 0,maxVertex.y + 1.9f , 0.8f }, { 0,1,0,1 }, RayType::TakingBlockUpRay);
//    //
//    //player_->CreateRay({ 0,(minVertex.y + maxVertex.y) / 3 * 2,maxVertex.z }, { 0,maxVertex.y,maxVertex.z }, { 0,0,1,1 }, RayType::UptopHeadRay);
//    //
//    //player_->CreateRay({ 0,(minVertex.y + maxVertex.y) / 3 * 2,minVertex.z }, { 0,maxVertex.y,minVertex.z }, { 0,0,1,1 }, RayType::UptopHeadRay);
//    //
//    //
//    //player_->updateWorldTrans();
//    //
//    //
//
//    // SceneStage
//#if 1
//    //stage1Button_ = new Stage1Button;
//    //stage1Button_->spr_ = graphicEngine->createSprite(L"./Data/Asset/Images/Sprites/1.png");
//    //stage1Button_->position_ = { 400, 500 };
//    //stage1Button_->scale_ = { 1, 1 };
//    //stage1Button_->color_ = { 1, 1, 1, 1 };
//    //stage1Button_->size_ = 1;
//    //stage1Button_->texturePosition_ = { 0, 0 };
//    //stage1Button_->textureSize_ = { 150, 150 };
//    //stage1Button_->hSize_ = { 150, 150 };
//    //
//    //stage2Button_ = new Stage2Button;
//    //stage2Button_->spr_ = graphicEngine->createSprite(L"./Data/Asset/Images/Sprites/2.png");
//    //stage2Button_->position_ = { 800, 500 };
//    //stage2Button_->scale_ = { 1, 1 };
//    //stage2Button_->color_ = { 1, 1, 1, 1 };
//    //stage2Button_->size_ = 1;
//    //stage2Button_->texturePosition_ = { 0, 0 };
//    //stage2Button_->textureSize_ = { 150, 150 };
//    //stage2Button_->hSize_ = { 150, 150 };
//    //
//    //stage3Button_ = new Stage3Button;
//    //stage3Button_->spr_ = graphicEngine->createSprite(L"./Data/Asset/Images/Sprites/3.png");
//    //stage3Button_->position_ = { 1200, 500 };
//    //stage3Button_->scale_ = { 1, 1 };
//    //stage3Button_->color_ = { 1, 1, 1, 1 };
//    //stage3Button_->size_ = 1;
//    //stage3Button_->texturePosition_ = { 0, 0 };
//    //stage3Button_->textureSize_ = { 150, 150 };
//    //stage3Button_->hSize_ = { 150, 150 };
//#endif
//
//
//    //GraphicEngine::get()->createEffect("./Data/Asset/3DObject/Effect/test.efkefc", "TEST");
//    //
//    //GraphicEngine::get()->createEffect("./Data/Asset/3DObject/Effect/goal_effect.efkefc", "GOAL");
//    //
//    //GraphicEngine::get()->createEffect("./Data/Asset/3DObject/Effect/warp_blue.efkefc", "SWAP_BLUE");
//    //GraphicEngine::get()->createEffect("./Data/Asset/3DObject/Effect/warp_blue_out.efkefc", "SWAP_BLUE_OUT");
//    //
//    //GraphicEngine::get()->createEffect("./Data/Asset/3DObject/Effect/warp_purple.efkefc", "SWAP_PURPLE");
//    //GraphicEngine::get()->createEffect("./Data/Asset/3DObject/Effect/warp_purple_out.efkefc", "SWAP_PURPLE_OUT");
//    //
//    //GraphicEngine::get()->createEffect("./Data/Asset/3DObject/Effect/warp_green.efkefc", "SWAP_GREEN");
//    //GraphicEngine::get()->createEffect("./Data/Asset/3DObject/Effect/warp_green_out.efkefc", "SWAP_GREEN_OUT");
//    //
//    //GraphicEngine::get()->createEffect("./Data/Asset/3DObject/Effect/warp_Yellow.efkefc", "SWAP_YELLOW");
//}
//
//DataBase::DataBase()
//{
//    
//}
//
//
////Rock* DataBase::createRock();
////{
////    if (!rock_) return nullptr;
////    Enemy* e = new Enemy(*enemy_);
////
////    return e;
////}
//
//Background* DataBase::CreateBackground()
//{
//    Background* back = new Background(*skyBox_);
//    return back;
//}
//
//Player* DataBase::CreatePlayer(int stage)
//{
//    Player* p = new Player(*player_);
//    p->setPosition(playerPos[stage-1]);
//    return p;
//}
//
//void DataBase::CreateSpotLight(SpotLight* spotlgt[10],int stageNum)
//{
//    for (int i = 0; i < 11; i++)
//    {
//        if (spotLights[i])
//        {
//            //spotlgt[i] = new SpotLight({ spotLights[i]->position.x,spotLights[i]->position.y,spotLights[i]->position.z },
//            //    { spotLights[i]->direction.x, spotLights[i]->direction.y, spotLights[i]->direction.z },
//            //    spotLights[i]->color, spotLights[i]->range, spotLights[i]->power, spotLights[i]->innerCorn, spotLights[i]->outerCorn);
//            //spotlgt[i]->position.x += mapOffsetPos[stageNum].x;
//            //spotlgt[i]->position.y += mapOffsetPos[stageNum].y;
//            //spotlgt[i]->position.z += mapOffsetPos[stageNum].z;
//        }
//        else break;
//    }
//}
//
//void DataBase::CreatePointLight(PointLight* pointlgt[10], int stageNum)
//{
//    for (int i = 0; i < 10; i++)
//    {
//        if (pointLights[i])
//        {
//            //pointlgt[i] = new PointLight({ pointLights[i]->position.x,pointLights[i]->position.y,pointLights[i]->position.z }, pointLights[i]->color, pointLights[i]->range, pointLights[i]->power);
//            //pointlgt[i]->position.x  += mapOffsetPos[stageNum].x;
//            //pointlgt[i]->position.y  += mapOffsetPos[stageNum].y;
//            //pointlgt[i]->position.z  += mapOffsetPos[stageNum].z;
//        }
//        else break;
//    }
//}
//
//
//DataBase::~DataBase()
//{
//    SafeDelete(player_);
//    SafeDelete(skyBox_);
//
//    for (int i = 0; i < MAX_SUPP_LIGHTS; i++)
//    {
//        if (pointLights[i])
//        {
//            SafeDelete(pointLights[i]);
//        }
//        if (spotLights[i])
//        {
//            SafeDelete(spotLights[i]);
//        }
//    }
//   
//}
