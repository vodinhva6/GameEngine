#ifndef CONTENTBROWSER_H
#define CONTENTBROWSER_H

#include <string>
#include <filesystem>
#include <vector>
#include <map>
#include <d3d11.h>
#include <thread>
#include <future>
#include "FrameThumbnail.h"
class ContentBrowser
{
public:
	void Init(std::string assetDir);
	void Update();
	void DrawOn();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getTrashFile() { return trashFile; }

	void setDirChange(bool change) { dirChanged = change; }
	void SetItemSelect(std::filesystem::path item);
	void GetThumbnailMesh(pSmartVoid mesh, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResouceView);
	void GetThumbnailMaterial(pSmartVoid material, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResouceView);
#ifdef USE_IMGUI
	std::string DropData(std::string dataType, ImGuiDragDropFlags flag = 0);
#endif // USE_IMGUI
	~ContentBrowser();
private:
	std::filesystem::path OriginalDirectory;
	std::filesystem::path contentDirectory;
	std::vector<std::filesystem::path> listNew;
	std::vector<std::filesystem::path> listOld;


	std::filesystem::path selectMesh;
	std::filesystem::path selectMaterial;
	std::filesystem::path selectTexture;
	std::filesystem::path selectAnimator;
	std::filesystem::path selectAnimation;


	std::filesystem::path selectItem;
	std::filesystem::path renameEdit;
private:
	void Query(std::filesystem::path dir);
	void DeleteThumbnails();
	void DeleteTexturesLoaded();
	void DeleteMeshsLoaded();
	void DeleteMaterialsLoaded();
	void DeleteAnimatorsLoaded();
	void DeleteAnimationsLoaded();

	void LoadTextureToThumbnails();
	void LoadMeshToThumbnails();
	void LoadMaterialToThumbnails();
	void LoadAnimatorToThumbnails();
	void LoadAnimationToThumbnails();
	void SearchDirectory();
	void CheckRemoveFromList();
	void DrawThumbnails();
	void LoadThumbnails();
	void DebugSelectItem();
	void ButtonDetail(std::pair<std::filesystem::path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> pair, bool click, bool doubleClick = false);
	
	void DebugMaterial();
	void DebugMesh();
	void DebugAnimator();
	void DebugAnimation();

	void SetContentDirectory(std::filesystem::path directory);

	void MenuBar();
	void DeleteFileMaterial();
	void DeleteFileAnimator();
	void RemoveMaterialLocalFileFromQueue(std::filesystem::path local);
	void CreateNewMaterial();
	void CreateNewAnimator();
	void DragData(const void* id);
	bool EditTexturesMaterial(TextureSlotName name, Material* material);
	std::string newName();
	void RenameEdit();
	void DrawMaterialToFramebufferThumbnail(pSmartVoid material, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResouceView);
	void DrawMeshToFramebufferThumbnail(pSmartVoid mesh, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResouceView,
		std::unordered_map<int64_t, std::shared_ptr<Material>>& dummyMaterial);
	void DistinctSelectItem(std::filesystem::path item);
	void CheckBack(bool doubleClick);
	void AddToQueue(std::filesystem::directory_entry entry);
	std::filesystem::path CheckUpdate();
private:
	float ButtonsDis;
	float ButtonSize;
	float TableSize;
	int ColumnSize;
	bool loadingTexture;
	bool loadingMesh;
	bool loadingMaterial;
	bool loadingAnimator;
	bool loadingAnimation;
	bool dirChanged;
	
private:
	std::map<std::filesystem::path, bool> textureLoadQueue;
	std::map<std::filesystem::path, bool> meshLoadQueue;
	std::map<std::filesystem::path, bool> materialLoadQueue;
	std::map<std::filesystem::path, bool> animatorLoadQueue;
	std::map<std::filesystem::path, bool> animationLoadQueue;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> play;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> folder;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> unknownFile;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> animationFile;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> loadingFile;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> animatorFile;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> trashFile;
	

	pSmartVoid materialNONMesh;
	//pSmartVoid editingMaterial;
	std::map<std::filesystem::path,Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textureFiles;
	std::map<std::filesystem::path,Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> meshFiles;
	std::map<std::filesystem::path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> mesh;
	std::map<std::filesystem::path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> materialFiles;
	std::map<std::filesystem::path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> listDirectory;
	std::map<std::filesystem::path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> animatorFiles;
	std::map<std::filesystem::path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> animationFiles;

	std::vector<std::string> FormatTextureLoadList;
	std::vector<std::string> FormatMaterialLoadList;
	std::vector<std::string> FormatMeshLoadList;
	std::vector<std::string> UnreadFormatFile;

	std::unique_ptr<std::thread> LoadTextureThread;
	std::unique_ptr<std::thread> LoadMeshThread;
	std::unique_ptr<std::thread> LoadMaterialThread;
	std::unique_ptr<std::thread> LoadAnimatorThread;
	std::unique_ptr<std::thread> LoadAnimationThread;


	std::unique_ptr<FrameThumbnail> thumbnailFramebuffer;
};

#endif // !CONTENTBROWSER_H