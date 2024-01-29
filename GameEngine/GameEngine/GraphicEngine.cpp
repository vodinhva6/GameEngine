#include "GraphicEngine.h"
#include "GameEngine.h"
#include "Actor.h"
#include "Misc.h"
void GraphicEngine::CreateCOM(ID3D11Device* device, IDXGISwapChain1* swapChain, const SIZE& frameSize)
{
	CreateRenderTagert(device, swapChain);
	CreateSamplerState(device);
	CreateBlendState(device);
	CreateDepthStencilView(device, swapChain, frameSize);
	CreateDepthStencilState(device);
	CreateRasterizerState(device);
	CreateConstantBuffer(device);
	CreateShaderManager(device);
	CreateTextureManager();
	CreateMaterialManager();
	CreateMeshLoaderManager();
	CreateSprite3DLoaderManager();
	CreateAnimatorManager();
	CreateFramebuffer(device, swapChain);
	


}

std::shared_ptr<Actor> aac;
std::shared_ptr<Actor> test1;
std::shared_ptr<Actor> test2;
#include "Player.h"
#include "ActorManager.h"
void GraphicEngine::Init()
{
	CreateTextFont();
	CreateSceneManager();
	SetSlotVSConstantBuffer();
	SetSlotPSConstantBuffer();
	SetSlotSampler();

	for (size_t i = 0; i < framebufferManager.get()->getSizeListFrameBuffer(); i++)
	{
		std::vector<ObjectDrawData*> dataBuffer;
		uninvisibleDataObjectsFrameBuffer.push_back(dataBuffer);
	}
	float farDis = sceneManager->getCameraManager()->getCamera(CameraName::GameScene)->getPerspectiveInforNow().farDis;
	
	
	aac.reset(new OBJ3D);
	std::shared_ptr<SkinnedMesh>& mesh1 = std::dynamic_pointer_cast<OBJ3D>(aac)->meshInfor.mesh_;
	
	pSmartVoid pMesh = SetToPoint(mesh1);
	createSkinnedMesh("Data/Asset/3DObject/Player/Lich/FreeLich.fbx", pMesh);
	mesh1 = GetFromPoint<SkinnedMesh>(pMesh);
	//dd->append_animations("./Data/Asset/3DObject/Player/Lich/Animation/Walk.fbx", 60, false);
	//aac->getOBJ3DActor()->setAnimat(AnimatNum::Idle, 1);
	aac->setScale({ 1,1,1 });
	aac->setColor({ 1,1,1,1 });
	aac->setPosition({ 0,5,0 });
	aac->setName("Lich");
	ActorManager* actorManager = GetFrom<ActorManager>(GameEngine::get()->getActorManager());
	actorManager->AddListActor(aac);

	//test1.reset(new OBJ3D);
	//createSkinnedMesh("Data/Asset/3DObject/Haikei/stage1.fbx", test1->getOBJ3DActor()->mesh_, false, shaderData, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
	////aac->mesh_->append_animations("./Data/Asset/3DObject/Player/Lich/Animation/Walk.fbx", 60, false);
	////aac->setAnimat(AnimatNum::Idle, 1);
	//test1->setScale({ 1,1,1 });
	//test1->setColor({ 1,1,1,1 });
	//test1->setPosition({ 0,0,0 });
	//test1->setSize(1);
	//test1->setName("Haikei");
	//actorManager->AddListActor(test1);

	test2.reset(new OBJ3D);
	std::shared_ptr<SkinnedMesh>& mesh2 = std::dynamic_pointer_cast<OBJ3D>(test2)->meshInfor.mesh_;
	pMesh = SetToPoint(mesh2);
	createSkinnedMesh("Data/Asset/3DObject/Player/DemonGirl/DemonGirlMesh.fbx", pMesh);
	mesh2 = GetFromPoint<SkinnedMesh>(pMesh);
	test2->setScale({ 1,1,1 });
	test2->setColor({ 1,1,1,1 });
	test2->setPosition({ 10,0,0 });
	test2->setName("DemonGirl");
	actorManager->AddListActor(test2);

	//aab = new OBJ3D;
	//shaderData = { "./Data/Shaders/skinned_mesh_vs.cso", "./Data/Shaders/skinned_mesh_ps.cso" };
	//aab->mesh_ = createSkinnedMesh("./Data/Asset/3DObject/Player/Lich/FreeLich.fbx", true, shaderData, { 0,0,0 }, { 0,0,0 }, { 5,5,5 });
	////aac->mesh_->append_animations("./Data/Asset/3DObject/Player/Lich/Animation/Walk.fbx", 60, false);
	////aac->setAnimat(AnimatNum::Idle, 1);
	//aab->scale_ = { 1,1,1 };
	//aab->color_ = { 1,1,1,1 };
	//aab->position_ = { 10,0,10 };
	//aab->size_ = 1;
	//GameEngine::get()->getActorManager()->addListActor(aab);
}

