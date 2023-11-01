#include "AnimatorManager.h"

AnimatorManager::AnimatorManager()
{
}

void AnimatorManager::CreateNewAnimatorFormModelFile(FbxScene* fbxScene, SceneMesh* sceneMesh, std::filesystem::path localfile, float samplingRate)
{
    std::filesystem::path localFileFix;
    std::shared_ptr<Animator> animator = std::make_shared<Animator>();
    CreateFolder(localfile, localFileFix);
    if (CheckHadFile(localfile, localFileFix))
        animator->LoadDataFromCereal(localFileFix);
    else
    {
        if (sceneMesh->nodes.size() == 0)
        {
            FbxGeometryConverter fbx_converter(fbxScene->GetFbxManager());
            std::function<void(FbxNode*)> traverse
            { [&](FbxNode* fbx_node)
                {
                    SceneMesh::Node& node{ sceneMesh->nodes.emplace_back() };
                    node.attribute = fbx_node->GetNodeAttribute() ? fbx_node->GetNodeAttribute()->GetAttributeType() : FbxNodeAttribute::EType::eUnknown;
                    node.name = fbx_node->GetName();
                    node.unique_id = fbx_node->GetUniqueID();
                    node.parent_index = sceneMesh->indexof(fbx_node->GetParent() ? fbx_node->GetParent()->GetUniqueID() : 0);

                    for (int child_index = 0; child_index < fbx_node->GetChildCount(); ++child_index)
                    {
                        traverse(fbx_node->GetChild(child_index));
                    }
            }
            };
            traverse(fbxScene->GetRootNode());
        }
        animator->sceneView = *sceneMesh;
        animator->localfile = localFileFix.string();
        animator->name = localFileFix.filename().string();


        CanLoadAnimationData(fbxScene, animator, samplingRate);
        CreateNewAnimatorCereal(animator);

    }
    CreateNewAnimationCereal(animator, localfile);
    RegsiterAnimator(animator);

}

std::shared_ptr<Animator> AnimatorManager::CreateNewAnimator(std::filesystem::path local)
{
    std::shared_ptr<Animator> animator;
    animator.reset(new Animator);
    animator->name = GetNotUsedName(local, "NewAnimator");


    std::filesystem::path cerealFileLocal = local.string() + std::string("/");
    cerealFileLocal.replace_filename(animator->name);
    cerealFileLocal.replace_extension("anitor");
    animator->localfile = cerealFileLocal.string();
    animator->name = cerealFileLocal.filename().string();
    animator->CreateNewCereal();

    animators.insert(std::make_pair(cerealFileLocal, animator));
    return animator;
}

bool AnimatorManager::LoadAnimator(std::filesystem::path local, std::shared_ptr<Animator>& animator)
{
    if (CheckHadAnimator(local, animator))
        return true;
    animator = std::make_shared<Animator>();
    animator->LoadDataFromCereal(local);
    RegsiterAnimator(animator);
    return true;
}

bool AnimatorManager::LoadAnimation(std::filesystem::path local, std::shared_ptr<Animation>& animation)
{
    if (CheckHadAnimation(local, animation))
        return true;
    animation = std::make_shared<Animation>();
    animation->LoadDataFromCereal(local);
    RegsiterAnimation(animation);
    return true;
}

AnimatorManager::~AnimatorManager()
{
    if (buildAnimatorThread.get())
        buildAnimatorThread->join();
    buildAnimatorThread.reset();
    if (buildAnimationThread.get())
        buildAnimationThread->join();
    buildAnimationThread.reset();
    animators.clear();
}

void AnimatorManager::CreateNewAnimatorCereal(std::shared_ptr<Animator>& animator)
{
    BuildCerealAnimator(animator);
}

void AnimatorManager::CreateNewAnimationCereal(std::shared_ptr<Animator>& animator, std::filesystem::path localFile)
{
    localFile.replace_filename("Anime/Animation/");
    if (!std::filesystem::exists(localFile))
    {
        std::filesystem::create_directory(localFile);
    }
    for (std::unordered_map<std::string, std::shared_ptr<Animation>>::iterator it = animator->animationClips.begin();
        it != animator->animationClips.end(); it++)
    {
        std::string anitorName = animator->name;
        anitorName = anitorName.substr(0, anitorName.find_first_of("."));
        std::string animName = "(" + anitorName + ")" + it->second->GetName();
        it->second->SetName(animName);

        localFile.replace_filename(animName);
        localFile.replace_extension(".anim");
        it->second->local = localFile.string();
        RegsiterAnimation(it->second);
        animator->animationClips.insert(std::make_pair(animName, it->second));
        it = animator->animationClips.erase(it);
        if (std::filesystem::exists(localFile))
            continue;
        BuildCerealAnimation(it->second);
    }
    //for (auto& animation : animator->animationClips)
    //{
    //    std::string anitorName = animator->name;
    //    anitorName = anitorName.substr(0, anitorName.find_first_of("."));
    //    std::string animName = "(" + anitorName + ")" + animation.second->GetName();
    //    animation.second->SetName(animName);
    //   
    //    localFile.replace_filename(animName);
    //    localFile.replace_extension(".anim");
    //    animation.second->local = localFile.string();
    //    RegsiterAnimation(animation.second);
    //    if (std::filesystem::exists(localFile))
    //        continue;
    //    BuildCerealAnimation(animation.second);
    //}

}

