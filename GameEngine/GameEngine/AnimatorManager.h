#pragma once
#ifndef ANIMATORMANAGER_H
#define ANIMATORMANAGER_H
#include "Animator.h"
#include <filesystem>
#include <thread>
#include <memory>
class AnimatorManager
{
public:
    AnimatorManager();
    void CreateNewAnimatorFormModelFile(FbxScene* fbxScene, SceneMesh* sceneMesh, std::filesystem::path localfile, float samplingRate);
    std::shared_ptr<Animator> CreateNewAnimator(std::filesystem::path);
    bool LoadAnimator(std::filesystem::path, std::shared_ptr<Animator>& );
    bool LoadAnimation(std::filesystem::path, std::shared_ptr<Animation>&);
    void BuildCerealAnimator(std::shared_ptr<Animator>& animator);
    void BuildCerealAnimation(std::shared_ptr<Animation>& animation);
    bool ReleaseAnimator(std::filesystem::path);
    bool ReleaseAnimation(std::filesystem::path);

    bool DeleteFileAnimator(std::filesystem::path);
    bool RenameAnimator(std::filesystem::path oldLocal, std::filesystem::path newLocal);
    bool RenameAnimation(std::filesystem::path oldLocal, std::filesystem::path newLocal);

    void RegsiterAnimator(std::shared_ptr<Animator>&);
    void RegsiterAnimation(std::shared_ptr<Animation>&);
    ~AnimatorManager();

private:

    std::unordered_map<std::filesystem::path, std::shared_ptr<Animator>> animators;
    std::unordered_map<std::filesystem::path, std::shared_ptr<Animation>> animations;
    std::unique_ptr<std::thread> buildAnimatorThread;
    std::unique_ptr<std::thread> buildAnimationThread;
    void CreateNewAnimatorCereal(std::shared_ptr<Animator>&);
    void CreateNewAnimationCereal(std::shared_ptr<Animator>& animator, std::filesystem::path);
   
    bool CheckHadFile(std::filesystem::path, std::filesystem::path&);
    bool CanLoadAnimationData(FbxScene* fbxScene, std::shared_ptr<Animator>& animator, float samplingRate);
    void CreateFolder(std::filesystem::path, std::filesystem::path& );
    bool CheckHadAnimator(std::filesystem::path dir, std::shared_ptr<Animator>& animator);
    bool CheckHadAnimation(std::filesystem::path, std::shared_ptr<Animation>&);
    std::string GetNotUsedName(std::filesystem::path local, std::string fileName);

};




#endif // !ANIMATORMANAGER_H