void GraphicEngine::Update(float elapsedTime)
{

	sceneManager.get()->UpdateChangeScene();
	sceneManager.get()->Update(elapsedTime);
	
	EditSkymap();
	
	UpdateResources();
}



	
#include <DirectXMath.h>

void GraphicEngine::Render()
{
	sceneManager.get()->DrawSubscribe();
	ClearRenderTagertAndDepthStencil({ 0.5f,0.5f,0.5f,1 });
	framebufferManager.get()->ClearAllBuffer(immediateContext.Get());
	Skymap* skymap = sceneManager->getSkymap();
	immediateContext->PSSetShaderResources(4, 1, skymap->getTextureShaderResourceView().GetAddressOf());
	framebufferManager.get()->DrawOnFrameBuffer(immediateContext.Get());

#ifdef USE_IMGUI
	framebufferManager.get()->DrawToDebug();
#else
	framebufferManager.get()->BlitMainFrameBufferToScreen(immediateContext.Get(),shaderManager.get());
#endif // USE_IMGUI
}

void GraphicEngine::DrawSkymap(bool debug)
{
	Skymap* skymap = sceneManager->getSkymap();
	shaderManager->BeginBlitFullScreenQuadSkymap(immediateContext.Get(), debug);
	skymap->Draw(immediateContext.Get(),0,1, framebufferManager->getFullscreenQuad());
	shaderManager->End(immediateContext.Get());
}

void GraphicEngine::SetIBLSkyMap()
{
	Skymap* skymap = sceneManager->getSkymap();
	skymap->SetIBL(immediateContext.Get());
}

void GraphicEngine::EditSkymap()
{
#ifdef _DEBUG
	ImGui::Begin("SkyBox");
	Skymap* skymap = sceneManager->getSkymap();
	if (ImGui::Selectable(skymap->getTextureLocal().c_str()))
	{
		GameEngine::get()->SetContentDirectory(skymap->getTextureLocal());
	}
	std::string local = GameEngine::get()->DropData("Texture");
	if (local != "")
	{
		skymap->setTextureLocal(local);
		skymap->LoadTexture();
	};
	VECTOR4& color = skymap->getColor();
	float hueShift = skymap->getHueShift();
	float saturation = skymap->getSaturation();
	float brightness = skymap->getBrightness();
	float gamma = skymap->getGamma();

	ImGui::ColorEdit4("SkyBox color", &color.x);
	ImGui::DragFloat("HueShift", &hueShift, 0.003f, -10.0f, 10.0f);
	ImGui::DragFloat("Saturation", &saturation, 0.003f, -10.0f, 10.0f);
	ImGui::DragFloat("Brightness", &brightness, 0.003f, -10.0f, 10.0f);
	ImGui::DragFloat("Gamma", &gamma, 0.003f, 0.1f, 3.0f);

	skymap->setHueShift(hueShift);
	skymap->setSaturation(saturation);
	skymap->setBrightness(brightness);
	skymap->setGamma(gamma);
	skymap->setColor(color);


	ImGui::End();
#endif // DEBUG


}

GraphicEngine::~GraphicEngine()
{
	DeleteAllEffect();
	aac.reset();
	test1.reset();
	test2.reset();
}

void GraphicEngine::drawSkinnedMesh(SkinnedMesh* obj, const DirectX::XMFLOAT4X4 world,
	std::unordered_map<int64_t, std::shared_ptr<Material>>* materialList,
	const VECTOR4& color, const int& drawTurn, DrawStates& drawStates,
	Animation::Keyframe* keyFrame, FrameBufferName slotFrameBuffer)
{
	uninvisibleDataObjectsFrameBuffer[(int)slotFrameBuffer].push_back(new SkinnedMeshData(obj, world, materialList, color, drawTurn, drawStates, keyFrame));
}


void GraphicEngine::drawEffect(std::string effName, const VECTOR3& position, const int& drawTurn, int slotFrameBuffer, float size, VECTOR3 rotation, float angle, VECTOR4 color)
{
	std::map<std::string, Effect*>::iterator fi = effects.find(effName);
	if (fi == effects.end())
		_ASSERT_EXPR(NULL, "Effect not had created");
	DrawStates drawState;
	drawState.blendState = BlendStates::Alpha;
	drawState.depthState = DepthStencilStates::DepthTestOn_DepthWriteOn;
	drawState.drawType = DrawType::EffectObject;
	drawState.rasterizerState = RasterizerStates::CullBack_DrawTriagles;
	drawState.samplerState = SamplerStates::Anisotropic;
	uninvisibleDataObjectsFrameBuffer[slotFrameBuffer].push_back(new EffectData(fi->second, position, rotation, color, size, drawTurn, angle, drawState));

}

