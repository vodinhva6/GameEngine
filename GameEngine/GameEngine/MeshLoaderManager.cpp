#include "MeshLoaderManager.h"
#include "MaterialManager.h"
#include "AnimatorManager.h"
#include "GameEngine.h"
#include "Log.h"


bool MeshLoaderManager::CreateMesh(pSmartVoid& mesh, std::filesystem::path local)
{
   
    if (CheckHadMesh(mesh, local))
        return true;
    if (!IsCanToLoadMesh(local, mesh))
        return false;


    std::shared_ptr<SkinnedMesh> pSkinnedMesh = GetFromPoint<SkinnedMesh>(mesh);
    //pSkinnedMesh.reset(new SkinnedMesh(GameEngine::get()->getDevice(), loadMeshInfor.local.string().c_str(), loadMeshInfor.createNewCereal, loadMeshInfor.shaderData));
    
    
    skinnedMeshes.insert(std::make_pair(local, std::make_shared<SkinnedMesh>(*pSkinnedMesh)));
    mesh = SetToPoint<SkinnedMesh>(pSkinnedMesh);
    return true;
}

bool MeshLoaderManager::ReleaseMesh(std::shared_ptr<SkinnedMesh>& mesh)
{
    for (auto& it : skinnedMeshes)
    {
        if (it.second.get() == mesh.get())
        {
            int count = it.second.use_count();
            if (count == 1)
                skinnedMeshes.erase(it.first);
            return true;
        }
    }
    return false;
}

bool MeshLoaderManager::ReleaseMesh(std::filesystem::path local)
{
    auto it = skinnedMeshes.find(local);
    if (it != skinnedMeshes.end())
    {
        int count = it->second.use_count();
        if (count == 1)
            it = skinnedMeshes.erase(it);
          
        return true;
    }
    return false;
}

bool MeshLoaderManager::RegisterMesh(pSmartVoid& mesh, std::unordered_map<int64_t, std::shared_ptr<Material>>& listMaterial)
{
    bool result = false;
    MaterialManager* materialmanager = GetFrom<MaterialManager>(GameEngine::get()->getMaterialManager());
    std::shared_ptr<SkinnedMesh> pSkinnedMesh = GetFromPoint<SkinnedMesh>(mesh);
    if(!pSkinnedMesh.get())
        if (CheckHadMesh(mesh, pSkinnedMesh->getLocalPath()))
            result = true;
    std::filesystem::path path = pSkinnedMesh->getLocalPath();
    std::shared_ptr<Material>& dummyMaterial = materialmanager->getDummyMaterial();
    listMaterial.at(dummyMaterial->getUniqueId()) = dummyMaterial;
   
    materialmanager->RegisterAllMaterialsFromMeshData(mesh, listMaterial);
    pSkinnedMesh->CreateComObjects(GameEngine::get()->getDevice());


    skinnedMeshes.insert(std::make_pair(path, pSkinnedMesh));
    return result;
}

bool MeshLoaderManager::SetDefaultTransform(std::filesystem::path fileLocal, const VECTOR3& posOff, const VECTOR3& rotOff, const VECTOR3& scaOff)
{
    auto it = skinnedMeshes.find(fileLocal);
    if (it != skinnedMeshes.end())
    {
        DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(scaOff.x, scaOff.y, scaOff.z) };
        DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotOff.x), DirectX::XMConvertToRadians(rotOff.y), DirectX::XMConvertToRadians(rotOff.z)) };
        DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(posOff.x, posOff.y, posOff.z) };

        DirectX::XMMATRIX W = S * R * T;
        DirectX::XMFLOAT4X4 w;
        DirectX::XMStoreFloat4x4(&w, W);
        it->second->SetDefaultTransform(w);

        if (rebuildThread.get())
            rebuildThread->join();
        rebuildThread.reset();
        rebuildThread = std::make_unique<std::thread>(&SkinnedMesh::CreateNewCereal, it->second);
        return true;
    }
  
    return false;
}

bool MeshLoaderManager::RebuildMeshCereal(pSmartVoid& mesh)
{
    SkinnedMesh* pMesh = nullptr;
    for(auto& it : skinnedMeshes)
        if (it.second == mesh)
        {
            pMesh = GetFrom<SkinnedMesh>(mesh);
            if (rebuildThread.get())
                rebuildThread->join();
            rebuildThread.reset();
            rebuildThread = std::make_unique<std::thread>(&SkinnedMesh::CreateNewCereal, pMesh);
            //pMesh->CreateNewCereal(it.first.string().c_str());
            return true;
        }
    return false;
}

