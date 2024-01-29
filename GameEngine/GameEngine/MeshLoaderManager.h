#pragma once
#include <Mesh.h>
#include <thread>
class MeshLoaderManager
{
public:
    MeshLoaderManager();
    bool CreateMesh(pSmartVoid& mesh, std::filesystem::path);
    bool ReleaseMesh(std::shared_ptr<Meshes>& mesh);
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
    void FetchRawMeshes(FbxScene* fbx_scene, std::shared_ptr<Meshes>& mesh);
    void FetchSkeletonMeshes(FbxScene* fbx_scene, std::shared_ptr<Meshes>& mesh);
    void FetchSkeleton(FbxMesh* fbx_mesh, Skeleton& bind_pose);
    void FetchBoneInfluences(const FbxMesh* fbx_mesh, std::vector<std::vector<Skeleton::BoneInfluence>>& bone_influences);
    void FetchMaterials(FbxScene* fbx_scene, std::filesystem::path local, std::unordered_map<int64_t, std::shared_ptr<Material>>& listMaterial);
    void FetchBouding(FbxMesh* fbx_mesh, std::shared_ptr<BaseMesh> mesh);
private:
    SceneMesh* sceneView;
    FbxManager* fbxManager;
    std::map<std::filesystem::path, std::shared_ptr<Meshes>> skinnedMeshes;
    std::unique_ptr<std::thread> rebuildThread;

    bool IsFbxHasMesh(std::filesystem::path local, FbxScene*&);
    void LoadScene(FbxScene*);
};

