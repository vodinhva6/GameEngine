#include "GameEngine.h"
#include "ContentBrowser.h"
#include "AnimatorManager.h"
#include <future>
#include "MousePoint.h"
void ContentBrowser::Init(std::string assetDir)
{
    OriginalDirectory = std::filesystem::path(assetDir);
    contentDirectory = OriginalDirectory;
    ButtonSize = 128;
    TableSize = 0;
    ColumnSize = 0;
    ButtonsDis = 32;
    loadingMesh = false;
    loadingTexture = false;
    loadingMaterial = false;
    dirChanged = false;
    selectTexture = "";
    selectMesh = "";
    selectMaterial = "";

    renameEdit = "";
    GameEngine::get()->LoadTextureFromFile("./EngineObject/Images/folder.png",folder);
    GameEngine::get()->LoadTextureFromFile("./EngineObject/Images/unknownFile.png", unknownFile);
    GameEngine::get()->LoadTextureFromFile("./EngineObject/Images/fbxAnim.png", animationFile);
    GameEngine::get()->LoadTextureFromFile("./EngineObject/Images/loading.png", loadingFile);
    GameEngine::get()->LoadTextureFromFile("./EngineObject/Images/animator.png", animatorFile);
    GameEngine::get()->LoadTextureFromFile("./EngineObject/Images/trash.png", trashFile);
    GameEngine::get()->LoadTextureFromFile("./EngineObject/Images/play.png", play);
    
    //GameEngine::get()->LoadSkinnedMesh("EngineObject/Material/materialPreviewNON.fbx", materialNONMesh);



    D3D11_VIEWPORT viewPort;
    viewPort.Height = ButtonSize*10;
    viewPort.Width = ButtonSize*10;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;
    thumbnailFramebuffer = std::make_unique<FrameThumbnail>(GameEngine::get()->getSwapChain(), GameEngine::get()->getDevice(), viewPort, false, FrameBufferName::FRAMETHUMBNAIL);
    
    FormatTextureLoadList =
    {
        ".png", ".PNG", ".jpg" , ".JPG", ".tga", ".TGA" , ".tif", ".TIF", ".hdr", ".HDR"
    };
    FormatMeshLoadList =
    {
        ".fbx", ".FBX"
    };

    FormatMaterialLoadList =
    {
        ".material", ".MATERIAL"
    };
    UnreadFormatFile =
    {
        ".DDS" , ".mes" 
    };


}

void ContentBrowser::Update()
{
#ifdef USE_IMGUI
    ImGui::Begin("Play");
    EngineRunMode mode = GameEngine::get()->getGameMode();
    if (ImGui::ImageButton(play.Get(), { 50,50 }))
        switch (mode)
        {
        case EngineRunMode::EngineDebugMode:
            GameEngine::get()->setGameRunningMode(EngineRunMode::EngineGameMode);
            break;
        case EngineRunMode::EngineGameMode:
            GameEngine::get()->setGameRunningMode(EngineRunMode::EngineDebugMode);
            break;
        default:
            break;
        }
        
    ImGui::End();

    ImGui::Begin("Asset Tree");
    ImGui::SetWindowFontScale(1.5f);
    listNew.push_back(OriginalDirectory);
    if (ImGui::TreeNode("Asset"))
    {
        Query(OriginalDirectory);
        ImGui::TreePop();
    }
    std::filesystem::path path = CheckUpdate();
    if (!path.empty())
    {
        SetContentDirectory(path);
    }
       
    listNew.clear();
    ImGui::End();

    
#endif // USE_IMGUI

    

}

void ContentBrowser::DrawOn()
{
#ifdef USE_IMGUI
    ImGui::Begin("Content");
    TableSize = ImGui::GetContentRegionAvail().x;
    ColumnSize = (int)(TableSize / (ButtonSize + ButtonsDis));
    ImGui::Columns((ColumnSize > 0)? ColumnSize : 2, nullptr, false);
   
    ImGui::Begin("Mesh Debug");
    ImGui::End();
    ImGui::Begin("Debug Material");
    ImGui::End();
    ImGui::Begin("Debug Animator");
    ImGui::End();
    ImGui::Begin("Debug Animation");
    ImGui::End();

    if (dirChanged)
    {
        DeleteThumbnails();
        dirChanged = false;
    }
    else
    {
        CheckRemoveFromList();
        SearchDirectory();
        DrawThumbnails();
        DebugSelectItem();
        MenuBar();
        LoadThumbnails();
        
    }
   
   
   
    ImGui::End();
    
#endif // USE_IMGUI
}

void ContentBrowser::SetContentDirectory(std::filesystem::path directory)
{
    if (directory != "" && contentDirectory != directory)
    {
        if (directory.has_extension())
            directory = directory.parent_path();
        contentDirectory = directory;
        dirChanged = true;

    }
        
}
ContentBrowser::~ContentBrowser()
{
    DeleteThumbnails();
}

void ContentBrowser::Query(std::filesystem::path dir)
{
#ifdef USE_IMGUI
    for (auto& dirr : std::filesystem::directory_iterator(dir))
    {
        std::string path = dirr.path().string();
        std::string fileName = dirr.path().filename().string();
        if (dirr.is_directory())
            if (ImGui::TreeNode(fileName.c_str()))
            {
                listNew.push_back(dirr.path());
                Query(dirr);
                ImGui::TreePop();
            }
    }
#endif // USE_IMGUI
}

void ContentBrowser::DeleteThumbnails()
{
    DeleteTexturesLoaded();
    DeleteMeshsLoaded();
    DeleteMaterialsLoaded();
    DeleteAnimatorsLoaded();
    DeleteAnimationsLoaded();
    listDirectory.clear();
}

void ContentBrowser::DeleteTexturesLoaded()
{
    if (LoadTextureThread.get())
        LoadTextureThread.get()->join();
    for (auto& it : textureFiles)
    {
        it.second.ReleaseAndGetAddressOf();
        GameEngine::get()->ReleaseTexture(it.first);
    }
    textureFiles.clear();
    textureLoadQueue.clear();
    LoadTextureThread.release();
    loadingTexture = false;
    LoadTextureThread.reset();
}

void ContentBrowser::DeleteMeshsLoaded()
{
    if (LoadMeshThread.get())
        LoadMeshThread.get()->join();
    for (auto& it : meshFiles)
    {
        GameEngine::get()->ReleaseMesh(it.first);
    }
        
    meshFiles.clear();
    meshLoadQueue.clear();
    LoadMeshThread.release();
    loadingMesh = false;
}

