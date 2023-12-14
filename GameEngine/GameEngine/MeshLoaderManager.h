#pragma once
#include "SkinnedMesh.h"
#include <thread>
class MeshLoaderManager
{
public:
    bool CreateMesh(pSmartVoid& mesh, std::filesystem::path);
    bool ReleaseMesh(std::shared_ptr<SkinnedMesh>& mesh);
    bool ReleaseMesh(std::filesystem::path local);
    bool RebuildMeshCereal(pSmartVoid& mesh);
    bool RegisterMesh(pSmartVoid& mesh, std::unordered_map<int64_t, std::shared_ptr<Material>>& listMaterial);
    bool SetDefaultTransform(std::filesystem::path fileLocal, const VECTOR3& posOff, const VECTOR3& rotOff, const VECTOR3& scaOff);
    ~MeshLoaderManager();
private:
    void DeleteAllMesh();
    bool CheckHadMesh(pSmartVoid& mesh, std::filesystem::path);
    bool IsCanToLoadMesh(std::filesystem::path, pSmartVoid& mesh);

private:
    void FetchMeshes(FbxScene* fbx_scene, std::vector<SkeletonMesh>& meshes);
    void FetchSkeleton(FbxMesh* fbx_mesh, Skeleton& bind_pose);
    void FetchBoneInfluences(const FbxMesh* fbx_mesh, std::vector<std::vector<Skeleton::BoneInfluence>>& bone_influences);
    void FetchMaterials(FbxScene* fbx_scene, std::filesystem::path local, std::unordered_map<int64_t, std::shared_ptr<Material>>& listMaterial);
    void FetchBouding(FbxMesh* fbx_mesh, MeshRaw& mesh);
private:
    SceneMesh* sceneView;
    std::map<std::filesystem::path, std::shared_ptr<SkinnedMesh>> skinnedMeshes;
    std::unique_ptr<std::thread> rebuildThread;

};