MeshLoaderManager::~MeshLoaderManager()
{
    if (rebuildThread.get())
        rebuildThread->join();
    rebuildThread.reset();
    DeleteAllMesh();
   
}

void MeshLoaderManager::DeleteAllMesh()
{
    skinnedMeshes.clear();
}

bool MeshLoaderManager::CheckHadMesh(pSmartVoid& mesh, std::filesystem::path pathh)
{
    bool result = false;
    std::shared_ptr<SkinnedMesh> pSkinnedMesh = GetFromPoint<SkinnedMesh>(mesh);
    auto it = skinnedMeshes.find(pathh);
    if (it != skinnedMeshes.end())
    {
        pSkinnedMesh = it->second;
        result = true;
    }
    mesh = SetToPoint<SkinnedMesh>(pSkinnedMesh);
    return result;
}

bool MeshLoaderManager::IsCanToLoadMesh(std::filesystem::path local, pSmartVoid& mesh)
{
    AnimatorManager* animatorManager = GetFrom<AnimatorManager>(GameEngine::get()->getAnimatorManager());
    MaterialManager* materialmanager = GetFrom<MaterialManager>(GameEngine::get()->getMaterialManager());
    std::unordered_map<int64_t, std::shared_ptr<Material>> materialMap;
    std::shared_ptr<SkinnedMesh> pSkinnedMesh = GetFromPoint<SkinnedMesh>(mesh);
    pSkinnedMesh.reset(new SkinnedMesh);
    if (pSkinnedMesh->IsCanLoadCereal(local.string()))
    {
        //animatorManager->CreateNewAnimatorFormModelFile(fbx_scene, sceneView, local, 0);

        pSkinnedMesh->CreateComObjects(GameEngine::get()->getDevice());
        mesh = SetToPoint<SkinnedMesh>(pSkinnedMesh);

        materialmanager->RegisterAllMaterialsFromMeshData(mesh, materialMap);
        return true;
    }

  
    FbxManager* fbx_manager{ FbxManager::Create() };
    FbxScene* fbx_scene{ FbxScene::Create(fbx_manager, "") };

    FbxImporter* fbx_importer{ FbxImporter::Create(fbx_manager, "") };
    bool import_status{ false };
    import_status = fbx_importer->Initialize(local.string().c_str());
#ifdef USE_IMGUI
    SUCCEEDEDRESULTTEXT(import_status, fbx_importer->GetStatus().GetErrorString());
#else
#endif // USE_IMGUI
    import_status = fbx_importer->Import(fbx_scene);
#ifdef USE_IMGUI
    SUCCEEDEDRESULTTEXT(import_status, fbx_importer->GetStatus().GetErrorString());
#endif // USE_IMGUI


    bool Ismesh = false;
    
    for (int i = 0; i < fbx_scene->GetNodeCount(); i++)
    {
        FbxNodeAttribute* nodeAtt = fbx_scene->GetNode(i)->GetNodeAttribute();
        if (nodeAtt && nodeAtt->GetAttributeType() == FbxNodeAttribute::EType::eMesh)
        {
            Ismesh = true;
        }
            
    }
    if (Ismesh)
    {
        sceneView = new SceneMesh;
        std::shared_ptr<SkinnedMesh> pSkinnedMesh = GetFromPoint<SkinnedMesh>(mesh);
       
        pSkinnedMesh.reset(new SkinnedMesh);

        if (!pSkinnedMesh->IsCanLoadCereal(local.string()))
        {
            FbxGeometryConverter fbx_converter(fbx_manager);
            fbx_converter.Triangulate(fbx_scene, true/*replace*/, false/*legacy*/);
            fbx_converter.RemoveBadPolygonsFromMeshes(fbx_scene);
    
            std::function<void(FbxNode*)> traverse
            { [&](FbxNode* fbx_node)
                {
                    SceneMesh::Node& node{ sceneView->nodes.emplace_back() };
                    node.attribute = fbx_node->GetNodeAttribute() ? fbx_node->GetNodeAttribute()->GetAttributeType() : FbxNodeAttribute::EType::eUnknown;
                    node.name = fbx_node->GetName();
                    node.unique_id = fbx_node->GetUniqueID();
                    node.parent_index = sceneView->indexof(fbx_node->GetParent() ? fbx_node->GetParent()->GetUniqueID() : 0);
    
                    for (int child_index = 0; child_index < fbx_node->GetChildCount(); ++child_index)
                    {
                        traverse(fbx_node->GetChild(child_index));
                    }
            }
            };
            traverse(fbx_scene->GetRootNode());
    
    
            FetchMeshes(fbx_scene, pSkinnedMesh->getMeshRawList());
    
            FetchMaterials(fbx_scene, local.string().c_str(), materialMap);
            
        }
   
        animatorManager->CreateNewAnimatorFormModelFile(fbx_scene, sceneView, local, 0);

        pSkinnedMesh->CreateComObjects(GameEngine::get()->getDevice());
        pSkinnedMesh->CreateNewCereal();
        mesh = SetToPoint<SkinnedMesh>(pSkinnedMesh);
    
        materialmanager->RegisterAllMaterialsFromMeshData(mesh, materialMap);
        delete sceneView;
       
    }

    fbx_manager->Destroy();
    return Ismesh;
}