void AnimatorManager::RegsiterAnimator(std::shared_ptr<Animator>& animator)
{
    animators.insert(std::make_pair(animator->localfile, animator));
}

void AnimatorManager::RegsiterAnimation(std::shared_ptr<Animation>& animation)
{
    animations.insert(std::make_pair(animation->local, animation));
}

bool AnimatorManager::CheckHadFile(std::filesystem::path local, std::filesystem::path& fix)
{
    fix += "Animator/";
    if (!std::filesystem::exists(fix))
    {
        std::filesystem::create_directory(fix);
    }

    fix += local.filename().string();
    fix.replace_extension("anitor");

    if (std::filesystem::exists(fix))
        return true;
    return false;
}

bool AnimatorManager::CanLoadAnimationData(FbxScene* fbxScene, std::shared_ptr<Animator>& animator, float samplingRate)
{
    FbxArray<FbxString*> animation_stack_names;
    fbxScene->FillAnimStackNameArray(animation_stack_names);
    const int animation_stack_count{ animation_stack_names.GetCount() };
    if (animation_stack_count == 0) 
        return false;
    for (int animation_stack_index = 0; animation_stack_index < animation_stack_count; ++animation_stack_index)
    {
        std::shared_ptr<Animation> animation_clip = std::make_shared<Animation>();// { animator->animationClips.emplace_back() };
        animation_clip = std::make_shared<Animation>();
        animation_clip->SetName(animation_stack_names[animation_stack_index]->Buffer());

        FbxAnimStack* animation_stack{ fbxScene->FindMember<FbxAnimStack>(animation_clip->GetName().c_str())};
        fbxScene->SetCurrentAnimationStack(animation_stack);

        const FbxTime::EMode time_mode{ fbxScene->GetGlobalSettings().GetTimeMode() };
        FbxTime one_second;
        one_second.SetTime(0, 0, 1, 0, 0, time_mode);
        animation_clip->sampling_rate = samplingRate > 0 ?
            samplingRate : static_cast<float>(one_second.GetFrameRate(time_mode));
        const FbxTime sampling_interval
        { static_cast<FbxLongLong>(one_second.Get() / animation_clip->sampling_rate) };
        const FbxTakeInfo* take_info{ fbxScene->GetTakeInfo(animation_clip->GetName().c_str())};
        const FbxTime start_time{ take_info->mLocalTimeSpan.GetStart() };
        const FbxTime stop_time{ take_info->mLocalTimeSpan.GetStop() };
        for (FbxTime time = start_time; time < stop_time; time += sampling_interval)
        {
            Animation::Keyframe& keyframe{ animation_clip->sequence.emplace_back() };

            const size_t node_count{ animator->sceneView.nodes.size() };
            keyframe.nodes.resize(node_count);
            for (size_t node_index = 0; node_index < node_count; ++node_index)
            {
                FbxNode* fbx_node{ fbxScene->FindNodeByName(animator->sceneView.nodes.at(node_index).name.c_str()) };
                if (fbx_node)
                {
                    Animation::Keyframe::Node& node{ keyframe.nodes.at(node_index) };

                    node.global_transform = to_xmfloat4x4(fbx_node->EvaluateGlobalTransform(time));

                    const FbxAMatrix& local_transform{ fbx_node->EvaluateLocalTransform(time) };
                    node.scaling = to_xmfloat3(local_transform.GetS());
                    node.rotation = to_xmfloat4(local_transform.GetQ());
                    node.translation = to_xmfloat3(local_transform.GetT());

                }

            }

        }
        animator->animationClips.insert(std::make_pair(animation_clip->GetName(), animation_clip));
    }
    for (int animation_stack_index = 0; animation_stack_index < animation_stack_count; ++animation_stack_index)
    {
        delete animation_stack_names[animation_stack_index];
    }
    return true;
}

