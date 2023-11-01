//#include "Button3D.h"
//#include "Collision.h"
//#include "MousePoint.h"
//#include "Button3DManager.h"
//#include "CameraManager.h"
//#include "Scene.h"
//
//
//void Button3D::update(float elapsed_time)
//{
//    MousePoint* mouse = MousePoint::get();
//    Button3DManager* button3DManager = Button3DManager::get();
//    CameraManager* cameraManager = CameraManager::get();
//
//    int frontNumber = button3DManager->getFrontNumber();
//    isTurn = TURN::NO;
//    Button3DManager::IS_SELECTING isSelecting = button3DManager->getIsSelecting();
//
//    updateTrans();
//
//    for (auto& block : buttonBlocks)
//    {
//        block->update(elapsed_time, transform);
//
//        if (mouse->onClick() && frontNumber - 1 <= myNumber && myNumber <= frontNumber + 1 && isSelecting == Button3DManager::IS_SELECTING::SELECTING)
//        {
//            if (Collision::get()->RayCastMousePointReturnBool(mouse->getPos(), block))
//            {
//                if (frontNumber == myNumber)    // number click
//                {
//                    button3DManager->setIsSelecting(Button3DManager::IS_SELECTING::DEFORMING);
//                    AudioManager::get()->play(EFFECT_1, EFFECT_SELECTCLICK);
//                    break;
//                }
//                else // arrow click
//                {
//
//                    if (frontNumber < myNumber)
//                    {
//                        isTurn = TURN::RIGHT;
//
//                    }
//                    else
//                    {
//                        isTurn = TURN::LEFT;
//                    }
//                    AudioManager::get()->play(EFFECT_1, EFFECT_SELECTMOVE);
//                    for (int i = 0; i < button3DManager->getButtonsStage3DSize(); ++i)
//                    {
//                        VECTOR3 nextSca = i == myNumber - 1 ?
//                            VECTOR3(1.0f, 1.0f, 1.0f) : VECTOR3(0.5f, 0.5f, 0.5f);
//                        button3DManager->getButtonsStage3D(i).setNextSca(nextSca);
//                    }
//
//                    button3DManager->setFrontNumber(myNumber);
//                    break;
//                }
//            }
//        }
//    }
//
//    switch (isTurn)
//    {
//    case TURN::LEFT:
//        Button3DManager::get()->turn(90);
//        cameraManager->Change2DTo3D(1.0f);
//        break;
//    case TURN::RIGHT:
//        Button3DManager::get()->turn(-90);
//        cameraManager->Change2DTo3D(1.0f);
//        break;
//    default:
//        break;
//    }
//
//    if (isSelecting != Button3DManager::IS_SELECTING::GO_STAGE)
//    {
//        rot = MyMath::get()->Lerp(rot, nextRot, 0.1f);
//        sca = MyMath::get()->Lerp(sca, nextSca, 0.1f);
//    }
//
//    if (fabsf(rot.y - nextRot.y) < 5 && isSelecting == Button3DManager::IS_SELECTING::SELECTING)
//    {
//        cameraManager->Change3DTo2D(0.2f);
//    }
//}
//
//void Button3D::draw()
//{
//    for (auto& block : buttonBlocks)
//    {
//        block->draw();
//    }
//}
//
//void Button3D::pushBlock(Block* block)
//{
//    buttonBlocks.push_back(block);
//}
//
//void Button3D::clear()
//{
//    for (auto& buttonBlock : buttonBlocks)
//    {
//        delete buttonBlock;
//    }
//
//    buttonBlocks.clear();
//}
//
//void Button3D::turn(float turnAngleY)
//{
//    this->turnAngleY += turnAngleY;
//    nextRot.y += turnAngleY;
//}
//
//void Button3D::deformationSquare()
//{
//    using IS_SELEC = Button3DManager::IS_SELECTING;
//    Button3DManager* button3DManager = Button3DManager::get();
//
//    size_t sqSize = button3DManager->getSquarePosSize();
//    VECTOR3 buttonPos = {};
//    VECTOR3 sqPos = {};
//
//    IS_SELEC isSelecting = IS_SELEC::GO_STAGE;
//
//    for (int i = 0; i < buttonBlocks.size(); ++i)
//    {
//        sqPos = i < sqSize ? button3DManager->getSquarePos(i) : VECTOR3(0, 0, 20);
//        buttonPos = buttonBlocks[i]->position_ = MyMath::get()->Lerp(buttonBlocks[i]->position_, sqPos, 0.2f);
//        if (0.01f < fabsf(buttonPos.x - sqPos.x) || 
//            0.01f < fabsf(buttonPos.y - sqPos.y) || 
//            0.01f < fabsf(buttonPos.z - sqPos.z))
//        {
//            isSelecting = IS_SELEC::DEFORMING;
//        }
//    }
//
//    if (isSelecting == IS_SELEC::GO_STAGE)
//    {
//        button3DManager->setIsSelecting(isSelecting);
//        CameraManager::get()->Change2DTo3D(0.0001f);
//    }
//}
//
//bool Button3D::RayCastReturnBool(const VECTOR3& begin, const VECTOR3& end)
//{
//    bool hit = false;
//
//    for (auto& block : buttonBlocks)
//    {
//        Collision::get()->RayCastReturnPoint(block, begin, end, hit);
//        if (hit)
//            return true;
//    }
//
//    return false;
//}
//
//void Button3D::updateTrans()
//{   
//    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(sca.x, sca.y, sca.z);
//    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(
//        DirectX::XMConvertToRadians(initRot.x + rot.x),
//        DirectX::XMConvertToRadians(initRot.y + rot.y),
//        DirectX::XMConvertToRadians(initRot.z + rot.z));
//    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
//
//    DirectX::XMStoreFloat4x4(&transform, S * R * T);
//}