void MeshLoaderManager::FetchMeshes(FbxScene* fbx_scene, std::vector<SkeletonMesh>& meshes)
{
    for (const SceneMesh::Node& node : sceneView->nodes)
    {
        if (node.attribute != FbxNodeAttribute::EType::eMesh)
        {
            continue;
        }

        FbxNode* fbx_node{ fbx_scene->FindNodeByName(node.name.c_str()) };
        FbxMesh* fbx_mesh{ fbx_node->GetMesh() };
        if (fbx_mesh->GetPolygonCount() == 0)
        {
            continue;
        }
        auto& mesh{ meshes.emplace_back() };
        mesh.uniqueId = fbx_mesh->GetNode()->GetUniqueID();
        mesh.name = fbx_mesh->GetNode()->GetName();
        mesh.nodeIndex = sceneView->indexof(mesh.uniqueId);
        mesh.defaultGlobalTransform = to_xmfloat4x4(fbx_mesh->GetNode()->EvaluateGlobalTransform());
        std::vector<std::vector<Skeleton::BoneInfluence>> bone_influences;
        FetchBoneInfluences(fbx_mesh, bone_influences);
        FetchSkeleton(fbx_mesh, mesh.bindPose);

        std::vector<Subset>& subsets{ mesh.subsets };
        const int material_count{ fbx_mesh->GetNode()->GetMaterialCount() };
        subsets.resize(material_count > 0 ? material_count : 1);


        for (int material_index = 0; material_index < material_count; ++material_index)
        {
            const FbxSurfaceMaterial* fbx_material{ fbx_mesh->GetNode()->GetMaterial(material_index) };
            subsets.at(material_index).materialUniqueId = fbx_material->GetUniqueID();
        }
        if (material_count > 0)
        {
            const int polygon_count{ fbx_mesh->GetPolygonCount() };
            for (int polygon_index = 0; polygon_index < polygon_count; ++polygon_index)
            {
                const int material_index{ fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(polygon_index) };
                subsets.at(material_index).indexCount += 3;
            }
            uint32_t offset{ 0 };
            for (Subset& subset : subsets)
            {
                subset.startIndexLocation = offset;
                offset += subset.indexCount;
                // This will be used as counter in the following procedures, reset to zero
                subset.indexCount = 0;
            }
        }

        const int polygon_count{ fbx_mesh->GetPolygonCount() };
        mesh.vertices.resize(polygon_count * 3LL);
        mesh.indices.resize(polygon_count * 3LL);
        if (mesh.vertices.size() == 0)
        {
            int a = 1;
            a = 2;
        }
        FbxStringList uv_names;
        fbx_mesh->GetUVSetNames(uv_names);

        const FbxVector4* control_points{ fbx_mesh->GetControlPoints() };
        for (int polygon_index = 0; polygon_index < polygon_count; ++polygon_index)
        {
            const int material_index{ material_count > 0 ? fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(polygon_index) : 0 };
            Subset& subset{ subsets.at(material_index) };
            const uint32_t offset{ subset.startIndexLocation + subset.indexCount };

            for (int position_in_polygon = 0; position_in_polygon < 3; ++position_in_polygon)
            {
                const int vertex_index{ polygon_index * 3 + position_in_polygon };

                BoneVertex vertex;
                const int polygon_vertex{ fbx_mesh->GetPolygonVertex(polygon_index, position_in_polygon) };
                vertex.position.x = static_cast<float>(control_points[polygon_vertex][0]);
                vertex.position.y = static_cast<float>(control_points[polygon_vertex][1]);
                vertex.position.z = static_cast<float>(control_points[polygon_vertex][2]);
                const std::vector<Skeleton::BoneInfluence>& influences_per_control_point
                { bone_influences.at(polygon_vertex) };

                float total_weight = 0;

                for (size_t influence_index = 0; influence_index < influences_per_control_point.size();
                    ++influence_index)
                {
                    if (influence_index < MAX_BONE_INFLUENCES)
                    {
                        total_weight += influences_per_control_point.at(influence_index).boneWeight;
                        vertex.boneWeights[influence_index] =
                            influences_per_control_point.at(influence_index).boneWeight;
                        vertex.boneIndices[influence_index] =
                            influences_per_control_point.at(influence_index).boneIndex;
                    }
                    else
                    {
                        total_weight += influences_per_control_point.at(influence_index).boneWeight;
                        int i = 0;
                        for (int j = 0; j < 4; j++)
                        {
                            if (vertex.boneWeights[i] < vertex.boneWeights[j])
                                i = j;
                        }
                        vertex.boneWeights[i] += influences_per_control_point.at(influence_index).boneWeight;
                        vertex.boneIndices[i] = influences_per_control_point.at(influence_index).boneIndex;

                       /* vertex.bone_weights[0] += influences_per_control_point.at(influence_index).bone_weight / 4;
                        vertex.bone_weights[1] += influences_per_control_point.at(influence_index).bone_weight / 4;
                        vertex.bone_weights[2] += influences_per_control_point.at(influence_index).bone_weight / 4;
                        vertex.bone_weights[3] += influences_per_control_point.at(influence_index).bone_weight / 4;
                        vertex.bone_indices[influence_index] = influences_per_control_point.at(influence_index).bone_index;*/
                    }
                }
                if(total_weight != 0)
                for (size_t i = 0; i < MAX_BONE_INFLUENCES; ++i)
                {
                    vertex.boneWeights[i] /= total_weight;
                }
                if (fbx_mesh->GetElementNormalCount() > 0)
                {
                    FbxVector4 normal;
                    fbx_mesh->GetPolygonVertexNormal(polygon_index, position_in_polygon, normal);
                    vertex.normal.x = static_cast<float>(normal[0]);
                    vertex.normal.y = static_cast<float>(normal[1]);
                    vertex.normal.z = static_cast<float>(normal[2]);
                }
                if (fbx_mesh->GetElementUVCount() > 0)
                {
                    FbxVector2 uv;
                    bool unmapped_uv;
                    fbx_mesh->GetPolygonVertexUV(polygon_index, position_in_polygon,
                        uv_names[0], uv, unmapped_uv);
                    vertex.texcoord.x = static_cast<float>(uv[0]);
                    vertex.texcoord.y = 1.0f - static_cast<float>(uv[1]);

                }
                if (fbx_mesh->GenerateTangentsData(0, false))
                {
                    const FbxGeometryElementTangent* tangent = fbx_mesh->GetElementTangent(0);
                    vertex.tangent.x = static_cast<float>(tangent->GetDirectArray().GetAt(vertex_index)[0]);
                    vertex.tangent.y = static_cast<float>(tangent->GetDirectArray().GetAt(vertex_index)[1]);
                    vertex.tangent.z = static_cast<float>(tangent->GetDirectArray().GetAt(vertex_index)[2]);
                    vertex.tangent.w = static_cast<float>(tangent->GetDirectArray().GetAt(vertex_index)[3]);
                }
                mesh.vertices.at(vertex_index) = std::move(vertex);
                mesh.indices.at(static_cast<size_t>(offset) + position_in_polygon) = vertex_index;


                subset.indexCount++;
            }
        }

        FetchBouding(fbx_mesh, mesh);
    }

}

