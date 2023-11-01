//#include "Button3DManager.h"
//#include "Scene.h"
//#include "../CameraManager.h"
//#include "DataBase.h"
//#include "../ControlPad.h"
//#include "../AudioManager.h"
//
//Button3DManager::Button3DManager() : frontNumber(1), isSelecting(IS_SELECTING::SELECTING)
//{
//}
//
//Button3DManager::~Button3DManager()
//{
//}
//
//void Button3DManager::update(float elapsed_time)
//{
//    if (!managerOn)  return;
//
//    ControlPad* controlPad = ControlPad::get();
//    CameraManager* cameraManager = CameraManager::get(); 
//
//    bool pressLeft = controlPad->PressLeft(0);
//    bool pressRight = controlPad->PressRight(0);
//    bool pressStart = controlPad->PressStart(0);
//
//    if (pressStart && isSelecting == Button3DManager::IS_SELECTING::SELECTING)
//    {
//        AudioManager::get()->play(EFFECT_1, EFFECT_SELECTCLICK);
//        setIsSelecting(Button3DManager::IS_SELECTING::DEFORMING);
//    }
//    else if ((pressLeft || pressRight) && isSelecting == Button3DManager::IS_SELECTING::SELECTING)
//    {
//        int nextFrontNumber = frontNumber;
//
//        if (pressLeft && frontNumber != 1)
//        {
//            AudioManager::get()->play(EFFECT_1, EFFECT_SELECTMOVE);
//            turn(90);
//            cameraManager->Change2DTo3D(1.0f);
//            --nextFrontNumber;
//        }
//        else if(pressRight && frontNumber != 10)
//        {
//            AudioManager::get()->play(EFFECT_1, EFFECT_SELECTMOVE);
//            turn(-90);
//            cameraManager->Change2DTo3D(1.0f);
//            ++nextFrontNumber;
//        }
//
//        for (int i = 0; i < getButtonsStage3DSize(); ++i)
//        {
//            VECTOR3 nextSca = i == nextFrontNumber - 1 ?
//                VECTOR3(1.0f, 1.0f, 1.0f) : VECTOR3(0.5f, 0.5f, 0.5f);
//            getButtonsStage3D(i).setNextSca(nextSca);
//        }
//
//        setFrontNumber(nextFrontNumber);
//    }
//    else
//    {
//        for (auto& button : buttonsStage3D)
//        {
//            button.update(elapsed_time);
//        }
//    }
//
//    if (isSelecting == IS_SELECTING::DEFORMING)
//        deformationSquare(frontNumber - 1);
//
//    if (isSelecting == IS_SELECTING::GO_STAGE)
//    {
//        CameraManager* cameraManager = CameraManager::get();
//        MyMath* myMath = MyMath::get();
//
//        Button3D& button = buttonsStage3D[static_cast<size_t>(frontNumber) - 1];
//        VECTOR3 buttonPos = button.getPos();
//        VECTOR3 buttonRot = button.getRot();
//        buttonPos.z += elapsed_time * squareComingSpeed;
//        buttonRot.z += elapsed_time * squareRotationSpeed;
//        button.setPos(buttonPos);
//        button.setRot(buttonRot);
//
//        VECTOR3 begin = cameraManager->getGameCameraPosition3();
//        VECTOR3 end = begin + myMath->ScaleVector3(myMath->Normalize(cameraManager->getGameCameraForwardVector3()), 2.0f);
//
//        if (button.RayCastReturnBool(begin, end))
//        {
//            SceneManager* sceneManager = SceneManager::get();
//            
//            sceneManager->changeScene(SCENEGAME, 0);
//            sceneManager->setStage(frontNumber);
//            
//        }
//    }
//}
//
//void Button3DManager::draw()
//{
//    if (!managerOn)  return;
//
//    for (auto& button : buttonsStage3D)
//    {
//        button.draw();
//    }
//}
//
//void Button3DManager::inportFile()
//{
//    for (auto& buttonStage3D : buttonsStage3D)
//    {
//        buttonStage3D.clear();
//    }
//    squarePos.clear();
//
//    const int stageMax = 10;
//
//    // make Button3D
//    for (int number = 1; number < stageMax + 1; ++number)
//    {
//        Button3D button3d;
//        button3d.setMyNumber(number);
//        std::string fileName = std::string("./Data/Save/select/selectNumber") + std::to_string(number) + std::string(".txt");
//        std::ifstream loadFile;
//        loadFile.open(fileName, std::ios::in);
//        if (loadFile)
//        {
//            // make block
//            while (1)
//            {
//                std::string saved_string[8];
//                bool close = false;
//
//                // nameId, typeId, 
//                // pos.x, pos.y, pos.z, 
//                // rot.x, rot.y, rot.z 
//                for (int i = 0; i < 8; ++i)
//                {
//                    std::getline(loadFile, saved_string[i], ',');
//                    if (saved_string[i] == "END")
//                    {
//                        close = true;
//                        break;
//                    }
//                }
//                if (close)
//                    break;
//                int nameId = std::stoi(saved_string[0]);    // nameId
//                int typeId = std::stoi(saved_string[1]);    // typeId
//                Block* block = DataBase::get()->CreateBlock(nameId, typeId);
//                
//                block->position_.x = std::stof(saved_string[2]);    // pos.x
//                block->position_.y = std::stof(saved_string[3]);    // pos.y
//                block->position_.z = std::stof(saved_string[4]);    // pos.z
//                block->rotation_.x = std::stof(saved_string[5]);    // rot.x
//                block->rotation_.y = std::stof(saved_string[6]);    // rot.y
//                block->rotation_.z = std::stof(saved_string[7]);    // rot.z
//                button3d.pushBlock(block);
//                loadFile.get();
//            }
//        }
//        loadFile.close();
//
//        buttonsStage3D.push_back(button3d);
//    }
//
//    // make square
//    {
//        std::string fileName = std::string("./Data/Save/select/sq.txt");
//        std::ifstream loadFile;
//        loadFile.open(fileName, std::ios::in);
//        if (loadFile)
//        {
//            // make block
//            while (1)
//            {
//                std::string saved_string[8];
//                bool close = false;
//
//                // nameId, typeId, 
//                // pos.x, pos.y, pos.z, 
//                // rot.x, rot.y, rot.z 
//                for (int i = 0; i < 8; ++i)
//                {
//                    std::getline(loadFile, saved_string[i], ',');
//                    if (saved_string[i] == "END")
//                    {
//                        close = true;
//                        break;
//                    }
//                }
//                if (close)
//                    break;
//
//                VECTOR3 Pos = {};
//
//                Pos.x = std::stof(saved_string[2]);    // pos.x
//                Pos.y = std::stof(saved_string[3]);    // pos.y
//                Pos.z = std::stof(saved_string[4]);    // pos.z
//                squarePos.push_back(Pos);
//                loadFile.get();
//            }
//        }
//        loadFile.close();
//    }
//}
//
//
//void Button3DManager::init()
//{
//    inportFile();
//    frontNumber = 1;
//    isSelecting = IS_SELECTING::SELECTING;
//    managerOn = true;
//
//    VECTOR3 pos = { 0.0f, 0.0f, 0.0f };
//    VECTOR3 rot = { 0.0f, 0.0f, 0.0f };
//    VECTOR3 sca = { 1.0f, 1.0f, 1.0f };
//    for (auto& buttonStage3D : buttonsStage3D)
//    {
//        if (buttonStage3D.getMyNumber() == frontNumber)
//        {
//            buttonStage3D.setSca({ 1, 1, 1 });
//            buttonStage3D.setNextSca({ 1, 1, 1 });
//        }
//        else
//        {
//            buttonStage3D.setSca({ 0.5f, 0.5f, 0.5f });
//            buttonStage3D.setNextSca({ 0.5f, 0.5f, 0.5f });
//        }
//
//        buttonStage3D.setPos(pos);
//        buttonStage3D.setInitRot(rot);
//        buttonStage3D.setSca(sca);
//
//        pos.x -= 70;
//        rot.y += 90;
//    }
//}
//
//void Button3DManager::clear()
//{
//    for (auto& buttonStage3D : buttonsStage3D)
//    {
//        buttonStage3D.clear();
//    }
//
//    buttonsStage3D.clear();
//    squarePos.clear();
//    isSelecting = IS_SELECTING::SELECTING;
//    managerOn = false;
//}
//
//void Button3DManager::turn(float angleY)
//{
//    for (auto& buttonStage3D : buttonsStage3D)
//    {
//        buttonStage3D.turn(angleY);
//    }
//}
//
//void Button3DManager::deformationSquare(int buttonIndex)
//{
//    if (0 <= buttonIndex && buttonIndex < buttonsStage3D.size())
//    {
//        buttonsStage3D[buttonIndex].deformationSquare();
//    }
//}