void GraphicEngine::drawSprite3D(Sprite3D* sprite3D, const DirectX::XMFLOAT4X4 world, const VECTOR4& color, int drawTurn, FrameBufferName slotFrameBuffer)
{
	DrawStates drawState;
	drawState.blendState = BlendStates::Alpha;
	drawState.depthState = DepthStencilStates::DepthTestOn_DepthWriteOn;
	drawState.drawType = DrawType::SPRITE3DObject;
	drawState.rasterizerState = RasterizerStates::CullNON_DrawTriagles;
	drawState.samplerState = SamplerStates::Anisotropic;
	uninvisibleDataObjectsFrameBuffer[(int)slotFrameBuffer].push_back(new Sprite3DData(sprite3D, world, color, drawTurn, drawState));
}

void GraphicEngine::drawSpriteLate(Sprite* sprite, const VECTOR2& position, const VECTOR2& scale, const VECTOR4& color, const VECTOR2& texture_position, const VECTOR2& texture_size, const float& angle, const int& drawTurn, DrawStates& drawStates)
{
	const VECTOR2& size = { texture_size.x * scale.x,texture_size.y * scale.y };
	std::string contents = "";
	drawStates.drawType = DrawType::TEXTUREOBJECT;

	Object2DListLate.push_back(new SpritesData(sprite, position, size, color, texture_position, texture_size, angle, drawTurn, drawStates, contents));

}

void GraphicEngine::drawSpriteEarly(Sprite* sprite, const VECTOR2& position, const VECTOR2& scale, const VECTOR4& color, const VECTOR2& texture_position, const VECTOR2& texture_size, const float& angle, const int& drawTurn, DrawStates& drawStates)
{
	const VECTOR2& size = { texture_size.x * scale.x,texture_size.y * scale.y };
	std::string contents = "";
	drawStates.drawType = DrawType::TEXTUREOBJECT;

	Object2DListEarly.push_back(new SpritesData(sprite, position, size, color, texture_position, texture_size, angle, drawTurn, drawStates, contents));

}

void GraphicEngine::drawSpriteLate(Sprite* sprite, const VECTOR2& position, const VECTOR2& scale, const VECTOR4& color, const VECTOR2& texture_position, const VECTOR2& texture_size, const float& angle, const int& drawTurn)
{
	const VECTOR2& size = { texture_size.x * scale.x,texture_size.y * scale.y };
	std::string contents = "";
	DrawStates drawStates;
	drawStates.blendState = BlendStates::Alpha;
	drawStates.depthState = DepthStencilStates::DepthTestOff_DepthWriteOff;
	drawStates.rasterizerState = RasterizerStates::CullNON_DrawTriagles;
	drawStates.samplerState = SamplerStates::Linear;
	drawStates.drawType = DrawType::TEXTUREOBJECT;

	Object2DListLate.push_back(new SpritesData(sprite, position, size, color, texture_position, texture_size, angle, drawTurn, drawStates, contents));

}

void GraphicEngine::drawSpriteEarly(Sprite* sprite, const VECTOR2& position, const VECTOR2& scale, const VECTOR4& color, const VECTOR2& texture_position, const VECTOR2& texture_size, const float& angle, const int& drawTurn)
{
	const VECTOR2& size = { texture_size.x * scale.x,texture_size.y * scale.y };
	std::string contents = "";
	DrawStates drawStates;
	drawStates.blendState = BlendStates::Alpha;
	drawStates.depthState = DepthStencilStates::DepthTestOff_DepthWriteOff;
	drawStates.rasterizerState = RasterizerStates::CullBack_DrawTriagles;
	drawStates.samplerState = SamplerStates::Linear;
	drawStates.drawType = DrawType::TEXTUREOBJECT;

	Object2DListEarly.push_back(new SpritesData(sprite, position, size, color, texture_position, texture_size, angle, drawTurn, drawStates, contents));


}

