#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H
#include <string>
#include <memory>
#include <directxmath.h>
#include <d3d11.h>
#include <Cereal.h>
#include <fbxsdk.h>
#include <filesystem>
#include <fstream>

inline DirectX::XMFLOAT4X4 to_xmfloat4x4(const FbxAMatrix& fbxamatrix)
{
    DirectX::XMFLOAT4X4 xmfloat4x4;
    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            xmfloat4x4.m[row][column] = static_cast<float>(fbxamatrix[row][column]);
        }
    }

    return xmfloat4x4;
}
inline DirectX::XMFLOAT3 to_xmfloat3(const FbxDouble3& fbxdouble3)
{
    DirectX::XMFLOAT3 xmfloat3;
    xmfloat3.x = static_cast<float>(fbxdouble3[0]);
    xmfloat3.y = static_cast<float>(fbxdouble3[1]);
    xmfloat3.z = static_cast<float>(fbxdouble3[2]);
    return xmfloat3;
}
inline DirectX::XMFLOAT4 to_xmfloat4(const FbxDouble4& fbxdouble4)
{
    DirectX::XMFLOAT4 xmfloat4;
    xmfloat4.x = static_cast<float>(fbxdouble4[0]);
    xmfloat4.y = static_cast<float>(fbxdouble4[1]);
    xmfloat4.z = static_cast<float>(fbxdouble4[2]);
    xmfloat4.w = static_cast<float>(fbxdouble4[3]);
    return xmfloat4;
}



class Animation
{
private:
    std::string name;
public:
    void CreateNewCereal(std::filesystem::path);
    void LoadDataFromCereal(std::filesystem::path);
    void SetName(std::string name);
    std::string GetName() { return name; }
public:
    std::string local;
   
    float sampling_rate{ 0 };
    class Keyframe
    {
    public:
        struct Node
        {
            // 'global_transform' is used to convert from local space of node to global space of scene.
            DirectX::XMFLOAT4X4 global_transform{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
            // The transformation data of a node includes its translation, rotation and scaling vectors
            // with respect to its parent. 
            DirectX::XMFLOAT3 scaling{ 1, 1, 1 };
            DirectX::XMFLOAT4 rotation{ 0, 0, 0, 1 }; // Rotation quaternion
            DirectX::XMFLOAT3 translation{ 0, 0, 0 };
            template < class T>
            void serialize(T& archive)
            {
                archive(global_transform, scaling, rotation, translation);
            }
        };
        std::vector<Node> nodes;
        template < class T>
        void serialize(T& archive)
        {
            archive(nodes);
        }
    };


    std::vector<Keyframe> sequence;


    template < class T>
    void serialize(T& archive)
    {
        archive(local, name, sampling_rate, sequence);
    }
};

#endif // !ANIMATION_H

