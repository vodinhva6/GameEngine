#include "FrameDebug.h"
#include "GameEngine.h"
#include "FrameBufferManager.h"
#include "ShaderManager.h"
#include "MousePoint.h"
#include "Collision.h"
#include "ActorManager.h"
#include "CameraManager.h"



void FrameSceneDebug::Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow, bool depth)
{
    FrameBuffer::Setup(swapChain, device, viewPort, msaa, name, shadow, depth);

	

}

void FrameSceneDebug::DrawedOn(ID3D11DeviceContext* immediateContext)
{
#ifdef USE_IMGUI
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Scene");
	//ImGui::PopStyleVar();

	ImVec2 ImSize = ImGui::GetContentRegionAvail();
	VECTOR2 Imsize = *((VECTOR2*)&ImSize);
	VECTOR2 ViewSize = { viewport_.Width,viewport_.Height };
	if (Imsize.x < 0 || Imsize.y < 0)
	{
		ImGui::End();
		return;
	}
	ImVec2 ImPos = ImGui::GetWindowPos();
	VECTOR2 pos = *((VECTOR2*)&ImPos);
	pos.y += 20;
	ActorManager* actorManager = GetFrom<ActorManager>(GameEngine::get()->getActorManager());
	actorManager->getActorDebug()->SetDebugWindowPos(pos);
	if (ViewSize != Imsize)
	{

		viewport_.Width = Imsize.x;
		viewport_.Height = Imsize.y;
		FrameBuffer::Resize(viewport_);
	}
	ImVec2 ImSizea = ImGui::GetContentRegionAvail();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
	ImGui::Image(shaderResourceView[0].Get(), { ImSizea.x,ImSizea.y });
	ImGui::PopStyleVar();

	static std::string payload_n = "";
	payload_n = GameEngine::get()->DropData("Mesh", ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect);
	if (payload_n != "")
	{
		bool release = ImGui::IsMouseReleased(ImGuiMouseButton_Left);
		static std::string local = "";
		static std::shared_ptr<Actor> obj(nullptr);
		if (payload_n != local)
		{
			local = payload_n;
			obj.reset(new OBJ3D);
			auto castObj = std::dynamic_pointer_cast<OBJ3D>(obj);
			pSmartVoid pMesh = SetToPoint(castObj->meshInfor.mesh_);
			GameEngine::get()->LoadSkinnedMesh(local.c_str(), pMesh);
			castObj->meshInfor.mesh_ = GetFromPoint<SkinnedMesh>(pMesh);

		}
		if (release)
		{
			payload_n = "";
			local = "";
			actorManager->getActorDebug()->setActorTagert(obj.get());
			actorManager->AddListActor(obj);
			obj.reset();

		}
		else
		{
			MousePoint* mouseP = GetFrom<MousePoint>(GameEngine::get()->getMousePoint());
			obj->setPosition(Collision::get()->ScreenToWorldTransition(mouseP->getPos() - pos, 50));
			obj->updateWorldTrans();
			obj->Draw();
		}
	}

	if(!actorManager->getActorDebug()->setTagert())
		actorManager->getActorDebug()->getTagert();
	ImGui::End();
	
#endif // USE_IMGUI
	ClearDepthStencil(immediateContext);
	ClearRenderTarget(immediateContext);
	FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
	ShaderManager* shaderManager = GetFrom< ShaderManager>(GameEngine::get()->getShaderManager());
	
    frameBufferManager->Activate(immediateContext, this);
	GameEngine::get()->DrawSkymap(true);
	GameEngine::get()->SetIBLSkyMap();
	shaderManager->BeginSkinnedMeshDrawDebug(immediateContext);
	GameEngine::get()->DrawAllUninvisibleObject(FrameBufferName::MSAABUFFER1, DrawType::SkinnedMeshObject);
	shaderManager->End(immediateContext);
	
	shaderManager->BeginMeshLightWithEmissionDebug(immediateContext);
	GameEngine::get()->DrawAllUninvisibleObject(FrameBufferName::MSAABUFFER1, DrawType::MeshLightWithEmissionShader);
	shaderManager->End(immediateContext);
	
	shaderManager->BeginDraw3DSprite(immediateContext);
	GameEngine::get()->DrawAllUninvisibleObject(FrameBufferName::MSAABUFFER1, DrawType::SPRITE3DObject);
	shaderManager->End(immediateContext);


	
	DrawDebugger(immediateContext);

	frameBufferManager->Deactivate(immediateContext, this);


   
   
	

}

void FrameSceneDebug::DrawToDebug()
{
#ifdef USE_IMGUI
	ImGui::Begin("Scene");
	ImGui::End();
#endif // USE_IMGUI
}

void FrameSceneDebug::DrawDebugger(ID3D11DeviceContext* immediateContext)
{
	FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
	ShaderManager* shaderManager = GetFrom< ShaderManager>(GameEngine::get()->getShaderManager());
   
	
	FrameBuffer* frameBuffer = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMESKINNEDMESHDEBUG);
	shaderManager->BeginBlitFullScreenQuad(immediateContext);
    frameBufferManager->BlitFrom(immediateContext, frameBuffer);
	shaderManager->End(immediateContext);

	frameBuffer = frameBufferManager->getFrameBuffer(FrameBufferName::FRAMEGEOMETRICDEBUGER);
	shaderManager->BeginBlitFullScreenQuad(immediateContext);
	frameBufferManager->BlitFrom(immediateContext, frameBuffer);
	shaderManager->End(immediateContext);


}