void GraphicEngine::drawBackgroudSpriteLate(Sprite* sprite, const VECTOR2& position, const VECTOR2& size, const int& drawTurn)
{
	VECTOR4 color = { 1,1,1,1 };
	VECTOR2 texture_position = { 0,0 };
	float angle = 0;
	DrawStates drawStates;
	drawStates.blendState = BlendStates::Alpha;
	drawStates.depthState = DepthStencilStates::DepthTestOff_DepthWriteOff;
	drawStates.rasterizerState = RasterizerStates::CullBack_DrawTriagles;
	drawStates.samplerState = SamplerStates::Linear;
	std::string contents = "";
	drawStates.drawType = DrawType::BACKGROUND;
	Object2DListLate.push_back(new SpritesData(sprite, position, size, color, texture_position, { 1920,1080 }, angle, drawTurn, drawStates, contents));


}

void GraphicEngine::drawBackgroudSpriteEarly(Sprite* sprite, const VECTOR2& position, const VECTOR2& size, const int& drawTurn)
{
	VECTOR4 color = { 1,1,1,1 };
	VECTOR2 texture_position = { 0,0 };
	float angle = 0;
	DrawStates drawStates;
	drawStates.blendState = BlendStates::Alpha;
	drawStates.depthState = DepthStencilStates::DepthTestOff_DepthWriteOff;
	drawStates.rasterizerState = RasterizerStates::CullBack_DrawTriagles;
	drawStates.samplerState = SamplerStates::Linear;
	std::string contents = "";
	drawStates.drawType = DrawType::BACKGROUND;
	Object2DListEarly.push_back(new SpritesData(sprite, position, size, color, texture_position, { 1920,1080 }, angle, drawTurn, drawStates, contents));


}

void GraphicEngine::textOut(int textNo, const std::string contents, const VECTOR2& position, const float& size, const VECTOR4& color, const int& drawTurn, bool late)
{
	if (textNo > 7) return;
	std::shared_ptr<Sprite> sprite = sprites[textNo];
	DrawStates drawStates;
	drawStates.blendState = BlendStates::Alpha;
	drawStates.depthState = DepthStencilStates::DepthTestOn_DepthWriteOn;
	drawStates.rasterizerState = RasterizerStates::CullBack_DrawTriagles;
	drawStates.samplerState = SamplerStates::Anisotropic;
	drawStates.drawType = DrawType::Text;
	const VECTOR2 size_ = text_fonts_size[textNo] * size;
	const VECTOR2 texture_position = { 0,0 };
	float angle = 0;
	if (late)
	{
		Object2DListLate.push_back(new SpritesData(sprite.get(), position, size_, color, texture_position, size_, angle, drawTurn, drawStates, contents));
		return;
	}
	Object2DListEarly.push_back(new SpritesData(sprite.get(), position, size_, color, texture_position, size_, angle, drawTurn, drawStates, contents));

}


void GraphicEngine::drawDebugBounding(GeometricPrimitive* geo, const DirectX::XMFLOAT4X4 world, const VECTOR4& color, const int& type)
{
	DrawStates drawStates;
	drawStates.drawType = DrawType::BOUDING;
	uninvisibleDataObjectsFrameBuffer[(int)(FrameBufferName::FRAMEGEOMETRICDEBUGER)].push_back(new GeometricPrimitiveData(geo, world, color, drawStates, type));
}

Sprite* GraphicEngine::createSprite(const wchar_t* filename, ShaderData* shaderData)
{
	std::shared_ptr<Sprite> sp;
	if (shaderData == nullptr)
		sp.reset(new Sprite(GameEngine::get()->getDevice(), filename));
	else sp.reset(new Sprite(GameEngine::get()->getDevice(), filename, *shaderData));
	sprites.push_back(sp);
	return sprites.back().get();
}

bool GraphicEngine::createSprite3D(std::filesystem::path local, pSmartVoid& sprite3D)
{
	std::shared_ptr<Sprite3D> pSprite3D = GetFromPoint<Sprite3D>(sprite3D);
	bool result = sprite3DLoadManager->CreateSprite3D(local, pSprite3D);
	sprite3D = SetToPoint(pSprite3D);
	return result;
}

void GraphicEngine::createEffect(const char* filename, std::string effName)
{
	std::map<std::string, Effect*>::iterator fi = effects.find(effName);
	if (fi != effects.end())
		return;
	Effect* e = new Effect(filename);

	effects.insert(std::pair<std::string, Effect*>(effName, e));
}


BoundingCapsule* GraphicEngine::createCapsuleDebug(const float& height, const float& weight, const VECTOR3& offset)
{
	BoundingCapsule* capsule = nullptr;
	capsule = new BoundingCapsule(GameEngine::get()->getDevice(), height, weight, offset);
	geometricPrimitive.push_back(std::shared_ptr<BoundingCapsule>(capsule));
	return capsule;
}

