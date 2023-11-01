#include "Animator.h"
#include "UsesImGui_h.h"
Animator::Animator()
{
    localfile = "";
    name = "";
}

void Animator::SetOnner(void* mesh)
{
}

float Animator::CalTimeLine(std::string clip, std::string sub, int frame)
{
    std::shared_ptr<Animation> anim;
    int begin, end = 0;
    FindFrame(&anim, clip, sub, begin, end);
    double frameCount = fabs(end - begin);
    return (float)(frame) / (float)(frameCount);
}

void Animator::FetchAnimations(FbxScene* fbx_scene, float sampling_rate)
{
    FbxArray<FbxString*> animation_stack_names;
    fbx_scene->FillAnimStackNameArray(animation_stack_names);
    const int animation_stack_count{ animation_stack_names.GetCount() };
    for (int animation_stack_index = 0; animation_stack_index < animation_stack_count; ++animation_stack_index)
    {
        std::shared_ptr<Animation> animation_clip = std::make_shared<Animation>();// { animationClips.emplace() };
        animation_clip->SetName(animation_stack_names[animation_stack_index]->Buffer());
    
        FbxAnimStack* animation_stack{ fbx_scene->FindMember<FbxAnimStack>(animation_clip->GetName().c_str())};
        fbx_scene->SetCurrentAnimationStack(animation_stack);
    
        const FbxTime::EMode time_mode{ fbx_scene->GetGlobalSettings().GetTimeMode() };
        FbxTime one_second;
        one_second.SetTime(0, 0, 1, 0, 0, time_mode);
        animation_clip->sampling_rate = sampling_rate > 0 ?
            sampling_rate : static_cast<float>(one_second.GetFrameRate(time_mode));
        const FbxTime sampling_interval
        { static_cast<FbxLongLong>(one_second.Get() / animation_clip->sampling_rate) };
        const FbxTakeInfo* take_info{ fbx_scene->GetTakeInfo(animation_clip->GetName().c_str())};
        const FbxTime start_time{ take_info->mLocalTimeSpan.GetStart() };
        const FbxTime stop_time{ take_info->mLocalTimeSpan.GetStop() };
        for (FbxTime time = start_time; time < stop_time; time += sampling_interval)
        {
            Animation::Keyframe& keyframe{ animation_clip->sequence.emplace_back() };
    
            const size_t node_count{ sceneView.nodes.size() };
            keyframe.nodes.resize(node_count);
            for (size_t node_index = 0; node_index < node_count; ++node_index)
            {
                FbxNode* fbx_node{ fbx_scene->FindNodeByName(sceneView.nodes.at(node_index).name.c_str()) };
                if (fbx_node)
                {
                    Animation::Keyframe::Node& node{ keyframe.nodes.at(node_index) };
                    // 'global_transform' is a transformation matrix of a node with respect to
                    // the scene's global coordinate system.
                    node.global_transform = to_xmfloat4x4(fbx_node->EvaluateGlobalTransform(time));
                    // 'local_transform' is a transformation matrix of a node with respect to
                    // its parent's local coordinate system.
                    const FbxAMatrix& local_transform{ fbx_node->EvaluateLocalTransform(time) };
                    node.scaling = to_xmfloat3(local_transform.GetS());
                    node.rotation = to_xmfloat4(local_transform.GetQ());
                    node.translation = to_xmfloat3(local_transform.GetT());
    
                }
    
            }
    
        }
        animationClips.insert(std::make_pair(animation_clip->GetName(), animation_clip));
    }
    for (int animation_stack_index = 0; animation_stack_index < animation_stack_count; ++animation_stack_index)
        delete animation_stack_names[animation_stack_index];
}

