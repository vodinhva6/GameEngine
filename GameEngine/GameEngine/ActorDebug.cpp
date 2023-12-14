#include "ActorManager.h"
#include "Collision.h"
#include "CameraManager.h"
#include "GameEngine.h"
#include "MousePoint.h"
#include "ControlPad.h"
#include "ContentBrowser.h"
#include "MaterialManager.h"
#include "LightManager.h"
void ActorDebug::Init()
{
    
    tagert = nullptr;
    operation = 0;
}


void ActorDebug::Update()
{
#ifdef USE_IMGUI
    ActorNodeTable();
    ControlPad* padP = GetFrom<ControlPad>(GameEngine::get()->getControlPad());
    if (padP->PressT(0))
        operation = ImGuizmo::OPERATION::TRANSLATE;
    if (padP->PressS(0))
        operation = ImGuizmo::OPERATION::ROTATE;
    if(padP->PressR(0))
        operation = ImGuizmo::OPERATION::SCALE;

    ImGui::Begin("Actor Debug");

    if (tagert)
    {
        OBJ3D* obj = dynamic_cast<OBJ3D*>(tagert);
        Debug3DOBJ3D(obj);
        
    }
    ImGui::End();
#endif // USE_IMGUI

}

void ActorDebug::Draw()
{
    if (tagert)
    {
      
    }
}

ActorDebug::~ActorDebug()
{


}

void ActorDebug::getTagert()
{
#ifdef USE_IMGUI
    MousePoint* mouseP = GetFrom<MousePoint>(GameEngine::get()->getMousePoint());
    ControlPad* padP = GetFrom<ControlPad>(GameEngine::get()->getControlPad());
    ActorManager* actorManager = GetFrom<ActorManager>(GameEngine::get()->getActorManager());
    CameraManager* cameraManager = GetFrom<CameraManager>(GameEngine::get()->getCameraManager());
   
    if(ImGui::IsWindowHovered())
    if (padP->PressA(0))
    {
        VECTOR2 mousePos = mouseP->getPos();
        std::vector<std::shared_ptr<Actor>>& list = actorManager->getActorList();
        const VECTOR3 cameraPos = cameraManager->getDebugCameraPosition();
        float tagertDis = D3D11_FLOAT32_MAX;
        bool hit = false;
        for (std::shared_ptr<Actor>& it : list)
        {
            tagert3D = dynamic_cast<OBJ3D*>(it.get());
            VECTOR3 pos = {};
            if (tagert3D)
                pos = Collision::get()->RayCastMousePointReturnPoint(mousePos - windowPos, *tagert3D, hit);
            if (hit)
            {
                float disTemp = MyMath::get()->Length(cameraPos - pos);
                if (disTemp < tagertDis)
                {
                    tagert = it.get();
                    tagertDis = disTemp;

                }

            }
        }
        if (tagertDis == D3D11_FLOAT32_MAX)
        {
            tagert = nullptr;
            tagert3D = nullptr;
            tagert2D = nullptr;
        }
    }
#endif // USE_IMGUI
}

bool ActorDebug::setTagert()
{
#ifdef USE_IMGUI
    MousePoint* mouseP = GetFrom<MousePoint>(GameEngine::get()->getMousePoint());
    ControlPad* padP = GetFrom<ControlPad>(GameEngine::get()->getControlPad());
    if (tagert)
    {
        bool deleteActor = false;
        ImGui::Begin("Actor Nodes");
        deleteActor = (ImGui::IsWindowHovered());
        ImGui::End();
        deleteActor ^= (ImGui::IsWindowHovered());
        deleteActor &= padP->PressDelete(0);
        if (deleteActor)
        {
            tagert->setDeadFlag(true);
            tagert = nullptr;
            return false;
        }
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImVec2 size = ImGui::GetWindowSize();
        ImVec2 pos = ImGui::GetWindowPos();
        VECTOR2 windowSize = *((VECTOR2*)&size);
        VECTOR2 windowPos = *((VECTOR2*)&pos);

        ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);

        CameraManager* cameraManager = GetFrom<CameraManager>(GameEngine::get()->getCameraManager());
        float viewDir[16];
        DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(viewDir), cameraManager->GetDebugCameraView3D());
        float projectionDir[16];
        DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(projectionDir), cameraManager->GetDebugProjection3D());
       
        OBJ3D* obj = dynamic_cast<OBJ3D*>(tagert);
        DirectX::XMFLOAT4X4& worldTransform = obj->getWorldTransform();
        memcpy(tagertTransform, &worldTransform, sizeof(worldTransform));

        ImGuizmo::Manipulate(viewDir, projectionDir, (ImGuizmo::OPERATION)operation, ImGuizmo::LOCAL, tagertTransform);

       if(ImGuizmo::IsUsing())
           return true;
      
    }
    return false;