void MeshLoaderManager::FetchSkeleton(FbxMesh* fbx_mesh, Skeleton& bind_pose)
{
    const int deformer_count = fbx_mesh->GetDeformerCount(FbxDeformer::eSkin);
    for (int deformer_index = 0; deformer_index < deformer_count; ++deformer_index)
    {
        FbxSkin* skin = static_cast<FbxSkin*>(fbx_mesh->GetDeformer(deformer_index, FbxDeformer::eSkin));
        const int cluster_count = skin->GetClusterCount();
        bind_pose.bones.resize(cluster_count);
        for (int cluster_index = 0; cluster_index < cluster_count; ++cluster_index)
        {
            FbxCluster* cluster = skin->GetCluster(cluster_index);

            Skeleton::Bone& bone{ bind_pose.bones.at(cluster_index) };
            bone.name = cluster->GetLink()->GetName();
            bone.uniqueId = cluster->GetLink()->GetUniqueID();
            bone.parentIndex = bind_pose.indexof(cluster->GetLink()->GetParent()->GetUniqueID());
            bone.nodeIndex = sceneView->indexof(bone.uniqueId);

            //'reference_global_init_position' is used to convert from local space of model(mesh) to
             // global space of scene.
            FbxAMatrix reference_global_init_position;
            cluster->GetTransformMatrix(reference_global_init_position);

            // 'cluster_global_init_position' is used to convert from local space of bone to
             // global space of scene.
            FbxAMatrix cluster_global_init_position;
            cluster->GetTransformLinkMatrix(cluster_global_init_position);

            // Matrices are defined using the Column Major scheme. When a FbxAMatrix represents a transformation
           // (translation, rotation and scale), the last row of the matrix represents the translation part of
           // the transformation.
           // Compose 'bone.offset_transform' matrix that trnasforms position from mesh space to bone space.
               // This matrix is called the offset matrix.
            bone.offsetTransform
                = to_xmfloat4x4(cluster_global_init_position.Inverse() * reference_global_init_position);

        }

    }
}