Animation::Keyframe* Animator::UpdateAnimation(Animation::Keyframe* key)
{;
    size_t node_count = key->nodes.size();
    for (size_t node_index = 0; node_index < node_count; ++node_index)
    {
        Animation::Keyframe::Node& node = key->nodes.at(node_index);
        DirectX::XMMATRIX S{  DirectX::XMMatrixScaling(node.scaling.x, node.scaling.y, node.scaling.z) };
        DirectX::XMMATRIX R{  DirectX::XMMatrixRotationQuaternion(XMLoadFloat4(&node.rotation)) };
        DirectX::XMMATRIX T{  DirectX::XMMatrixTranslation(node.translation.x, node.translation.y, node.translation.z) };
    
        int64_t parent_index = sceneView.nodes.at(node_index).parent_index;
        DirectX::XMMATRIX P{ parent_index < 0 ? DirectX::XMMatrixIdentity() :
            DirectX::XMLoadFloat4x4(&key->nodes.at(parent_index).global_transform) };
    
        DirectX::XMStoreFloat4x4(&node.global_transform, S * R * T * P);
    }
    return nullptr;
}
static int b = 0;
Animation::Keyframe* Animator::GetFeyframeAt(std::string clip, std::string sub, float timer, int& saveFrameNum)
{
   /* static float angleY1, angleX1, angleZ1 = 0;
    static int nodeInt1 = 8;
    static float angleY2, angleX2, angleZ2 = 0;
    static int nodeInt2 = 15;
    ImGui::Begin("Testa1");
    ImGui::SliderFloat("Angle X 1", &angleX1, -180, 180);
    ImGui::SliderFloat("Angle Y 1", &angleY1, -180, 180);
    ImGui::SliderFloat("Angle Z 1", &angleZ1, -180, 180);
    ImGui::SliderInt("NodeInt 1", &nodeInt1, 0, 375);
    ImGui::End();

    ImGui::Begin("Testa2");
    ImGui::SliderFloat("Angle X 2", &angleX2, -180, 180);
    ImGui::SliderFloat("Angle Y 2", &angleY2, -180, 180);
    ImGui::SliderFloat("Angle Z 2", &angleZ2, -180, 180);
    ImGui::SliderInt("NodeInt 2", &nodeInt2, 0, 375);
    ImGui::End();*/

    Animation::Keyframe* key = FindKey(clip, sub, timer, saveFrameNum);
    key = new Animation::Keyframe(*key);
    QuatumKeyFrame(key, listQuaNow, listNodeIndexNow, listValueNow);
    /*
    {
        DirectX::XMVECTOR V = DirectX::XMLoadFloat4(&key->nodes.at(nodeInt1).rotation);
        DirectX::XMVECTOR rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMConvertToRadians(angleY1));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), DirectX::XMConvertToRadians(angleX1));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMConvertToRadians(angleZ1));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        DirectX::XMStoreFloat4(&key->nodes.at(nodeInt1).rotation, V);
    }
    {
        DirectX::XMVECTOR V = DirectX::XMLoadFloat4(&key->nodes.at(nodeInt2).rotation);
        DirectX::XMVECTOR rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMConvertToRadians(angleY2));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), DirectX::XMConvertToRadians(angleX2));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        rotV = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMConvertToRadians(angleZ2));
        V = DirectX::XMQuaternionMultiply(V, rotV);
        DirectX::XMStoreFloat4(&key->nodes.at(nodeInt2).rotation, V);
    }*/

   /* DirectX::XMStoreFloat4(&key->nodes.at(nodeInt).rotation,
         DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMConvertToRadians(angle)));
    key->nodes.at(nodeInt).translation.x = 0;*/
    ImGui::Begin("ss");
    ImGui::Text(std::to_string(b).c_str());
    ImGui::End();
    UpdateAnimation(key);
    return key;
}