BoundingSphere* GraphicEngine::createSphereDebug(const float& radius, const VECTOR3& offset)
{
	BoundingSphere* sphere = nullptr;
	sphere = new BoundingSphere(GameEngine::get()->getDevice(), radius, offset);
	geometricPrimitive.push_back(std::shared_ptr<BoundingSphere>(sphere));
	return sphere;
}
Ray* GraphicEngine::createRay(const VECTOR3& begin, const VECTOR3& end, const VECTOR4& color, RayType Type)
{
	Ray* ray = nullptr;
	ray = new Ray(GameEngine::get()->getDevice(), begin, end, color, Type);
	return ray;
}


bool GraphicEngine::createSkinnedMesh(std::filesystem::path fileLocal, pSmartVoid& mesh)
{
	return meshLoaderManager.get()->CreateMesh(mesh, fileLocal);
}

bool GraphicEngine::LoadMaterial(std::filesystem::path fileName, pSmartVoid& material)
{
	bool result = false;
	std::shared_ptr<Material> mate = GetFromPoint<Material>(material);
	result = materialManager->LoadMaterial(fileName, mate);
	material = SetToPoint(mate);
	return result;
}

bool GraphicEngine::LoadAnimator(std::filesystem::path fileName, pSmartVoid& animator)
{
	bool result = false;
	std::shared_ptr<Animator> anitor = GetFromPoint<Animator>(animator);
	result = animatorManager->LoadAnimator(fileName, anitor);
	animator = SetToPoint(anitor);
	return result;
}

bool GraphicEngine::LoadAnimation(std::filesystem::path fileName, pSmartVoid& animation)
{
	bool result = false;
	std::shared_ptr<Animation> anim = GetFromPoint<Animation>(animation);
	result = animatorManager->LoadAnimation(fileName, anim);
	animation = SetToPoint(anim);
	return result;
}

bool GraphicEngine::LoadTextureFromFile(std::wstring fileName, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, bool* result)
{
	D3D11_TEXTURE2D_DESC texture2d_desc;
	ID3D11Device* device;
	device = GameEngine::get()->getDevice();
	HRESULT hr = CoInitialize(nullptr);
	bool sucess = textureManager.get()->loadTextureFromFile(device, fileName.c_str(), shader_resource_view, &texture2d_desc);
	CoUninitialize();
	if (result != nullptr)
		*result = true;
	return sucess;
}

void GraphicEngine::ResetRenderTagertAndDepthStencil()
{
	if (renderTargetView)
	{
		renderTargetView.Reset();
	}
	if (depthStencilView)
	{
		depthStencilView.Reset();
	}
}

void GraphicEngine::RemoveQueneObject()
{
	for (size_t i = 0; i < uninvisibleDataObjectsFrameBuffer.size(); i++)
	{
		for (auto& it : uninvisibleDataObjectsFrameBuffer[i])
		{
			SafeDelete(it);
		}
		uninvisibleDataObjectsFrameBuffer[i].clear();
	}


	for (int i = 0; i < invisibleDataObjects.size(); i++)
	{
		SafeDelete(invisibleDataObjects[i]);
	}
	invisibleDataObjects.clear();

	for (int i = 0; i < Object2DListLate.size(); i++)
	{
		SafeDelete(Object2DListLate[i]);
	}
	Object2DListLate.clear();

	for (int i = 0; i < Object2DListEarly.size(); i++)
	{
		SafeDelete(Object2DListEarly[i]);
	}
	Object2DListEarly.clear();

}

void GraphicEngine::Draw2D(bool late)
{
	std::vector<ObjectDrawData*>* list = &Object2DListLate;
	if (!late)
		list = &Object2DListEarly;
	size_t n = list->size();
	if (n == 0) return;
	{
		for (int i = 0; i < n - 1; i++)
		{
			for (int j = i + 1; j < list->size(); j++)
			{
				if ((*list)[i]->drawTurn_ > (*list)[j]->drawTurn_)
				{
					ObjectDrawData* a = (*list)[i];
					(*list)[i] = (*list)[j];
					(*list)[j] = a;

				}
			}

		}
	}


	for (int i = 0; i < list->size(); i++)
	{
		SpritesData* data = (*list)[i]->getSpritesData();
		switch ((*list)[i]->drawState_.drawType)
		{
		case DrawType::BACKGROUND:
		{
			data->sprite_->render(immediateContext.Get(),
				data->position_.x, data->position_.y,
				data->size_.x, data->size_.y);
			break;
		}
		case DrawType::TEXTUREOBJECT:

			data->sprite_->render(immediateContext.Get(),
				data->position_.x, data->position_.y,
				data->size_.x, data->size_.y,
				data->color_.x, data->color_.y, data->color_.z, data->color_.w,
				data->angle_,
				data->texture_position_.x, data->texture_position_.y,
				data->texture_size_.x, data->texture_size_.y
			);
			break;
		case DrawType::Text:
			data->sprite_->textout(immediateContext.Get(), data->contents_, data->position_.x, data->position_.y,
				data->size_.x, data->size_.y, data->color_.x, data->color_.y, data->color_.z, data->color_.w
			);
			break;
		}
	}
}