void MeshLoaderManager::FetchBoneInfluences(const FbxMesh* fbx_mesh, std::vector<std::vector<Skeleton::BoneInfluence>>& bone_influences)
{
    const int control_points_count{ fbx_mesh->GetControlPointsCount() };
    bone_influences.resize(control_points_count);

    const int skin_count{ fbx_mesh->GetDeformerCount(FbxDeformer::eSkin) };
    for (int skin_index = 0; skin_index < skin_count; ++skin_index)
    {
        const FbxSkin* fbx_skin
        { static_cast<FbxSkin*>(fbx_mesh->GetDeformer(skin_index, FbxDeformer::eSkin)) };

        const int cluster_count{ fbx_skin->GetClusterCount() };
        for (int cluster_index = 0; cluster_index < cluster_count; ++cluster_index)
        {
            const FbxCluster* fbx_cluster{ fbx_skin->GetCluster(cluster_index) };

            const int control_point_indices_count{ fbx_cluster->GetControlPointIndicesCount() };
            for (int control_point_indices_index = 0; control_point_indices_index < control_point_indices_count;
                ++control_point_indices_index)
            {
                int control_point_index{ fbx_cluster->GetControlPointIndices()[control_point_indices_index] };
                double control_point_weight
                { fbx_cluster->GetControlPointWeights()[control_point_indices_index] };
                Skeleton::BoneInfluence& bone_influence{ bone_influences.at(control_point_index).emplace_back() };
                bone_influence.boneIndex = static_cast<uint32_t>(cluster_index);
                bone_influence.boneWeight = static_cast<float>(control_point_weight);
            }
        }
    }
}