void ContentBrowser::DeleteMaterialsLoaded()
{
    if (LoadMaterialThread.get())
        LoadMaterialThread.get()->join();
    for (auto& it : materialFiles)
        GameEngine::get()->ReleaseMaterial(it.first);
    materialFiles.clear();
    materialLoadQueue.clear();
    LoadMaterialThread.reset();
    loadingMaterial = false;
}

void ContentBrowser::DeleteAnimatorsLoaded()
{
    if (LoadAnimatorThread.get())
        LoadAnimatorThread.get()->join();
    for (auto& it : animatorFiles)
        GameEngine::get()->ReleaseAnimator(it.first);
    animatorFiles.clear();
    animatorLoadQueue.clear();
    LoadAnimatorThread.reset();
    loadingAnimator = false;
}

void ContentBrowser::DeleteAnimationsLoaded()
{
    if (LoadAnimationThread.get())
        LoadAnimationThread.get()->join();
    for (auto& it : animationFiles)
        GameEngine::get()->ReleaseAnimation(it.first);
    animationFiles.clear();
    animationLoadQueue.clear();
    LoadAnimationThread.reset();
    loadingAnimation = false;
}

void ContentBrowser::LoadTextureToThumbnails()
{
    for (auto& it : textureLoadQueue)
    {
        static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderRessource;
        if (!loadingTexture && !it.second)
        {
            loadingTexture = true;
            LoadTextureThread = std::make_unique<std::thread>(&GameEngine::LoadTextureFromFileMultiThread, GameEngine::get(),
                it.first, &shaderRessource, &it.second);

        }
        if (it.second)
        {
            auto jt = textureFiles.find(it.first);
            if (jt->second.Get() != loadingFile.Get() || jt->second.Get() == unknownFile.Get()) continue;
            jt = textureFiles.erase(jt);
            std::pair<std::filesystem::path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> pairr;
            pairr.first = it.first;
            if (shaderRessource.Get())
                pairr.second = shaderRessource;
            else pairr.second = unknownFile;
            textureFiles.insert(pairr);
            shaderRessource.ReleaseAndGetAddressOf();
            loadingTexture = false;
            LoadTextureThread->join();
            LoadTextureThread.reset();
               
        }
        else
        {
            if (textureFiles.find(it.first) == textureFiles.end())
            {
                textureFiles.insert(std::make_pair(it.first, loadingFile.Get()));
            }
        }
    }
}

void ContentBrowser::LoadMeshToThumbnails()
{
    for (auto& it : meshLoadQueue)
    {
        static pSmartVoid mesh = nullptr;
        if (!loadingMesh && !it.second)
        {
            loadingMesh = true;
            LoadMeshThread = std::make_unique<std::thread>(&GameEngine::CreateSkinnedMeshThumbnail, GameEngine::get(), it.first, &mesh, &it.second);
        }
        if (it.second)
        {
            auto jt = meshFiles.find(it.first);
            if (jt->second.Get() != loadingFile.Get() || jt->second.Get() == unknownFile.Get() || jt->second.Get() == animationFile.Get()) continue;
            jt = meshFiles.erase(jt);
            if (mesh)
            {
                Microsoft::WRL::ComPtr < ID3D11ShaderResourceView> copyShaderResorce;
                std::unordered_map<int64_t, std::shared_ptr<Material>> dummyMaterial;
                DrawMeshToFramebufferThumbnail(mesh, copyShaderResorce, dummyMaterial);
                meshFiles.insert(std::make_pair(it.first, copyShaderResorce));
            }
            else
            {
                meshFiles.insert(std::make_pair(it.first, animationFile.Get()));
            }
            LoadMeshThread.release();
            mesh = nullptr;
            loadingMesh = false;
            
        }
        else
        {
            if (meshFiles.find(it.first) == meshFiles.end())
            {
                meshFiles.insert(std::make_pair(it.first, loadingFile.Get()));
            }
        }
    }
}

void ContentBrowser::LoadMaterialToThumbnails()
{
    for (auto& it : materialLoadQueue)
    {
        static pSmartVoid material = nullptr;
        if (!loadingMaterial && !it.second)
        {
            loadingMaterial = true;
            LoadMaterialThread = std::make_unique<std::thread>(&GameEngine::LoadMaterialThumbnail, GameEngine::get(), 
                it.first, &material, &it.second);
        }
        if (it.second)
        {
            auto jt = materialFiles.find(it.first);
            if (jt->second.Get() != loadingFile.Get() || jt->second.Get() == unknownFile.Get() ) continue;
            jt = materialFiles.erase(jt);
            if (material)
            {
                Microsoft::WRL::ComPtr < ID3D11ShaderResourceView> copyShaderResorce;
                DrawMaterialToFramebufferThumbnail(material, copyShaderResorce);
                materialFiles.insert(std::make_pair(it.first, copyShaderResorce));

            }
            else
            {
                materialFiles.insert(std::make_pair(it.first, animationFile.Get()));
            }
            LoadMaterialThread.release();
            material = nullptr;
            loadingMaterial = false;

        }
        else
        {
            if (materialFiles.find(it.first) == materialFiles.end())
            {
                materialFiles.insert(std::make_pair(it.first, loadingFile.Get()));
            }
        }
    }
}

void ContentBrowser::LoadAnimatorToThumbnails()
{
    for (auto& it : animatorLoadQueue)
    {
        static pSmartVoid animator = nullptr;
        if (!loadingAnimator && !it.second)
        {
            loadingAnimator = true;
            LoadAnimatorThread = std::make_unique<std::thread>(&GameEngine::LoadAnimatorThumbnail, GameEngine::get(),
               it.first, &animator, &it.second);
        }
        if (it.second)
        {
            auto jt = animatorFiles.find(it.first);
            if (jt->second.Get() != loadingFile.Get() || jt->second.Get() == unknownFile.Get()) continue;
            jt = animatorFiles.erase(jt);
            std::pair<std::filesystem::path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> pairr;
            pairr.first = it.first;
            pairr.second = animatorFile;

            animatorFiles.insert(pairr);

            loadingAnimator = false;
            LoadAnimatorThread->join();
            LoadAnimatorThread.reset();

        }
        else
        {
            if (animatorFiles.find(it.first) == animatorFiles.end())
            {
                animatorFiles.insert(std::make_pair(it.first, loadingFile.Get()));
            }
        }
    }
}