void GraphicEngine::DrawAllUninvisibleObject(int frameBufferSlot, DrawType type)
{
	std::vector<ObjectDrawData*>* uninvisibleDataObjects = &uninvisibleDataObjectsFrameBuffer[frameBufferSlot];

	size_t n = uninvisibleDataObjects->size();
	if (n == 0) return;
	//if (n> 5)
	//Quicksort(data, 0, n-1);
	//else
	if (frameBufferSlot == 2)
	{
		int a = 0;
		a = 1;
	}
	{
		for (int i = 0; i < n - 1; i++)
		{
			for (int j = i + 1; j < uninvisibleDataObjects->size(); j++)
			{
				if (uninvisibleDataObjects->at(i)->drawTurn_ > uninvisibleDataObjects->at(j)->drawTurn_)
				{
					ObjectDrawData* a = (*uninvisibleDataObjects)[i];
					(*uninvisibleDataObjects)[i] = (*uninvisibleDataObjects)[j];
					(*uninvisibleDataObjects)[j] = a;

				}
			}

		}
	}


	for (int i = 0; i < (*uninvisibleDataObjects).size(); i++)
	{
		ObjectDrawData* objDrawData = (*uninvisibleDataObjects)[i];
		//m_immediate_context->OMSetBlendState(m_blend_states[(*uninvisibleDataObjects)[i]->drawState_.blendState], nullptr, 0xFFFFFFFF);
		//m_immediate_context->RSSetState(m_rasterizer_states[(*uninvisibleDataObjects)[i]->drawState_.rasterizerState]);
		//m_immediate_context->OMSetDepthStencilState(m_depth_stencil_states[(*uninvisibleDataObjects)[i]->drawState_.depthState], 1);
		if (type != objDrawData->drawState_.drawType) continue;
		switch (objDrawData->drawState_.drawType)
		{
		case DrawType::MeshLightWithEmissionShader:
		{
			SkinnedMeshData* data = objDrawData->getSkinnedMeshData();
		const VECTOR4& color = data->color_;


		//if (data->skinnedMesh_->animation_clips.empty() != 0)
		data->skinnedMesh_->Render(immediateContext.Get(), data->world_,*data->materialList, color, data->keyFrame);


		break;
		}
		case DrawType::StaticMeshObject:
		{
			//const VECTOR3& rotation = (*uninvisibleDataObjects)[i]->getStaticMeshData()->rotation_;
			//const VECTOR3& position = (*uninvisibleDataObjects)[i]->getStaticMeshData()->position_;
			//const VECTOR4& color = (*uninvisibleDataObjects)[i]->getStaticMeshData()->color_;
			//const VECTOR3& size = (*uninvisibleDataObjects)[i]->getStaticMeshData()->size_;
			//DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(size.x, size.y, size.z) };
			//DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotation.x), DirectX::XMConvertToRadians(rotation.y), DirectX::XMConvertToRadians(rotation.z)) };
			//DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(position.x, position.y, position.z) };
			//DirectX::XMStoreFloat4x4(&world, S * R * T);
			//(*uninvisibleDataObjects)[i]->getStaticMeshData()->staticMesh_->render(m_immediate_context, world, color);
			//if ((*uninvisibleDataObjects)[i]->getStaticMeshData()->drawBoudingBox_)
			//{
			//	m_immediate_context->PSSetSamplers(2, 1, &m_sampler_states[SamplerStates::Linear]);
			//	m_immediate_context->OMSetBlendState(m_blend_states[BlendStates::Alpha], nullptr, 0xFFFFFFFF);
			//	m_immediate_context->RSSetState(m_rasterizer_states[RasterizerStates::DrawLines_BackLinesOn_AntialiasedLineOn]);
			//	m_immediate_context->OMSetDepthStencilState(m_depth_stencil_states[DepthStencilStates::DepthTestOn_DepthWriteOn], 1);
			//	VECTOR3 sizeMin = (*uninvisibleDataObjects)[i]->getStaticMeshData()->staticMesh_->boundingBox.getSizeMin();
			//	VECTOR3 sizeMax = (*uninvisibleDataObjects)[i]->getStaticMeshData()->staticMesh_->boundingBox.getSizeMax();
			//
			//	//geometric3DPrimitives[1]->renderBoudingBox(m_immediate_context.Get(), world, sizeMin, sizeMax, { 1,1,1,1 });
			//
			//}
			break;
		}
		case DrawType::SkinnedMeshObject:
		{
			SkinnedMeshData* data = objDrawData->getSkinnedMeshData();
			const VECTOR4& color = data->color_;


			//if (data->skinnedMesh_->animation_clips.empty() != 0)
			data->skinnedMesh_->Render(immediateContext.Get(), data->world_, *data->materialList, color, data->keyFrame);


			break;
		}

		case DrawType::SPRITE3DObject:
		{
			Sprite3DData* data = objDrawData->getSprite3D();
			data->sprite3D->render(immediateContext.Get(), data->world, data->color);

			break;
		}
		case DrawType::EffectObject:
		{
			EffectData* effData = objDrawData->getEffectData();
			Effekseer::Handle handle = effData->eff_->Play(effData->position_, effData->scale_, effData->rotation_, effData->angle_);
			break;
		}
		case DrawType::SKYBOX:
		{
			//ShaderManager::get()->BeginDrawSkyBox(m_immediate_context, m_depth_stencil_states[DepthStencilStates::DepthTestOn_DepthWriteOn],
			//	m_blend_states[BlendStates::Alpha], m_rasterizer_states[RasterizerStates::CullingOpposition]);

			SkinnedMeshData* data = objDrawData->getSkinnedMeshData();
			const VECTOR4& color = data->color_;

			//if (data->skinnedMesh_->animation_clips.empty() != 0)
			data->skinnedMesh_->Render(immediateContext.Get(), data->world_, *data->materialList, color, data->keyFrame);



			break;
		}
		case DrawType::BOUDING:
		{
			GeometricPrimitiveData* data = objDrawData->getGeometricPrimitiveData();
			const VECTOR4& color = data->color_;
			switch (data->type_)
			{
			case 2:
				data->geometric_->renderPointStrip(immediateContext.Get(), data->world_, data->color_);
				break;

			case 3:
				data->geometric_->renderPointList(immediateContext.Get(), data->world_, data->color_);
				break;
			}
		}
		}

	}

}