#else
    return false;
#endif // USE_IMGUI
}

bool ActorDebug::Transforming(DirectX::XMVECTOR& translation)
{
    VECTOR3 transla;
    DirectX::XMStoreFloat3(&transla, translation);
    OBJ3D* obj = dynamic_cast<OBJ3D*>(tagert);
    obj->setPosition(transla);
    return true;
}

bool ActorDebug::Scaling(DirectX::XMVECTOR& scaling)
{
    VECTOR3 scale;
    DirectX::XMStoreFloat3(&scale, scaling);
    OBJ3D* obj = dynamic_cast<OBJ3D*>(tagert);
    obj->setScale(scale);
    return true;
}

bool ActorDebug::Rotating(DirectX::XMVECTOR& rotationQuat)
{
    DirectX::XMMATRIX RMatrix = DirectX::XMMatrixRotationQuaternion(rotationQuat);
    DirectX::XMFLOAT4X4 R;
    DirectX::XMStoreFloat4x4(&R, DirectX::XMMatrixTranspose(RMatrix));

    VECTOR3 rot;
    rot.x = MyMath::get()->convertToDegrees((float)asin(-R._23));
    rot.y = MyMath::get()->convertToDegrees((float)atan2(R._13, R._33));
    rot.z = MyMath::get()->convertToDegrees((float)atan2(R._21, R._22));

    OBJ3D* obj = dynamic_cast<OBJ3D*>(tagert);
    VECTOR3 oldRot = obj->getRotation();
    obj->addRotation(rot - oldRot);
    return true;
}

void ActorDebug::UpdateDebugMaterial(OBJ3D* obj)
{
#ifdef USE_IMGUI
    ImGui::Indent();
    SkinnedMesh* pSkinnedMesh = obj->meshInfor.mesh_.get();
    if (!pSkinnedMesh)
        return;
    static size_t itemCurrent = 0;
    static size_t selected = -1;


    std::vector<const char*> listMeshName;
    for (auto& mesh : pSkinnedMesh->getMeshRawList())
    {
        listMeshName.push_back(mesh.name.c_str());
    }
    
    
    ImGui::Combo("Mesh", (int*)&itemCurrent, &(listMeshName[0]), (int)listMeshName.size());
    
        if (itemCurrent >= listMeshName.size())
            itemCurrent = listMeshName.size() - 1;
    MeshRaw* mesh = nullptr;
    mesh = &pSkinnedMesh->getMeshRawList().at(itemCurrent);
    
    if (selected >= mesh->subsets.size())
        selected = mesh->subsets.size() - 1;
    int i = 0;
    for (auto& subset : mesh->subsets)
    {
        ImGui::Text(std::string("Subset Material Id : " + std::to_string(subset.materialUniqueId)).c_str());
        auto it = obj->meshInfor.materials_.find(subset.materialUniqueId);
        std::shared_ptr<Material> material;
        if (it == obj->meshInfor.materials_.end())
            material = obj->meshInfor.materials_.at(0);
        else material = it->second;
        std::string name = material->getMaterialName();
        if (ImGui::Selectable(name.c_str(), selected == i))
        {
            ContentBrowser* contentBrowser = GetFrom<ContentBrowser>(GameEngine::get()->getContentBrowser());
            std::filesystem::path path = material->getFileLocal();
            contentBrowser->SetItemSelect(path);
            selected = i;
        }
        
        std::string payload_n = "";
        payload_n = GameEngine::get()->DropData("Material");
        if (payload_n != "")
        {
            pSmartVoid material;
            if (GameEngine::get()->LoadMaterial(payload_n, material))
            {
                std::shared_ptr<Material> pMaterial = GetFromPoint<Material>(material);
                obj->meshInfor.materials_.insert(std::make_pair(pMaterial->getUniqueId(), pMaterial));
                //GameEngine::get()->RebuildMeshCerealByMaterial(material);
                break;
            }
        }

        i++;
    }


    //for (auto& subset : mesh->subsets)
    //{
    //    
    //   
    //    std::string subsetMaterialName = "Subset Material Name: " + subset.material_name;
    //   
    //    if (ImGui::Button(subsetMaterialName.c_str()))
    //    {
    //        std::filesystem::path path = GameEngine::get()->getMaterialLocal(SetToPoint(subset.material));
    //        contentBrowser->SetContentDirectory(path.parent_path());
    //        contentBrowser->SetItemSelect(path);
    //        contentBrowser->setDirChange(true);
    //    }
    //  
    //    std::string payload_n = "";
    //    payload_n = GameEngine::get()->DropData("Material");
    //    if (payload_n != "")
    //    {
    //        pSmartVoid material;
    //        if (GameEngine::get()->LoadMaterial(payload_n, material))
    //        {
    //            subset.material = GetFromPoint<Material>(material);
    //            GameEngine::get()->RebuildMeshCerealByMaterial(material);
    //            break;
    //        }
    //    }
    //}
    ImGui::Unindent();
#endif // USE_IMGUI
}
void ActorDebug::UpdateDebugMeshes(OBJ3D* obj)
{
#ifdef USE_IMGUI
    ImGui::Indent();
    ContentBrowser* contentBrowser = GetFrom<ContentBrowser>(GameEngine::get()->getContentBrowser());
    auto& listMesh = obj->meshInfor.mesh_->getMeshRawList();

    int i = 0;
    int size = (int)listMesh.size();
    for (i = 0; i < size; i++)
    {
        auto mesh = listMesh.at(i);
        ImGui::Text(std::to_string(i).c_str());
        ImGui::SameLine();
        ImGui::Button(mesh.name.c_str());
        ImGui::SameLine();
        ImGui::PushID(i);
        if (ImGui::ImageButton(contentBrowser->getTrashFile().Get(), {20.0f,20.0f}))
        {
            ImGui::PopID();
            break;
        }
        ImGui::PopID();
    }

    if (i < size)
        listMesh.erase(listMesh.begin() + i);

    ImGui::Button("Add+");
    std::string meshLocal = contentBrowser->DropData("Mesh");
    if (meshLocal != "")
    {
        pSmartVoid mesh;
        GameEngine::get()->LoadSkinnedMesh(meshLocal, mesh);
        std::shared_ptr<SkinnedMesh> pMesh = GetFromPoint<SkinnedMesh>(mesh);
        auto& pMeshRawList = pMesh->getMeshRawList();
        for (auto& meshRaw : pMeshRawList)
        {
            MeshRaw& raw = listMesh.emplace_back();
            raw = meshRaw;
        }
        

    }
    ImGui::Unindent();


    ImGui::Unindent();
#endif // USE_IMGUI
}