void ContentBrowser::LoadAnimationToThumbnails()
{
    for (auto& it : animationLoadQueue)
    {
        static pSmartVoid animation = nullptr;
        if (!loadingAnimation && !it.second)
        {
            loadingAnimation = true;
            LoadAnimationThread = std::make_unique<std::thread>(&GameEngine::LoadAnimationThumbnail, GameEngine::get(),
                it.first, &animation, &it.second);
        }
        if (it.second)
        {
            auto jt = animationFiles.find(it.first);
            if (jt->second.Get() != loadingFile.Get() || jt->second.Get() == unknownFile.Get()) continue;
            jt = animationFiles.erase(jt);
            std::pair<std::filesystem::path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> pairr;
            pairr.first = it.first;
            pairr.second = animationFile;

            animationFiles.insert(pairr);

            loadingAnimation = false;
            LoadAnimationThread->join();
            LoadAnimationThread.reset();

        }
        else
        {
            if (animationFiles.find(it.first) == animationFiles.end())
            {
                animationFiles.insert(std::make_pair(it.first, loadingFile.Get()));
            }
        }
    }
}

void ContentBrowser::SearchDirectory()
{
    for (auto& dirr : std::filesystem::directory_iterator(contentDirectory))
    {
        AddToQueue(dirr);
    }
    
}

void ContentBrowser::CheckRemoveFromList()
{
    for (std::map<std::filesystem::path, bool>::iterator it = materialLoadQueue.begin(); it != materialLoadQueue.end();)
    {
        if (!std::filesystem::exists(it->first))
        {
            auto jt = materialFiles.find(it->first);
            materialFiles.erase(jt);
            it = materialLoadQueue.erase(it);

        }
        else it++;
    }
    for (std::map<std::filesystem::path, bool>::iterator it = animatorLoadQueue.begin(); it != animatorLoadQueue.end();)
    {
        if (!std::filesystem::exists(it->first))
        {
            auto jt = animatorFiles.find(it->first);
            animatorFiles.erase(jt);
            it = animatorLoadQueue.erase(it);

        }
        else it++;
    }
}

void ContentBrowser::DrawThumbnails()
{
#ifdef USE_IMGUI
    MousePoint* mouseP = GetFrom<MousePoint>(GameEngine::get()->getMousePoint());
    bool ClickDouble = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
    bool LeftClick = false;
    if (!ClickDouble) LeftClick = ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right);
    std::filesystem::path path;

    if (contentDirectory != OriginalDirectory)
    {
        CheckBack(ClickDouble);
    }

    for (auto& it : listDirectory)
    {
        ButtonDetail(it, LeftClick, ClickDouble);
    }

    for (auto& it : meshFiles)
    {
        ButtonDetail(it, LeftClick);
    }

    for (auto& it : materialFiles)
    {
        ButtonDetail(it, LeftClick);
    }

    for (auto& it : textureFiles)
    {
        ButtonDetail(it, LeftClick);
    }

    for (auto& it : animatorFiles)
    {
        ButtonDetail(it, LeftClick);
    }

    for (auto& it : animationFiles)
    {
        ButtonDetail(it, LeftClick);
    }
    

#endif // USE_IMGUI
}

void ContentBrowser::LoadThumbnails()
{
    LoadTextureToThumbnails();
    LoadMeshToThumbnails();
    LoadMaterialToThumbnails();
    LoadAnimatorToThumbnails();
    LoadAnimationToThumbnails();
}

void ContentBrowser::DebugSelectItem()
{
    DebugMaterial();
    DebugMesh();
    DebugAnimator();
    DebugAnimation();
}

void ContentBrowser::ButtonDetail(std::pair<std::filesystem::path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> pair
    ,bool click, bool doubleClick)
{
#ifdef USE_IMGUI
    if (selectItem == pair.first)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.4f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.3f, 1.0f));
    }
    bool t = false;
    ImGui::ImageButton(pair.second.Get(), { ButtonSize,ButtonSize });
    if (ImGui::IsItemHovered())
    {
        if (doubleClick)
        {
            contentDirectory = pair.first;
            dirChanged = true;
        }
        else
            if (click)
                t = true;
    }

    if (selectItem == pair.first)
        ImGui::PopStyleColor(3);
    if (t)
        DistinctSelectItem(pair.first);
        
    if (selectItem.has_extension())
    {
        ImGui::PushID(selectItem.string().c_str());
        DragData(selectItem.string().c_str());
        ImGui::PopID();
    }
    if(renameEdit == pair.first)
    RenameEdit();
    else
    ImGui::TextWrapped(pair.first.filename().string().c_str());
    ImGui::NextColumn();
#endif // USE_IMGUI
}

template <typename T>
std::string ToStringWithPrecision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return std::move(out).str();
}

std::string getTextMaterialDebug(std::shared_ptr<Material> pMaterial)
{
    return "Name: " + pMaterial->getMaterialName() +
        "\nUnique Id: " + std::to_string(pMaterial->getUniqueId()) +
        "\nKa(rgba): " + ToStringWithPrecision(pMaterial->getKa().x,3) + "," +
        ToStringWithPrecision(pMaterial->getKa().y,3) + "," + ToStringWithPrecision(pMaterial->getKa().z,3) + "," +
        ToStringWithPrecision(pMaterial->getKa().w,3);
}