Animation::Keyframe* Animator::BlendAnimations(std::string clipOld, std::string subOld, float timeLine,
    std::string clip, std::string sub, float factor, int& saveFrameNum)
{
    Animation::Keyframe* keyframeOld = FindKey(clipOld, subOld, timeLine, saveFrameNum);
    keyframeOld = new Animation::Keyframe(*keyframeOld);
    QuatumKeyFrame(keyframeOld, listQuaOld, listNodeIndexOld, listValueOld);
   
    b = saveFrameNum;
    ImGui::Begin("ss");
    ImGui::Text(std::to_string(b).c_str());
    ImGui::End();

    int frameTo = FindBlendToFrame(subOld, sub, saveFrameNum);
    Animation::Keyframe* keyframeNew = FindKey(clip, sub, frameTo, saveFrameNum);
    keyframeNew = new Animation::Keyframe(*keyframeNew);
    QuatumKeyFrame(keyframeNew, listQuaNext, listNodeIndexNext, listValueNext);
   
     
    size_t node_count{ keyframeOld->nodes.size() };
    blendFrame.nodes.resize(node_count);
    for (size_t node_index = 0; node_index < node_count; ++node_index)
    {
        DirectX::XMVECTOR S[2]{
        DirectX::XMLoadFloat3(&keyframeOld->nodes.at(node_index).scaling),
         DirectX::XMLoadFloat3(&keyframeNew->nodes.at(node_index).scaling) };
        DirectX::XMStoreFloat3(&blendFrame.nodes.at(node_index).scaling, DirectX::XMVectorLerp(S[0], S[1], factor));

        DirectX::XMVECTOR R[2]{
        DirectX::XMLoadFloat4(&keyframeOld->nodes.at(node_index).rotation),
        DirectX::XMLoadFloat4(&keyframeNew->nodes.at(node_index).rotation) };
        DirectX::XMStoreFloat4(&blendFrame.nodes.at(node_index).rotation, DirectX::XMQuaternionSlerp(R[0], R[1], factor));

        DirectX::XMVECTOR T[2]{ XMLoadFloat3(&keyframeOld->nodes.at(node_index).translation),
        DirectX::XMLoadFloat3(&keyframeNew->nodes.at(node_index).translation) };
        DirectX::XMStoreFloat3(&blendFrame.nodes.at(node_index).translation, DirectX::XMVectorLerp(T[0], T[1], factor));
        
        DirectX::XMMATRIX M1 = DirectX::XMLoadFloat4x4(&keyframeOld->nodes.at(node_index).global_transform);
        DirectX::XMMATRIX M2 = DirectX::XMLoadFloat4x4(&keyframeNew->nodes.at(node_index).global_transform);
        DirectX::XMMATRIX M3 = DirectX::XMMatrixIdentity();
        for (int i = 0; i < 4; i++) 
            for (int j = 0; j < 4; j++)
            {
                float f1 = M1.r[i].m128_f32[j];
                float f2 = M2.r[i].m128_f32[j];
                M3.r[i].m128_f32[j] = f1 + (f2 - f1) * factor;
            }
        

        DirectX::XMStoreFloat4x4(&blendFrame.nodes.at(node_index).global_transform, M3);
    }
    return &blendFrame;
}

void Animator::AppendAnimations(const char* animation_filename, float sampling_rate, bool newCereal)
{
    std::filesystem::path cereal_filename(animation_filename);
    cereal_filename.replace_extension("ant");
    if (std::filesystem::exists(cereal_filename.c_str()) && !newCereal)
    {
        std::ifstream ifs(cereal_filename.c_str(), std::ios::binary);
        cereal::BinaryInputArchive deserialization(ifs);
        //deserialization(this);
    }
    else
    {
        FbxManager* fbx_manager{ FbxManager::Create() };
        FbxScene* fbx_scene{ FbxScene::Create(fbx_manager, "") };

        FbxImporter* fbx_importer{ FbxImporter::Create(fbx_manager, "") };
        bool import_status{ false };
        import_status = fbx_importer->Initialize(animation_filename);
        _ASSERT_EXPR(import_status, fbx_importer->GetStatus().GetErrorString());
        import_status = fbx_importer->Import(fbx_scene);
        _ASSERT_EXPR(import_status, fbx_importer->GetStatus().GetErrorString());
        FetchAnimations(fbx_scene, sampling_rate);

        fbx_manager->Destroy();
        std::ofstream ofs(cereal_filename.c_str(), std::ios::binary);
        cereal::BinaryOutputArchive serialization(ofs);
        serialization(animationClips);
    }

}

void Animator::CreateNewCereal()
{
    std::ofstream ofs(localfile.c_str(), std::ios::binary);
    cereal::BinaryOutputArchive serialization(ofs);
    serialization(localfile, name, animationClips, sceneView, animationMap, blendKeyMap);
}