void ActorDebug::Debug3DOBJ3D(OBJ3D* obj)
{
    if (obj)
    {
        LocalOfResource(obj);
        ChangeClass(obj);
        EditName(obj);
        DebugTransform(obj);
        DebugTexturing(obj);
        DebugAnimation(obj);
        DebugPointLight(obj);
    }
  
}
void ActorDebug::LocalOfResource(OBJ3D* obj)
{
#ifdef USE_IMGUI
  
    if (obj->meshInfor.mesh_.get())
    {
        static std::filesystem::path fileLocal = "";
        static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> imageMesh;
        ContentBrowser* content = GetFrom<ContentBrowser>(GameEngine::get()->getContentBrowser());
        if (fileLocal != obj->meshInfor.mesh_->getLocalPath())
        {
            fileLocal = obj->meshInfor.mesh_->getLocalPath();
            content->GetThumbnailMesh(obj->meshInfor.mesh_, imageMesh);
        }
            
        if (ImGui::ImageButton(imageMesh.Get(), { 50, 50 }))
        {
            content->SetItemSelect(std::filesystem::path(fileLocal));
            //content->SetContentDirectory(obj->mesh_->getLocalPath());
        }
    }
       //if (obj->sprite3D_.get())
       //{
       //
       //}
#endif // USE_IMGUI
}
void ActorDebug::DebugTexturing(OBJ3D* obj)
{
#ifdef USE_IMGUI
   
    if (ImGui::CollapsingHeader("Material"))
        UpdateDebugMaterial(obj);
    if (ImGui::CollapsingHeader("Meshes"))
        UpdateDebugMeshes(obj);

#endif // USE_IMGUI

}