void ContentBrowser::DebugMaterial()
{
#ifdef USE_IMGUI
    
    static std::filesystem::path FilePath;
    static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> copyShaderResource;
    static pSmartVoid editingMaterial;
    bool changeShaderResource = false;
    if (FilePath != selectMaterial)
    {
        changeShaderResource = true;
        FilePath = selectMaterial;
        copyShaderResource.ReleaseAndGetAddressOf();
        if (!GameEngine::get()->LoadMaterial(FilePath, editingMaterial))
            editingMaterial.reset();
        ImGui::SetWindowFocus();
    }
    if (editingMaterial.get())
    {
        std::shared_ptr<Material> pMaterial = GetFromPoint<Material>(editingMaterial);
    
    
        ImGui::Begin("Debug Material");
        if (!copyShaderResource.Get() || changeShaderResource)
            DrawMaterialToFramebufferThumbnail(editingMaterial, copyShaderResource);
    
        if (ImGui::ImageButton(copyShaderResource.Get(), { 80,80 }))
        {
            contentDirectory = FilePath.parent_path();
            DistinctSelectItem(FilePath);
            dirChanged = true;
        }
    
        ImGui::SameLine();
        std::string text = getTextMaterialDebug(pMaterial);
        ImGui::Text(text.c_str());


        if (ImGui::CollapsingHeader("Texture File"))
            for (int i = 0; i < TextureShaderResourceSize; i++)
                if (EditTexturesMaterial((TextureSlotName)i, pMaterial.get()))
                {
                    GameEngine::get()->RebuildMaterial(SetToPoint(pMaterial));
                    copyShaderResource.Reset();
                    RemoveMaterialLocalFileFromQueue(FilePath);
                }
        if (ImGui::CollapsingHeader("Material UniqueId"))
        {
            static char newId[8];
            ImGui::SetNextItemWidth(60);
            if (ImGui::InputText(" ID", newId, 8, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                pMaterial->setUniqueId(std::stoull(std::string(newId)));
                GameEngine::get()->RebuildMaterial(SetToPoint(pMaterial));
                memset(newId, 0, sizeof(newId));
            }
        }

        if (ImGui::CollapsingHeader("Material Color"))
        {
            if (ImGui::ColorEdit4("Ka: ", &(pMaterial->getKa().x)))
                GameEngine::get()->RebuildMaterial(SetToPoint(pMaterial));
            if (ImGui::ColorEdit4("Ks: ", &(pMaterial->getKs().x)))
                GameEngine::get()->RebuildMaterial(SetToPoint(pMaterial));
            if (ImGui::ColorEdit4("Kd: ", &(pMaterial->getKd().x)))
                GameEngine::get()->RebuildMaterial(SetToPoint(pMaterial));
        }
    
        if (ImGui::CollapsingHeader("Environment Value"))
        {
            float t = pMaterial->getSmoothness();
            if (ImGui::DragFloat("Smoothness value", &t, 0.001f, 0, 1))
            {
                pMaterial->setSmoothness(t);
                GameEngine::get()->RebuildMaterial(SetToPoint(pMaterial));
            }
            t = pMaterial->getOcclusionStrength();
            if (ImGui::DragFloat("Occlusion value", &t, 0.001f, 0, 1))
            {
                pMaterial->setOcclusionStrength(t);
                GameEngine::get()->RebuildMaterial(SetToPoint(pMaterial));
            }
            t = pMaterial->getMetallicfactor();
            if (ImGui::DragFloat("Metallic value", &t, 0.001f, 0, 1))
            {
                pMaterial->setMetallicfactor(t);
                GameEngine::get()->RebuildMaterial(SetToPoint(pMaterial));
            }
        }
        ImGui::End();
    }
 
#endif // USE_IMGUI

}

void ContentBrowser::DebugMesh()
{
#ifdef USE_IMGUI
   
    static std::filesystem::path FilePath;
    static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> copyShaderResource;
    static pSmartVoid edittingMesh;
    bool changeShaderResource = false;
    if (FilePath != selectMesh)
    {
        changeShaderResource = true;
        FilePath = selectMesh;
        copyShaderResource.ReleaseAndGetAddressOf();
        if (!GameEngine::get()->LoadSkinnedMesh(FilePath, edittingMesh))
            edittingMesh.reset();
        ImGui::SetWindowFocus();
    }
    ImGui::Begin("Mesh Debug");
    if (edittingMesh.get())
    {
      
        std::shared_ptr<SkinnedMesh> pMesh = GetFromPoint<SkinnedMesh>(edittingMesh);
    
    
        if (!copyShaderResource.Get() || changeShaderResource)
        {
            std::unordered_map<int64_t, std::shared_ptr<Material>> dummyMaterial;
            DrawMeshToFramebufferThumbnail(edittingMesh, copyShaderResource, dummyMaterial);
        }
        
        ImGui::Indent(130.0f);
        if (ImGui::ImageButton(copyShaderResource.Get(), { 80,80 }))
        {
            SetContentDirectory(FilePath.parent_path());
            DistinctSelectItem(FilePath);
            
        }
        ImGui::Indent(-130.0f);

        bool setting = false;
        DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&pMesh->getDefaultTransform());
        VECTOR3 rotOffset, vertexOffset, scaleOffset;
        MyMath::get()->MatrixDecompose(Transform, vertexOffset, rotOffset, scaleOffset);
        setting = ImGui::InputFloat3("Vertex Offset", &vertexOffset.x,"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
        setting |= ImGui::InputFloat3("Size Offset", &scaleOffset.x, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
        setting |= ImGui::InputFloat3("Rotation Offset", &rotOffset.x, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
        

        if (setting)
        {
            GameEngine::get()->SetMeshDefaultTransform(selectMesh, vertexOffset, rotOffset, scaleOffset);

        }

    }
    ImGui::End();
#endif // USE_IMGUI

}

void ContentBrowser::DebugAnimator()
{
#ifdef USE_IMGUI
    if (selectAnimator == "") return;
   
    ImGui::Begin("Debug Animator");
    static pSmartVoid animator;
    bool rebuild = false;
    GameEngine::get()->LoadAnimator(selectAnimator, animator);
    std::shared_ptr<Animator> pAnimator = GetFromPoint<Animator>(animator);
    if (pAnimator.get())
    {
        if (ImGui::CollapsingHeader("Animation Clip"))
        {
            if (ImGui::Button(pAnimator->name.c_str()))
                SetItemSelect(pAnimator->localfile);
            ImGui::Indent();
            int i = 0;
            std::string deleteClip = "";
            for (auto& clip : pAnimator->animationClips)
            {
                auto name = clip.second->GetName();
                ImGui::Text(std::to_string(i).c_str());
                ImGui::SameLine();
                if (ImGui::Button(name.c_str()))
                    SetItemSelect(clip.second->local);
                ImGui::SameLine();
                ImGui::PushID(i);
                if (ImGui::ImageButton(trashFile.Get(), { 20.0f,20.0f }))
                {
                    rebuild = true;
                    deleteClip = name;
                    ImGui::PopID();
                    break;
                }
                i++;
                ImGui::PopID();
            }

            if (deleteClip != "")
            {
                auto it = pAnimator->animationClips.find(deleteClip);
                pAnimator->animationClips.erase(it);
            }
             

            ImGui::Button("Add+");
            ContentBrowser* contentBrowser = GetFrom<ContentBrowser>(GameEngine::get()->getContentBrowser());
            std::string animationlocal = contentBrowser->DropData("Animation");
            if (animationlocal != "")
            {
                rebuild = true;
                AnimatorManager* animatorManager = GetFrom<AnimatorManager>(GameEngine::get()->getAnimatorManager());
                std::shared_ptr<Animation> animation = std::make_shared<Animation>();
                animatorManager->LoadAnimation(animationlocal, animation);
                pAnimator->animationClips.insert(std::make_pair(animation->GetName(), animation));
            }
            ImGui::Unindent();
        }

        if (ImGui::CollapsingHeader("Animation Map"))
        {

            ImGui::Indent();
            std::unordered_map<std::string, std::unordered_map<std::string, std::vector<int>>>& animationMap = pAnimator->animationMap;
            std::vector<std::string> listAnimationName;
            for (auto& name : animationMap)
            {
                listAnimationName.push_back(name.first);
            }

           
            static size_t itemCurrent = 0;
            if (!listAnimationName.empty())
            {
                const char** comboItems = new const char* [listAnimationName.size()];
                for (size_t i = 0; i < listAnimationName.size(); ++i) {
                    comboItems[i] = listAnimationName[i].c_str();
                }
                ImGui::Combo("Clip", (int*)&itemCurrent, comboItems, (int)listAnimationName.size());
            }
              
            if (ImGui::Button("+Add"))
            {
                std::vector<int> temp = { 0,0 };
                std::unordered_map<std::string, std::vector<int>> tempMap;
                tempMap.insert(std::make_pair("NONSub",temp));
                animationMap.insert(std::make_pair(std::to_string(animationMap.size() + 1) + " :NONClip", tempMap));
                itemCurrent = animationMap.size()-1;
                rebuild = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete"))
            {
                auto it = animationMap.find(listAnimationName[itemCurrent]);
                animationMap.erase(it);
                rebuild = true;
                listAnimationName.erase(listAnimationName.begin() + itemCurrent);
                itemCurrent = std::clamp(itemCurrent - 1, (size_t)(0), listAnimationName.size());
            }

            char editname[256] = "";
            if (!listAnimationName.empty())
                if(itemCurrent < listAnimationName.size())
                    if (ImGui::InputTextEx("Rename Clip", listAnimationName[itemCurrent].c_str(), editname, sizeof(editname), { 150,20 }, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        auto key = listAnimationName[itemCurrent];
                        auto it = animationMap.find(key);
                        animationMap.insert(std::make_pair(editname, it->second));
                        animationMap.erase(it);
                        listAnimationName[itemCurrent] = editname;
                        rebuild = true;
                    }
            ImGui::Separator();
            ImGui::Separator();
            std::vector<std::string> listSubName;
            if (!listAnimationName.empty())
                if (itemCurrent < listAnimationName.size())
            {
                auto& clip = animationMap.find(listAnimationName[itemCurrent])->second;
                for (auto& name : clip)
                {
                    listSubName.push_back(name.first);
                }
                static size_t itemSubCurrent = 0;
                if (!listSubName.empty())
                {
                    const char** comboItems = new const char* [listSubName.size()];
                    for (size_t i = 0; i < listSubName.size(); ++i) {
                        comboItems[i] = listSubName[i].c_str();
                    }
                    ImGui::Combo("Sub", (int*)&itemSubCurrent, comboItems, (int)listSubName.size());
                }


                if (ImGui::Button("+Add Sub"))
                {
                    std::vector<int> temp = { 0,0 };
                    clip.insert(std::make_pair(std::to_string(clip.size() + 1) + " :NONSub", temp));
                    itemSubCurrent = clip.size() - 1;
                    rebuild = true;
                }
                ImGui::SameLine();
                if (ImGui::Button("Delete Sub"))
                {
                    auto it = clip.find(listSubName[itemSubCurrent]);
                    clip.erase(it);
                    rebuild = true;
                    listSubName.erase(listSubName.begin() + itemSubCurrent);
                    itemSubCurrent = std::clamp(itemSubCurrent - 1,(size_t)(0), listSubName.size());
                  
                }
                char editnameSub[256] = "";
                if (!listSubName.empty())
                    if (itemSubCurrent < listSubName.size())
                    {
                        if (ImGui::InputTextEx("Rename Sub", listSubName[itemSubCurrent].c_str(), editnameSub, sizeof(editnameSub), { 150,20 }, ImGuiInputTextFlags_EnterReturnsTrue))
                        {
                            auto key = listSubName[itemSubCurrent];
                            auto it = clip.find(key);
                            clip.insert(std::make_pair(editnameSub, it->second));
                            clip.erase(it);
                            listSubName[itemSubCurrent] = editnameSub;
                            rebuild = true;
                        }


                        char beginTime[256] = "";
                        auto it = clip.find(listSubName[itemSubCurrent]);
                        if (ImGui::InputTextEx("Begin", std::to_string(it->second.at((size_t)(Animator::AnimationPlayArea::BEGIN))).c_str(), beginTime, sizeof(beginTime), {150,20}, ImGuiInputTextFlags_EnterReturnsTrue))
                        {
                            it->second.at((size_t)(Animator::AnimationPlayArea::BEGIN)) = std::stoi(beginTime);
                            rebuild = true;
                        }
                        char endTime[256] = "";
                        if (ImGui::InputTextEx("End", std::to_string(it->second.at((size_t)(Animator::AnimationPlayArea::END))).c_str(), endTime, sizeof(endTime), { 150,20 }, ImGuiInputTextFlags_EnterReturnsTrue))
                        {
                            it->second.at((size_t)(Animator::AnimationPlayArea::END)) = std::stoi(endTime);
                            rebuild = true;
                        }
                    }
            }
            ImGui::Unindent();
        }

        if (ImGui::CollapsingHeader("Blend Map"))
        {
            ImGui::Indent();
            std::unordered_map < std::string, std::unordered_map<std::string, std::vector<int>>>& blendMap = pAnimator->blendKeyMap;

            std::vector<std::string> listStateName;
            for (auto& name : blendMap)
            {
                listStateName.push_back(name.first);
            }


            static size_t itemCurrent = 0;
            if (!listStateName.empty())
            {
                const char** comboItems = new const char* [listStateName.size()];
                for (size_t i = 0; i < listStateName.size(); ++i) {
                    comboItems[i] = listStateName[i].c_str();
                }
                ImGui::Combo("State Begin", (int*)&itemCurrent, comboItems, (int)listStateName.size());
            }

            if (!listStateName.empty())
            {
                char editname[256] = "";

                if (ImGui::InputTextEx("Rename", listStateName[itemCurrent].c_str(), editname, sizeof(editname), { 150,20 }, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    auto it = blendMap.find(listStateName[itemCurrent]);
                    blendMap.insert(std::make_pair(editname, it->second));
                    it = blendMap.erase(it);
                    listStateName[itemCurrent] = editname;

                }
            }

            if (ImGui::Button("+Add"))
            {
                std::vector<int> temp = { 0 };
                std::unordered_map<std::string, std::vector<int>> tempMap;
                tempMap.insert(std::make_pair("NONStateEnd", temp));
                std::string name = std::to_string(blendMap.size() + 1) + " :NONStateBegin";
                blendMap.insert(std::make_pair(name, tempMap));
                listStateName.push_back(name);
                itemCurrent = blendMap.size() - 1;
                rebuild = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete"))
            {
                auto it = blendMap.find(listStateName[itemCurrent]);
                listStateName.erase(listStateName.begin() + itemCurrent);
                itemCurrent--;
                it = blendMap.erase(it);
            }

            ImGui::Separator();
            ImGui::Separator();
            if (!listStateName.empty())
            {
                std::unordered_map<std::string, std::vector<int>>& listState = blendMap.at(listStateName[itemCurrent]);
                std::vector<std::string> listToStateName;

                for (auto& name : listState)
                {
                    listToStateName.push_back(name.first);
                }


                static size_t itemCurrent2 = 0;
                if (!listToStateName.empty())
                {
                    const char** comboItems = new const char* [listToStateName.size()];
                    for (size_t i = 0; i < listToStateName.size(); ++i) {
                        comboItems[i] = listToStateName[i].c_str();
                    }
                    ImGui::Combo(" To State", (int*)&itemCurrent2, comboItems, (int)listToStateName.size());
                }
                if (!listToStateName.empty())
                {
                    char editname[256] = "";

                    if (ImGui::InputTextEx("Rename To", listToStateName[itemCurrent2].c_str(), editname, sizeof(editname), { 150,20 }, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        auto it = listState.find(listToStateName[itemCurrent2]);
                        listState.insert(std::make_pair(editname, it->second));
                        it = listState.erase(it);
                        listToStateName[itemCurrent2] = editname;
                        rebuild = true;
                    }

                    if (ImGui::Button("+Add To"))
                    {
                        std::vector<int> temp = { 0 };
                        std::string name = std::to_string(listState.size() + 1) + " :NONStateEnd";
                        listState.insert(std::make_pair(name, temp));
                        listToStateName.push_back(name);
                        itemCurrent2 = listState.size() - 1;
                        rebuild = true;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Delete To"))
                    {
                        auto it = listState.find(listToStateName[itemCurrent2]);
                        listToStateName.erase(listToStateName.begin() + itemCurrent2);
                        itemCurrent2--;
                        it = listState.erase(it);
                        rebuild = true;
                    }

                    ImGui::Separator();
                    ImGui::Separator();

                    std::vector<int>& listFrame = listState.at(listToStateName[itemCurrent2]);
                    static int maxFrame = 0;
                    if (ImGui::InputInt("Max Frame", &maxFrame, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        std::vector<int> t = listFrame;
                        listFrame.resize(maxFrame);
                        for (size_t i = 0; i < t.size(); i++)
                        {
                            if (i == listFrame.size())
                                break;
                            listFrame[i] = t[i];
                        }
                        rebuild = true;
                    }
                    static int allframe = 1;
                    if (ImGui::InputInt("All frame", &allframe, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        for (auto& it : listFrame)
                            it = allframe;
                        rebuild = true;
                    }



                    ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg 
                        | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable 
                        | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

                    if (ImGui::BeginTable("Frame", 2, flags))
                    {
                        ImGui::TableSetupScrollFreeze(0, 1);
                        ImGui::TableSetupColumn("Frame", ImGuiTableColumnFlags_WidthFixed);
                        ImGui::TableSetupColumn("To Frame", ImGuiTableColumnFlags_WidthStretch);
                        ImGui::TableHeadersRow();
                        for (size_t i = 0; i < listFrame.size(); i++)
                        {
                            ImGui::TableNextColumn();
                            ImGui::Text(std::to_string(i).c_str());

                            ImGui::TableNextColumn();
                            std::string buf = std::to_string(listFrame[i]);
                            char editFrame[256] = "";
                            ImGui::PushID((int)i);
                            if (ImGui::InputTextEx("", buf.c_str(), editFrame, sizeof(editFrame), { 150,20 }, ImGuiInputTextFlags_EnterReturnsTrue))
                            {
                                listFrame[i] = std::stoi(editFrame);
                                rebuild = true;
                            }
                            ImGui::PopID();

                        }
                        ImGui::EndTable();
                    }
                }
            }
            ImGui::Unindent();
        }

        if (rebuild)
            pAnimator->CreateNewCereal();
    }
    ImGui::End();
#endif // USE_IMGUI
}

void ContentBrowser::DebugAnimation()
{
#ifdef USE_IMGUI
    if (selectAnimation == "") return;

    ImGui::Begin("Debug Animation");
    ImGui::Indent();
    static pSmartVoid animation;
    GameEngine::get()->LoadAnimation(selectAnimation, animation);
    std::shared_ptr<Animation> pAnimation = GetFromPoint<Animation>(animation);
    if (pAnimation.get())
    {
        std::string name = pAnimation->GetName();
        if (ImGui::Button(name.c_str()))
            SetItemSelect(pAnimation->local);
        


    }
    ImGui::Unindent();
    ImGui::End();
#endif // USE_IMGUI
}

void ContentBrowser::MenuBar()
{
#ifdef USE_IMGUI
    ControlPad* controlPad = GetFrom<ControlPad>(GameEngine::get()->getControlPad());
    static bool mouseClick; 
    mouseClick = ImGui::IsMouseClicked(ImGuiMouseButton_Right);
    if (selectMaterial != "")
    {
        if (ImGui::IsWindowHovered())
        {
            if (controlPad->PressDelete(0))
            {
                DeleteFileMaterial();
            }
        }

    }
    if (selectAnimator != "")
    {
        if (ImGui::IsWindowHovered())
        {
            if (controlPad->PressDelete(0))
            {
                DeleteFileAnimator();
            }
        }
    }


    if (mouseClick && ImGui::IsWindowHovered())
    {
        ImGui::OpenPopup("ContentBrowser");
    }
    
    
    if (ImGui::BeginPopup("ContentBrowser"))
    {
        bool disable = true;
        if (ImGui::BeginMenu("Create"))
        {
            if (ImGui::MenuItem("Create New Material"))
            {
                CreateNewMaterial();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Create New Animator"))
            {
                CreateNewAnimator();
            }


            ImGui::EndMenu();
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Copy", nullptr, nullptr, disable))
        {
        }
        if (ImGui::MenuItem("Paste", nullptr, nullptr, disable))
        {
        }
        if (ImGui::MenuItem("Delete", nullptr, nullptr, disable))
        {
            DeleteFileMaterial();
        }
        if (ImGui::MenuItem("Rename", nullptr, nullptr, disable))
        {
            renameEdit = selectItem;

        }
        ImGui::EndPopup();
    }
   
   
#endif // USE_IMGUI

    
}

void ContentBrowser::DeleteFileMaterial()
{
    GameEngine::get()->DeleteMaterial(selectMaterial);
    RemoveMaterialLocalFileFromQueue(selectMaterial);
    std::filesystem::remove(selectMaterial);
    selectMaterial = "";
    selectItem = "";
}

void ContentBrowser::DeleteFileAnimator()
{
    GameEngine::get()->DeleteMaterial(selectAnimator);
    RemoveMaterialLocalFileFromQueue(selectAnimator);
    std::filesystem::remove(selectAnimator);
    selectAnimator = "";
    selectItem = "";
}

void ContentBrowser::RemoveMaterialLocalFileFromQueue(std::filesystem::path local)
{
    {
        auto it = materialFiles.find(local);
        if(it != materialFiles.end())
        it = materialFiles.erase(it);
    }
    {
        auto it = materialLoadQueue.find(local);
        if (it != materialLoadQueue.end())
        it = materialLoadQueue.erase(it);
    }
}

void ContentBrowser::CreateNewMaterial()
{
#ifdef USE_IMGUI
    std::filesystem::path t = GameEngine::get()->CreateNewDummyMaterial(contentDirectory);
    DistinctSelectItem(t);
#endif // USE_IMGUI
}

void ContentBrowser::CreateNewAnimator()
{
#ifdef USE_IMGUI
    std::filesystem::path t = GameEngine::get()->CreateNewAnimator(contentDirectory);
    DistinctSelectItem(t);
#endif // USE_IMGUI
}

void ContentBrowser::DragData(const void* id)
{
#ifdef USE_IMGUI
    enum DataType
    {
        MESH,
        MATERIAL,
        TEXTURE,
        ANIMATOR,
        ANIMATION
    } dataType = TEXTURE;
    std::filesystem::path local = (char*)id;
    std::string extension = local.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), ::toupper);
    (extension == (".MATERIAL")) ? dataType = MATERIAL: 1;
    (extension == (".FBX")) ? dataType = MESH : 1;
    (extension == (".ANITOR")) ? dataType = ANIMATOR : 1;
    (extension == (".ANIM")) ? dataType = ANIMATION : 1;

    switch (dataType)
    {
    case MESH:
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            const char* str = (char*)id;
            size_t size = strlen(str) + 1;
            ImGui::SetDragDropPayload("ContentBrowserDropDragMesh", id, size);
            ImGui::EndDragDropSource();
        }
        break;
    case MATERIAL:
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            const char* str = (char*)id;
            size_t size = strlen(str) + 1;
            ImGui::SetDragDropPayload("ContentBrowserDropDragMaterial", id, size);
            ImGui::EndDragDropSource();
        }
        break;
    case TEXTURE:
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            const char* str = (char*)id;
            size_t size = strlen(str) + 1;
            ImGui::SetDragDropPayload("ContentBrowserDropDragTexture", id, size);
            ImGui::EndDragDropSource();
        }
        break;

    case ANIMATOR:
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            const char* str = (char*)id;
            size_t size = strlen(str) + 1;
            ImGui::SetDragDropPayload("ContentBrowserDropDragAnimator", id, size);
            ImGui::EndDragDropSource();
        }
        break;

    case ANIMATION:
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            const char* str = (char*)id;
            size_t size = strlen(str) + 1;
            ImGui::SetDragDropPayload("ContentBrowserDropDragAnimation", id, size);
            ImGui::EndDragDropSource();
        }
        break;

    default:
        break;
    }
#endif // USE_IMGUI
}

bool ContentBrowser::EditTexturesMaterial(TextureSlotName name, Material* material)
{
#ifdef USE_IMGUI
    ImGui::Indent();
    switch (name)
    {
    case TextureSlotName::ColorMap:
        ImGui::TextWrapped("Color Map: ");
        break;
    case TextureSlotName::NormalMap:
        ImGui::TextWrapped("Normal Map: ");
        break;
    case TextureSlotName::Emmisive:
        ImGui::TextWrapped("Emmisive Map: ");
        break;
    case TextureSlotName::Metallic:
        ImGui::TextWrapped("Metallic Map: ");
        break;
    case TextureSlotName::Occlusion:
        ImGui::TextWrapped("Occlusion Map: ");
        break;
    default:
        return false;
        break;
    }
    std::string textureFileLocal = material->getTextureFileNames(name);
    if (textureFileLocal == "")
        textureFileLocal = "Dummy";

    ImGui::Image(material->getTextureShaderResource(name).Get(), {20,20});
    ImGui::SameLine();
    bool open;
    ImVec2 size = ImVec2(500, 20);
    if (ImGui::Selectable(textureFileLocal.c_str(), &open, 0, size))
    {
        if (textureFileLocal != "Dummy")
        {
            std::filesystem::path pathh = textureFileLocal;
            contentDirectory = pathh.parent_path();
            DistinctSelectItem(textureFileLocal);
            dirChanged = true;
        }
    }
    ImGui::Unindent();
    std::string local = DropData("Texture");
    if (local != "")
    {
        material->getTextureFileNames(name) = local;
        return true;
    };
    return false;
#else
    return false;
#endif // USE_IMGUI
   
}

std::string ContentBrowser::newName()
{
#ifdef USE_IMGUI
    static char newName[64] = "";
    if (ImGui::InputText("Name", newName, 64, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        std::string result = newName;
        memset(newName, 0, sizeof(newName));
        return result;
       
    }
    return std::string();
#endif // USE_IMGUI
}

void ContentBrowser::RenameEdit()
{
#ifdef USE_IMGUI
    std::string newName = this->newName();
    if (newName != "")
    {
        std::filesystem::path fileName = renameEdit;
        fileName.replace_filename(newName + fileName.extension().string());
        std::filesystem::rename(renameEdit, fileName);
        if(selectItem == selectMaterial)
            GameEngine::get()->RenameMaterial(fileName, renameEdit);
        if(selectItem == selectAnimator)
            GameEngine::get()->RenameAnimator(fileName, renameEdit);
        if (selectItem == selectAnimation)
            GameEngine::get()->RenameAnimation(fileName, renameEdit);
        DistinctSelectItem(fileName);
        renameEdit = "";
    }
    
    
    if (!ImGui::IsItemActive() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        renameEdit = "";
    }
    
#endif // USE_IMGUI

}

void ContentBrowser::DrawMeshToFramebufferThumbnail(pSmartVoid mesh, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResouceView,
    std::unordered_map<int64_t, std::shared_ptr<Material>>& dummyMaterial)
{
    ID3D11DeviceContext* dc = GameEngine::get()->getDeviceContext();
    ID3D11Device* device = GameEngine::get()->getDevice();
    std::shared_ptr<SkinnedMesh> skinnedMeshPtr = std::static_pointer_cast<SkinnedMesh>(mesh);
    thumbnailFramebuffer.get()->Draw(dc, skinnedMeshPtr.get(), dummyMaterial);
    Microsoft::WRL::ComPtr <ID3D11Texture2D> tex2D;
    D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
    Microsoft::WRL::ComPtr < ID3D11Resource> pRecource;
    thumbnailFramebuffer.get()->getShaderResourceView(0)->GetResource(&pRecource);
    thumbnailFramebuffer.get()->getShaderResourceView(0)->GetDesc(&desc);
    pRecource->QueryInterface<ID3D11Texture2D>(&tex2D);
    D3D11_TEXTURE2D_DESC texDesc{};
    tex2D->GetDesc(&texDesc);
    HRESULT hr = device->CreateTexture2D(&texDesc, 0, &tex2D);

    hr = device->CreateShaderResourceView(tex2D.Get(), &desc, shaderResouceView.ReleaseAndGetAddressOf());
    Microsoft::WRL::ComPtr < ID3D11Resource> pNewRecource;
    shaderResouceView->GetResource(&pNewRecource);
    dc->CopyResource(pNewRecource.Get(), pRecource.Get());
}

void ContentBrowser::DistinctSelectItem(std::filesystem::path item)
{
    selectItem = item;
    for (auto it : FormatTextureLoadList)
        if (item.extension() == it)
        {
            selectTexture = item;
            return;
        }
    for (auto it : FormatMeshLoadList)
        if (item.extension() == it)
        {
            selectMesh = item;
            return;
        }
    for (auto it : FormatMaterialLoadList)
        if (item.extension() == it)
        {
            selectMaterial = item;
            return;
        }
    if (item.extension() == ".anitor")
    {
        selectAnimator = item;
        return;
    }
    if (item.extension() == ".anim")
    {
        selectAnimation = item;
        return;
    }
  
}

void ContentBrowser::CheckBack(bool doubleClick)
{
#ifdef USE_IMGUI
    MousePoint* mouseP = GetFrom<MousePoint>(GameEngine::get()->getMousePoint());
    if (mouseP->BackButtonClick())
    {
        contentDirectory = contentDirectory.parent_path();
        dirChanged = true;
    }
    ImGui::Button("...", { ButtonSize,ButtonSize });
    if (ImGui::IsItemHovered() && doubleClick)
    {
        contentDirectory = contentDirectory.parent_path();
        dirChanged = true;
    }
    ImGui::TextWrapped("...");
    ImGui::NextColumn();
#endif // USE_IMGUI
}

void ContentBrowser::AddToQueue(std::filesystem::directory_entry entry)
{
    std::filesystem::path path = entry.path();
    std::string extension = path.extension().string();
    if (entry.is_directory())
    {
        for (auto& it : listDirectory)
            if (it.first == entry.path())
                return;
        listDirectory.insert(std::make_pair(entry.path(),folder));
        return;
    }
    for (auto& formatName : FormatTextureLoadList)
        if (extension == formatName)
        {
            auto it = textureLoadQueue.find(path);
            if (it == textureLoadQueue.end())
            {
                textureLoadQueue.insert(std::make_pair(entry, false));
            }
            return;
        }
    for (auto& formatName : FormatMeshLoadList)
        if (extension == formatName)
        {
            auto it = meshLoadQueue.find(path);
            if (it == meshLoadQueue.end())
            {
                meshLoadQueue.insert(std::make_pair(entry, false));
            }
            return;
        }
    for (auto& formatName : FormatMaterialLoadList)
        if (extension == formatName)
        {
            auto it = materialLoadQueue.find(path);
            if (it == materialLoadQueue.end())
            {
                materialLoadQueue.insert(std::make_pair(entry, false));
            }
            return;
        }
    if (extension == ".anitor")
    {
        auto it = animatorLoadQueue.find(path);
        if (it == animatorLoadQueue.end())
        {
            animatorLoadQueue.insert(std::make_pair(entry, false));
        }
        return;
    }


    if (extension == ".anim")
    {
        auto it = animationLoadQueue.find(path);
        if (it == animationLoadQueue.end())
        {
            animationLoadQueue.insert(std::make_pair(entry, false));
        }
        return;
    }

}

void ContentBrowser::SetItemSelect(std::filesystem::path item)
{
    selectItem = item;
    DistinctSelectItem(item);
    SetContentDirectory(item.parent_path());
}

void ContentBrowser::GetThumbnailMesh(pSmartVoid mesh, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResouceView)
{
    std::shared_ptr<SkinnedMesh> skinnedMeshPtr = std::static_pointer_cast<SkinnedMesh>(mesh);
    auto it = meshFiles.find(skinnedMeshPtr->getLocalPath());
    if (it != meshFiles.end())
    {
        shaderResouceView = it->second;
        return;
    }
    std::unordered_map<int64_t, std::shared_ptr<Material>> dummyMaterial;
    DrawMeshToFramebufferThumbnail(mesh, shaderResouceView, dummyMaterial);
}

void ContentBrowser::GetThumbnailMaterial(pSmartVoid material, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResouceView)
{
}

void ContentBrowser::DrawMaterialToFramebufferThumbnail(pSmartVoid material, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResouceView)
{
    std::shared_ptr<SkinnedMesh> pMesh = GetFromPoint<SkinnedMesh>(materialNONMesh);
    std::shared_ptr<Material> pMaterial = GetFromPoint<Material>(material);
    //std::shared_ptr<Material> pMaterialcache = pMesh->getMeshRawList().at(0).subsets.at(0).material;
    //pMesh->getMeshRawList().at(0).subsets.at(0).material = pMaterial;
    std::unordered_map<int64_t, std::shared_ptr<Material>> dummyMaterial = { {pMaterial->getUniqueId(),pMaterial}};
    DrawMeshToFramebufferThumbnail(pMesh, shaderResouceView, dummyMaterial);
    //pMesh->getMeshRawList().at(0).subsets.at(0).material = pMaterialcache;
}

#ifdef USE_IMGUI
std::string ContentBrowser::DropData(std::string dataType, ImGuiDragDropFlags flag)
{
    std::string payload_n = "";
    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload((std::string("ContentBrowserDropDrag") + dataType).c_str(), flag);
        if (payload)
        {
            payload_n = (char*)(payload->Data);
        }
        ImGui::EndDragDropTarget();
    }
    return payload_n;
}
#endif // USE_IMGUI

std::filesystem::path ContentBrowser::CheckUpdate()
{
    std::filesystem::path path_{};
    if (listOld.size() > listNew.size())
    {
        listOld = listNew;
        return listNew[listNew.size() - 1];
    }
    for (size_t i = 0; i < listNew.size(); i++)
        if (i + 1 > listOld.size() || listNew[i] != listOld[i])
        {
            listOld = listNew;
            path_ = listNew[i];
            break;
        }
    return path_;
}
