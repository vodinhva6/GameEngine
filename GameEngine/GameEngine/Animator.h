#ifndef ANIMATOR_H
#define ANIMATOR_H
#include "Animation.h"
#include "DataFormat.h"
#include <unordered_map>
struct SceneMesh
{
    struct Node
    {
        uint64_t unique_id{ 0 };
        std::string name;
        FbxNodeAttribute::EType attribute{ FbxNodeAttribute::EType::eUnknown };
        int64_t parent_index{ -1 };
        template < class T>
        void serialize(T& archive)
        {
            archive(unique_id, name, attribute, parent_index);
        }

    };
    std::vector<Node> nodes;
    template < class T>
    void serialize(T& archive)
    {
        archive(nodes);
    }
    int64_t indexof(uint64_t unique_id) const
    {
        int64_t index{ 0 };
        for (const Node& node : nodes)
        {
            if (node.unique_id == unique_id)
            {
                return index;
            }
            ++index;

        }
        return -1;

    }

};

class Animator
{
public:
    enum AnimationPlayArea
    {
        BEGIN,
        END
    };
public:
    Animator();
    void SetOnner(void* mesh);
    float CalTimeLine(std::string clip, std::string sub, int frame);
    void FetchAnimations(FbxScene* fbx_scene, float sampling_rate);
    Animation::Keyframe* UpdateAnimation(Animation::Keyframe*);
    Animation::Keyframe* GetFeyframeAt(std::string clip, std::string sub, float timer, int& saveFrameNum);
    
    Animation::Keyframe* BlendAnimations(std::string clipOld, std::string subOld, float timeLine,
        std::string clip, std::string sub, float factor, int& saveFrameNum);
    void AppendAnimations(const char* animation_filename, float sampling_rate, bool newCereal);
    void CreateNewCereal();
    void LoadDataFromCereal(std::filesystem::path);
    void SetNextBlendAnimation( std::vector<VECTOR4>* listQua,  std::vector<int>* listNodeIndex,
         std::vector<float>* listValue);
    void SetNowBlendAnimation(std::vector<VECTOR4>* listQua, std::vector<int>* listNodeIndex,
        std::vector<float>* listValue);
    void SetOldBlendAnimation(std::vector<VECTOR4>* listQua, std::vector<int>* listNodeIndex,
    std::vector<float>* listValue);
    ~Animator();

private:
    Animation::Keyframe* FindKey(std::string clip, std::string sub, float timeLine, int& saveFrameNum);
    Animation::Keyframe* FindKey(std::string clip, std::string sub, int frame, int& saveFrameNum);
    void FindFrame(std::shared_ptr<Animation>* anim,std::string clip, std::string sub, int& begin, int& end);

    Animation::Keyframe* QuatumKeyFrame(Animation::Keyframe* key, const std::vector<VECTOR4>* listQua = nullptr, const std::vector<int>* listNodeIndex = nullptr,
        const std::vector<float>* listValue = nullptr);
    int FindBlendToFrame(std::string subBegin, std::string subTo, int keyBegin);
public:
    template < class T>
    void serialize(T& archive)
    {
        archive(localfile, name, animationClips, sceneView, animationMap, blendKeyMap);
    }
public:
    std::string localfile;
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<Animation>> animationClips;
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<int>>> animationMap;
    std::unordered_map < std::string, std::unordered_map<std::string, std::vector<int>>> blendKeyMap;
    
    Animation::Keyframe blendFrame;
    SceneMesh sceneView;


    const std::vector<VECTOR4>* listQuaNext;
    const std::vector<int>* listNodeIndexNext;
    const std::vector<float>* listValueNext;

    const std::vector<VECTOR4>* listQuaNow;
    const std::vector<int>* listNodeIndexNow;
    const std::vector<float>* listValueNow;

    const std::vector<VECTOR4>* listQuaOld;
    const std::vector<int>* listNodeIndexOld;
    const std::vector<float>* listValueOld;
};


#endif // !ANIMATOR_H