void MeshLoaderManager::FetchMaterials(FbxScene* fbx_scene, std::filesystem::path local, std::unordered_map<int64_t, std::shared_ptr<Material>>& listMaterial)
{
    const size_t node_count{ sceneView->nodes.size() };
    MaterialManager* materialmanager = GetFrom<MaterialManager>(GameEngine::get()->getMaterialManager());
    for (size_t node_index = 0; node_index < node_count; ++node_index)
    {
        const SceneMesh::Node& node{ sceneView->nodes.at(node_index) };
        const FbxNode* fbx_node{ fbx_scene->FindNodeByName(node.name.c_str()) };

        const int material_count{ fbx_node->GetMaterialCount() };
        for (int material_index = 0; material_index < material_count; ++material_index)
        {
            const FbxSurfaceMaterial* fbx_material{ fbx_node->GetMaterial(material_index) };

            std::string materialName = fbx_material->GetName();
            uint64_t materialId = fbx_material->GetUniqueID();
            std::vector<std::string> materialTextureFileName;
            materialTextureFileName.resize(TextureShaderResourceSize);
            VECTOR4 fKd;
            VECTOR4 fKs;
            VECTOR4 fKa;


            FbxProperty fbx_property;
            fbx_property = fbx_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
            if (fbx_property.IsValid())
            {
                const FbxDouble4 color{ fbx_property.Get<FbxDouble4>() };

                fKd.x = static_cast<float>(color[0]);
                fKd.y = static_cast<float>(color[1]);
                fKd.z = static_cast<float>(color[2]);
                fKd.w = 1.0f;


                const FbxDouble4 color2{ fbx_property.Get<FbxDouble4>() };
                fKs.x = static_cast<float>(color2[0]);
                fKs.y = static_cast<float>(color2[1]);
                fKs.z = static_cast<float>(color2[2]);
                fKs.w = 1.0f;


                const FbxDouble4 color3{ fbx_property.Get<FbxDouble4>() };
                fKa.x = static_cast<float>(color3[0]);
                fKa.y = static_cast<float>(color3[1]);
                fKa.z = static_cast<float>(color3[2]);
                fKa.w = 1.0f;

                const FbxFileTexture* fbx_texture{ fbx_property.GetSrcObject<FbxFileTexture>() };
                materialTextureFileName[0] =
                    fbx_texture ? fbx_texture->GetRelativeFileName() : "";

            }
            fbx_property = fbx_material->FindProperty(FbxSurfaceMaterial::sNormalMap);

            if (fbx_property.IsValid())
            {

                const FbxFileTexture* file_texture{ fbx_property.GetSrcObject<FbxFileTexture>() };
                materialTextureFileName[1] = file_texture ? file_texture->GetRelativeFileName() : "";
            }

            std::string name = {};
            for (size_t fileNameNum = 0; fileNameNum < 2; fileNameNum++)
            {
                if (materialTextureFileName[fileNameNum].size() == 0) continue;
                for (size_t i = materialTextureFileName[fileNameNum].size(); i--;)
                {
                    if (materialTextureFileName[fileNameNum].at(i) == '\\' || materialTextureFileName[fileNameNum].at(i) == '/')
                        break;

                    name += materialTextureFileName[fileNameNum].at(i);
                    if (i == 0)
                        break;
                }
                materialTextureFileName[fileNameNum] = "Texture/";
                std::filesystem::path path(local);
                materialTextureFileName[fileNameNum] = path.replace_filename(materialTextureFileName[fileNameNum]).string();
                if (!std::filesystem::exists(materialTextureFileName[fileNameNum]))
                {
                    std::filesystem::create_directory(materialTextureFileName[fileNameNum]);
                }
                for (size_t i = name.size() - 1; ; i--)
                {
                    materialTextureFileName[fileNameNum] += name[i];
                    if (i == 0)
                        break;
                }
            }
           
            std::shared_ptr<Material> material = std::make_shared<Material>();
            material->setFileLocal(local.string());
            material->setUniqueId(materialId);
            material->setName(materialName);
            material->setKd(fKd);
            material->setKs(fKs);
            material->setKa(fKa);
            material->getVectorTextureFileNames() = materialTextureFileName;


            listMaterial.insert(std::make_pair(material->getUniqueId(), material));


            //for (auto& mesh : meshes)
            //    for (auto& subset : mesh.subsets)
            //    {
            //        if (subset.material_unique_id == materialId)
            //        {
            //            subset.material = material;
            //        }
            //    }

        }
    }



}

void MeshLoaderManager::FetchBouding(FbxMesh* fbx_mesh, BaseMesh& mesh)
{
    fbx_mesh->ComputeBBox();
    const FbxDouble3 bbMax = fbx_mesh->BBoxMax;
    const FbxDouble3 bbMin = fbx_mesh->BBoxMin;
    mesh.boundingBox[0].x = std::min<float>(mesh.boundingBox[0].x, static_cast<float>(bbMin.mData[0]));
    mesh.boundingBox[0].y = std::min<float>(mesh.boundingBox[0].y, static_cast<float>(bbMin.mData[1]));
    mesh.boundingBox[0].z = std::min<float>(mesh.boundingBox[0].z, static_cast<float>(bbMin.mData[2]));
    mesh.boundingBox[1].x = std::max<float>(mesh.boundingBox[1].x, static_cast<float>(bbMax.mData[0]));
    mesh.boundingBox[1].y = std::max<float>(mesh.boundingBox[1].y, static_cast<float>(bbMax.mData[1]));
    mesh.boundingBox[1].z = std::max<float>(mesh.boundingBox[1].z, static_cast<float>(bbMax.mData[2]));
    DirectX::XMVECTOR V = DirectX::XMLoadFloat3(&mesh.boundingBox[0]);
    DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&mesh.defaultGlobalTransform);
    DirectX::XMStoreFloat3(&mesh.boundingBox[0], DirectX::XMVector3TransformCoord(V, M));
    V = DirectX::XMLoadFloat3(&mesh.boundingBox[1]);
    DirectX::XMStoreFloat3(&mesh.boundingBox[1], DirectX::XMVector3TransformCoord(V, M));
}
