#pragma once
#include <string>
#include <memory>
#include <directxmath.h>
#include <d3d11.h>
#include <Cereal.h>
static const int MAX_BONE_INFLUENCES{ 4 };
static const int MAX_BONES{ 512 };
struct Skeleton
{
    struct BoneInfluence
    {
        uint32_t bone_index;
        float bone_weight;
    };


    struct Bone
    {
        uint64_t unique_id{ 0 };
        std::string name;
        // 'parent_index' is index that refers to the parent bone's position in the array that contains itself.
        int64_t parent_index{ -1 }; // -1 : the bone is orphan
        // 'node_index' is an index that refers to the node array of the scene.
        int64_t node_index{ 0 };

        // 'offset_transform' is used to convert from model(mesh) space to bone(node) scene.
        DirectX::XMFLOAT4X4 offset_transform{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

        bool is_orphan() const { return parent_index < 0; };
        template < class T>
        void serialize(T& archive)
        {
            archive(unique_id, name, parent_index, node_index, offset_transform);
        }

    };
    std::vector<Bone> bones;

    int64_t indexof(uint64_t unique_id) const
    {
        int64_t index{ 0 };
        for (const Bone& bone : bones)
        {
            if (bone.unique_id == unique_id)
            {
                return index;
            }
            ++index;
        }
        return -1;
    }
    template < class T>
    void serialize(T& archive)
    {
        archive(bones);
    }
};