void GraphicEngine::CastShadow(int frameBufferSlot, DrawType type, UINT numIndex)
{
	std::vector<ObjectDrawData*>* dataObjects = &uninvisibleDataObjectsFrameBuffer[frameBufferSlot];
	size_t n = dataObjects->size();
	for (size_t i = 0; i < n; i++)
	{
		ObjectDrawData* objDrawData = (*dataObjects)[i];
		if (type != objDrawData->drawState_.drawType) continue;
		switch (objDrawData->drawState_.drawType)
		{
			case DrawType::SkinnedMeshObject:
			{
				SkinnedMeshData* data = objDrawData->getSkinnedMeshData();
				const VECTOR4& color = data->color_;
				data->skinnedMesh_->ShadowCaster(immediateContext.Get(), data->world_, *data->materialList, color, numIndex, data->keyFrame);
				break;
			}

		}

	}
}

void GraphicEngine::CreateRenderTagert(ID3D11Device* device, IDXGISwapChain1* swapChain)
{
	HRESULT hr{ S_OK };
	ID3D11Texture2D* back_buffer{};
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&back_buffer));
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	hr = device->CreateRenderTargetView(back_buffer, NULL, renderTargetView.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	back_buffer->Release();




}

void GraphicEngine::CreateDepthStencilView(ID3D11Device* device, IDXGISwapChain1* swapChain, const SIZE& frameSize)
{
	HRESULT hr{ S_OK };
	ID3D11Texture2D* depth_stencil_buffer{};
	D3D11_TEXTURE2D_DESC texture2d_desc{};
	texture2d_desc.Width = frameSize.cx;
	texture2d_desc.Height = frameSize.cy;
	texture2d_desc.MipLevels = 1;
	texture2d_desc.ArraySize = 1;
	texture2d_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texture2d_desc.SampleDesc.Count = 1;
	texture2d_desc.SampleDesc.Quality = 0;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texture2d_desc.CPUAccessFlags = 0;
	texture2d_desc.MiscFlags = 0;
	hr = device->CreateTexture2D(&texture2d_desc, NULL, &depth_stencil_buffer);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};
	depth_stencil_view_desc.Format = texture2d_desc.Format;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(depth_stencil_buffer, &depth_stencil_view_desc, depthStencilView.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	depth_stencil_buffer->Release();


}

void GraphicEngine::CreateSamplerState(ID3D11Device* device)
{
	samplerStateManager.reset(new SamplerStateManager);
	samplerStateManager.get()->Init(device);

}