void Animator::LoadDataFromCereal(std::filesystem::path local)
{
    std::ifstream ifs(local.c_str(), std::ios::binary);
    cereal::BinaryInputArchive deserialization(ifs);
    deserialization(localfile, name, animationClips, sceneView, animationMap, blendKeyMap);
}

void Animator::SetNextBlendAnimation( std::vector<VECTOR4>* listQua,  std::vector<int>* listNodeIndex,  std::vector<float>* listValue)
{
   listQuaNext = listQua;
   listNodeIndexNext = listNodeIndex;
   listValueNext = listValue;
}

void Animator::SetNowBlendAnimation(std::vector<VECTOR4>* listQua, std::vector<int>* listNodeIndex, std::vector<float>* listValue)
{
    listQuaNow = listQua;
    listNodeIndexNow = listNodeIndex;
    listValueNow = listValue;
}

void Animator::SetOldBlendAnimation(std::vector<VECTOR4>* listQua, std::vector<int>* listNodeIndex, std::vector<float>* listValue)
{
    listQuaOld = listQua;
    listNodeIndexOld = listNodeIndex;
    listValueOld = listValue;
}

Animator::~Animator()
{
}

Animation::Keyframe* Animator::FindKey(std::string clip, std::string sub, float timeLine, int& saveFrameNum)
{
    int begin, end = 0;
    std::shared_ptr<Animation> anim;
    FindFrame(&anim, clip, sub, begin, end);
    double frameCount = fabs(end - begin);
    int frame = begin + (int)(timeLine * frameCount);
    saveFrameNum = frame - begin;
    //static int t = 0;
    //ImGui::Begin("Testa");
    //ImGui::SliderInt("NodeInt", &t, begin, end);
    //ImGui::End();
    return &anim->sequence[frame];
}

Animation::Keyframe* Animator::FindKey(std::string clip, std::string sub, int frame, int& saveFrameNum)
{
    int begin, end = 0;
    std::shared_ptr<Animation> anim;
    FindFrame(&anim, clip, sub, begin, end);
    frame = begin + frame; 
    saveFrameNum = frame - begin;
    //static int t = 0;
    //ImGui::Begin("Testa");
    //ImGui::SliderInt("NodeInt", &t, begin, end);
    //ImGui::End();
    return &anim->sequence[frame];
}

void Animator::FindFrame(std::shared_ptr<Animation>* anim, std::string clip, std::string sub, int& begin, int& end)
{
    if(anim)
    *anim = animationClips.find(clip)->second;
    auto thisClip = animationMap.find(clip);
    if (thisClip == animationMap.end())
        return;
    auto thisSub = thisClip->second.find(sub);
    if (thisSub == thisClip->second.end())
        return;

    begin = thisSub->second.at(Animator::AnimationPlayArea::BEGIN);
    end = thisSub->second.at(Animator::AnimationPlayArea::END);
}

Animation::Keyframe* Animator::QuatumKeyFrame(Animation::Keyframe* key, const std::vector<VECTOR4>* listQua, const std::vector<int>* listNodeIndex, const std::vector<float>* listValue)
{
    if (listQua)
    {
        for (int i = 0; i < (int)listQua->size(); i++)
        {
            auto& it = key->nodes.at(listNodeIndex->at(i)).rotation;
            DirectX::XMVECTOR V1 = DirectX::XMLoadFloat4(&it);
            DirectX::XMVECTOR V2 = DirectX::XMLoadFloat4(&listQua->at(i));
            DirectX::XMStoreFloat4(&it, DirectX::XMQuaternionSlerp(V1, DirectX::XMQuaternionMultiply(V1, V2), listValue->at(i)));
        }
        UpdateAnimation(key);
    }
        
    return key;
}

int Animator::FindBlendToFrame(std::string subBegin, std::string subTo, int keyBegin)
{
    int keyT = 0;
    if (blendKeyMap.empty()) return keyT;
    auto subB = blendKeyMap.find(subBegin);
    if (subB == blendKeyMap.end()) return keyT;
    auto subT = subB->second.find(subTo);
    if (subT == subB->second.end()) return keyT;
    keyT = subT->second.at(keyBegin);
    return keyT;
}