void AnimatorManager::CreateFolder(std::filesystem::path localfile, std::filesystem::path& fix)
{
    fix = localfile;
    fix.replace_filename("Anime/");
    if (!std::filesystem::exists(fix))
    {
        std::filesystem::create_directory(fix);
    }
}

bool AnimatorManager::CheckHadAnimator(std::filesystem::path dir, std::shared_ptr<Animator>& animator)
{
    auto it = animators.find(dir);
    if (it != animators.end())
    {
        animator = it->second;
        return true;
    }
    return false;
}

bool AnimatorManager::CheckHadAnimation(std::filesystem::path local, std::shared_ptr<Animation>& animation)
{
    auto it = animations.find(local);
    if (it != animations.end())
    {
        animation = it->second;
        return true;
    }
    return false;
}

std::string AnimatorManager::GetNotUsedName(std::filesystem::path local, std::string fileName)
{
    int count = 0;
    std::filesystem::path checkName = local.string() + "/" + fileName;
    checkName.replace_extension("anitor");
    while (std::filesystem::exists(checkName))
    {
        count++;
        checkName = local.string() + "/" + fileName + "(" + std::to_string(count) + ")";
        checkName.replace_extension("anitor");
    }

    return checkName.filename().string();
}

void AnimatorManager::BuildCerealAnimator(std::shared_ptr<Animator>& animator)
{
    if (buildAnimatorThread.get())
        buildAnimatorThread->join();
    buildAnimatorThread.reset();
    buildAnimatorThread = std::make_unique<std::thread>(&Animator::CreateNewCereal, animator.get());
}

void AnimatorManager::BuildCerealAnimation(std::shared_ptr<Animation>& animation)
{
    if (buildAnimationThread.get())
        buildAnimationThread->join();
    buildAnimationThread.reset();
    buildAnimationThread = std::make_unique<std::thread>(&Animation::CreateNewCereal, animation, animation->local);
}

bool AnimatorManager::ReleaseAnimator(std::filesystem::path local)
{
    auto it = animators.find(local);
    if (it != animators.end())
    {
        int count = it->second.use_count();
        if (count == 1)
        {
            it->second.reset();
            it = animators.erase(it);
        }

        return true;
    }
    return false;
}

bool AnimatorManager::ReleaseAnimation(std::filesystem::path local)
{
    auto it = animations.find(local);
    if (it != animations.end())
    {
        int count = it->second.use_count();
        if (count == 1)
        {
            it->second.reset();
            it = animations.erase(it);
        }

        return true;
    }
    return false;
}

bool AnimatorManager::DeleteFileAnimator(std::filesystem::path local)
{
    auto it = animators.find(local);
    if (it != animators.end())
    {
        it->second.reset();
        it = animators.erase(it);
        return true;
    }
    return false;
}

bool AnimatorManager::RenameAnimator(std::filesystem::path oldLocal, std::filesystem::path newLocal)
{
    auto it = animators.find(oldLocal);
    std::shared_ptr<Animator> pAnimator;
    if (it != animators.end())
    {
        pAnimator = it->second;
        it = animators.erase(it);
        pAnimator->name = newLocal.filename().string();
        pAnimator->localfile = newLocal.string();
        animators.insert(std::make_pair(newLocal, pAnimator));
        BuildCerealAnimator(pAnimator);
        return true;
    }

    return false;
}

bool AnimatorManager::RenameAnimation(std::filesystem::path oldLocal, std::filesystem::path newLocal)
{
    auto it = animations.find(oldLocal);
    std::shared_ptr<Animation> pAnimation;
    if (it != animations.end())
    {
        pAnimation = it->second;
       
        it = animations.erase(it);
        std::string name = newLocal.filename().string();
        name = name.substr(0, name.find_first_of("."));
        pAnimation->SetName(name);
        pAnimation->local = newLocal.string();
        animations.insert(std::make_pair(newLocal, pAnimation));

        for (auto& anitor : animators)
        {
            bool ok = false;
            for (std::unordered_map<std::string, std::shared_ptr<Animation>>::iterator it = anitor.second->animationClips.begin();
                it != anitor.second->animationClips.end(); it++)
                if (pAnimation == it->second)
                {
                    anitor.second->animationClips.insert(std::make_pair(pAnimation->GetName(), pAnimation));
                    it = anitor.second->animationClips.erase(it);
                    ok = true;
                    break;
                }
            if (ok) break;
        }
           
        BuildCerealAnimation(pAnimation);
       
        return true;
    }
    return false;
}