void GraphicEngine::CreateDepthStencilState(ID3D11Device* device)
{
	depthStencilStateManager.reset(new DepthStencilStateManager);
	depthStencilStateManager.get()->Init(device);
}

void GraphicEngine::CreateBlendState(ID3D11Device* device)
{
	blendStateManager.reset(new BlendStateManager);
	blendStateManager.get()->Init(device);
}

void GraphicEngine::CreateRasterizerState(ID3D11Device* device)
{
	rasterizerStateManager.reset(new RasterizerStateManager);
	rasterizerStateManager.get()->Init(device);
}

void GraphicEngine::CreateConstantBuffer(ID3D11Device* device)
{
	constantBufferManager.reset(new ConstantBufferManager);
	constantBufferManager.get()->Init(device);
}

void GraphicEngine::CreateFramebuffer(ID3D11Device* device, IDXGISwapChain1* swapChain)
{
	framebufferManager.reset(new FrameBufferManager);
	D3D11_VIEWPORT viewport;
	UINT num_viewports{ 1 };
	immediateContext.Get()->RSGetViewports(&num_viewports, &viewport);
	framebufferManager.get()->Init(swapChain, immediateContext.Get(), device,viewport);
}

void GraphicEngine::CreateShaderManager(ID3D11Device* device)
{
	shaderManager.reset(new ShaderManager);
	shaderManager.get()->CreateShaderAllLayout(device);
}

void GraphicEngine::CreateTextureManager()
{
	textureManager.reset(new TextureManager);
	textureManager->Init(GameEngine::get()->getDevice());
}

void GraphicEngine::CreateMaterialManager()
{
	materialManager.reset(new MaterialManager);
	materialManager.get()->Init();
}



void GraphicEngine::CreateSceneManager()
{
	sceneManager = std::make_unique<SceneManager>();
	sceneManager.get()->Init();
}

void GraphicEngine::CreateMeshLoaderManager()
{
	meshLoaderManager = std::make_unique<MeshLoaderManager>();
}

void GraphicEngine::CreateSprite3DLoaderManager()
{
	sprite3DLoadManager = std::make_unique<Sprite3DLoadManager>();
}


void GraphicEngine::CreateTextFont()
{
	for (int i = 0; i < 8; i++)
	{
		createSprite(textFonts[i]);
	}
}

void GraphicEngine::CreateAnimatorManager()
{
	animatorManager = std::make_unique<AnimatorManager>();
}



void GraphicEngine::SetSlotPSConstantBuffer()
{
	constantBufferManager.get()->setPSSlotNum(immediateContext.Get());
}

void GraphicEngine::SetSlotVSConstantBuffer()
{
	constantBufferManager.get()->setVSSlotNum(immediateContext.Get());
}

void GraphicEngine::UpdateResources()
{

	constantBufferManager.get()->updateResource(immediateContext.Get());
}

void GraphicEngine::SetSlotSampler()
{
	samplerStateManager.get()->setSamplerState(immediateContext.Get());
}

void GraphicEngine::NewScene()
{
	sceneManager->NewScene();
}

void GraphicEngine::SaveScene(std::filesystem::path local, bool saveAs)
{
	sceneManager->SaveScene(local, saveAs);
}

void GraphicEngine::OpenScene(std::filesystem::path local)
{
	sceneManager->OpenScene(local);
}

bool GraphicEngine::SceneHadSaveLocal()
{
	return sceneManager->CheckSceneHadSaveLocal();
}

void GraphicEngine::ClearRenderTagertAndDepthStencil(const VECTOR4& color)
{
	FLOAT colorr[]{ color.x, color.y, color.z, color.w };
	immediateContext.Get()->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
	immediateContext.Get()->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	immediateContext.Get()->ClearRenderTargetView(renderTargetView.Get(), colorr);

}


void GraphicEngine::DeleteAllEffect()
{
	for (auto it : effects)
		if (it.second)
			delete it.second;
	effects.clear();
}

float GraphicEngine::getAspectRatio(FrameBufferName name)
{
	D3D11_VIEWPORT viewport;
	viewport = framebufferManager.get()->getViewPort(name);
	return { viewport.Width / viewport.Height };
}

D3D11_VIEWPORT& GraphicEngine::getViewPort(FrameBufferName name)
{
	UINT num_viewports{ 1 };
	return framebufferManager.get()->getViewPort(name);
	//immediateContext.Get()->RSGetViewports(&num_viewports, viewport);
}

float GraphicEngine::getElapsedTime()
{
	return GameEngine::get()->getElapsedTime();
}