void ActorDebug::DebugTransform(OBJ3D* obj)
{
#ifdef USE_IMGUI
    if (ImGuizmo::IsUsing())
    {
        DirectX::XMFLOAT4X4 trans;
        memcpy(trans.m, tagertTransform, sizeof(float) * 16);
        DirectX::XMVECTOR scaling, rotationQuat, translation;
        DirectX::XMMatrixDecompose(&scaling, &rotationQuat, &translation, DirectX::XMLoadFloat4x4(&trans));

        switch (operation)
        {
        case ImGuizmo::TRANSLATE:
        {
            Transforming(translation);
            break;
        }
        case ImGuizmo::ROTATE:
        {
            Rotating(rotationQuat);
            break;
        }

        case ImGuizmo::SCALE:
        {
            Scaling(scaling);
            break;
        }

        default:
            break;
        }

       
    }
   
    if (ImGui::CollapsingHeader("Transform"))
    {
        ImGui::Indent();
        {
            VECTOR3 position = obj->getPosition();
            VECTOR3 scale = obj->getScale();
            VECTOR3 rot = obj->getRotation();   

            ImGui::DragFloat3("Position", &position.x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
            ImGui::DragFloat3("Scale", &scale.x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
            ImGui::DragFloat3("Rotation", &rot.x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");

            obj->setRotation(rot);
            obj->setScale(scale);
            obj->setPosition(position);

        }
        ImGui::Unindent();
    }
#endif // USE_IMGUI
}

void ActorDebug::DebugAnimation(OBJ3D* obj)
{
#ifdef USE_IMGUI
    SkinnedMesh* pSkinnedMesh = obj->meshInfor.mesh_.get();
    if (!pSkinnedMesh)
        return;
    if (ImGui::CollapsingHeader("Animator"))
    {
        ImGui::Indent();
        {
            static size_t itemCurrent = 0;
            std::shared_ptr<Animator> pAnimator = obj->meshInfor.animator_;
            ContentBrowser* content = GetFrom<ContentBrowser>(GameEngine::get()->getContentBrowser());
            std::string name = "NON-Animator";
            if (pAnimator.get())
                name = pAnimator->name;
            if (ImGui::Button(name.c_str()) && pAnimator.get())
                content->SetItemSelect(pAnimator->localfile);
            std::string payload_n = "";
            payload_n = GameEngine::get()->DropData("Animator");
            if (payload_n != "")
            {
                pSmartVoid animator;
                if (GameEngine::get()->LoadAnimator(payload_n, animator))
                {
                    pAnimator = GetFromPoint<Animator>(animator);
                    obj->meshInfor.animator_ = pAnimator;
                }
            }

        }
        ImGui::Unindent();
    }

#endif // USE_IMGUI
}

void ActorDebug::EditName(OBJ3D* obj)
{
#ifdef USE_IMGUI
    if (ImGui::CollapsingHeader("EditName"))
    {
        char editname[256] = "";
        ImGui::Indent();
        if (ImGui::InputText("name", editname, sizeof(editname), ImGuiInputTextFlags_EnterReturnsTrue))
            obj->setName(editname);
        ImGui::Unindent();
    }
#endif // USE_IMGUI
}

void ActorDebug::ChangeClass(OBJ3D* obj)
{
#ifdef USE_IMGUI
    if (ImGui::CollapsingHeader("ChageClass"))
    {
        ImGui::Indent();

        static size_t itemCurrent = 0;

        std::vector<const char*> listClassChange =
        {
            "OBJ3D",
            "Player",
            "Enemy"
        };


        ImGui::Combo("Class", (int*)&itemCurrent, &(listClassChange[0]), (int)listClassChange.size());
       
        if (ImGui::Button("Change"))
        {
            
            switch (itemCurrent)
            {
            case 1:
            {
                std::shared_ptr<Actor> o = std::make_shared<Player>(*obj);
                ActorManager* actorManager = GetFrom<ActorManager>(GameEngine::get()->getActorManager());
                actorManager->AddListActor(o);
            }
            default:
                break;
            }
        }

        ImGui::Unindent();
    }
#endif // USE_IMGUI
}

void ActorDebug::DebugPointLight(OBJ3D* obj)
{
#ifdef USE_IMGUI
    if (obj->getTypeOBJ3D() != OBJ3DType::POINTLIGHT)
        return;
    PointLight* pL = dynamic_cast<PointLight*>(obj);
    if (ImGui::CollapsingHeader("Point Light"))
    {
        float& range = pL->getRange();
        float& power = pL->getPower();
        VECTOR4& color = pL->getColor();
        bool& renWithSph = pL->getRenderWithSphere();
        bool& shadowCast = pL->getShadowCast();
        int& volumentricLightScatteringSample = pL->getVolumentricLightScatteSample();
        float& Density  = pL->getDensity();
        float& Weight   = pL->getWeight();
        float& Decay    = pL->getDecay();
        float& Exposure = pL->getExposure();

        int& shadowSampleCount = pL->getShadowSampleCount();
        float& shadowDepthBias = pL->getShadowDepthBias();
        float& shadowColor = pL->getShadowColor();
        float& shadowFilterRadius = pL->getShadowFilterRadius();
       




        ImGui::DragFloat("Range", &range, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
        ImGui::DragFloat("Power", &power, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
        ImGui::ColorEdit4("Color", &color.x);
       
        ImGui::Checkbox("Render With Sphere", &renWithSph);
        if (renWithSph)
        {
            ImGui::Indent();
            ImGui::DragInt("VolumentricLightScatteringSample", &volumentricLightScatteringSample, 1, 0, INT_MAX);
            ImGui::DragFloat("Denstity", &Density, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
            ImGui::DragFloat("Weight", &Weight, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
            ImGui::DragFloat("Decay", &Decay, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
            ImGui::DragFloat("Exposure", &Exposure, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
            ImGui::Unindent();
        }
        ImGui::Checkbox("ShadowCast ", &shadowCast);
        if (shadowCast)
        {
            ImGui::Indent();
            ImGui::DragInt("ShadowSampleCount", &shadowSampleCount, 1, 0, 64);
            ImGui::DragFloat("ShadowDepthBias", &shadowDepthBias, 0.000001f, 0.0f, 0.01f, "%.8f");
            ImGui::SliderFloat("ShadowColor", &shadowColor, 0.0f, 1.0f);
            ImGui::SliderFloat("ShadowFilterRadius", &shadowFilterRadius, 0.0f, 64.0f);
            ImGui::Unindent();
        }


        //pL->setColor(color);
        //pL->setRange(range);
        //pL->setPower(power);
        //pL->setRenderWithSphere(renWithSph);
        //pL->setShadowCast(shadowCast);
    }

#endif // USE_IMGUI
}

void ActorDebug::DisplayNode()
{
#ifdef USE_IMGUI
    ActorManager* actorManager = GetFrom<ActorManager>(GameEngine::get()->getActorManager());
    std::vector<std::shared_ptr<Actor>>& listActor = actorManager->getActorList();
    for (auto& node : listActor)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        if (SelectNode(node.get()))
        {
            tagert = node.get();
        }

       
    }
#endif // USE_IMGUI
}

bool ActorDebug::SelectNode(Actor* actor)
{
#ifdef USE_IMGUI
    std::string type = actor->getActorTypeString();
    std::string name = actor->getActorName();
    bool a = false;
    if (actor == tagert)
        a = true;
    if (ImGui::Selectable(name.c_str(), &a))
    {
        a = true;
    }
    ImGui::TableNextColumn();
    ImGui::Text(type.c_str());
    return a;
#else
    return false;
#endif // USE_IMGUI
}

void ActorDebug::ActorNodeTable()
{
#ifdef USE_IMGUI
   
    ImGui::Begin("Actor Nodes");
    ImVec2 windowSize = ImGui::GetContentRegionMax();
    VECTOR2 windowsize = *((VECTOR2*)&windowSize);
    static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
    if (ImGui::BeginTable("3ways", 2, flags, { windowSize.x-5,windowSize.y-10 }, 5))
    {
        // The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
        ImGui::TableHeadersRow();

        DisplayNode();

        ImGui::EndTable();
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
    {
        ImGui::OpenPopup("Create Actor");
    }

    if (ImGui::BeginPopup("Create Actor"))
    {
        bool disable = true;
        if (ImGui::BeginMenu("Create"))
        {
            if (ImGui::MenuItem("Create Point Light"))
            {
                CreatePointLight();
            }
            ImGui::EndMenu();
        }
        ImGui::Separator();
        //if (ImGui::MenuItem("Copy", nullptr, nullptr, disable))
        //{
        //}
        //if (ImGui::MenuItem("Paste", nullptr, nullptr, disable))
        //{
        //}
        //if (ImGui::MenuItem("Delete", nullptr, nullptr, disable))
        //{
        //    DeleteFileMaterial();
        //}
        //if (ImGui::MenuItem("Rename", nullptr, nullptr, disable))
        //{
        //    renameEdit = selectItem;
        //
        //}
        ImGui::EndPopup();
    }

    ImGui::End();
#endif // USE_IMGUI
}

void ActorDebug::CreatePointLight()
{
    LightManager* lightManager = GetFrom<LightManager>(GameEngine::get()->getLightManager());
    lightManager->CreatePointLight();
}
