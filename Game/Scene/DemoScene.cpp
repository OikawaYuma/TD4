#include "DemoScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "IPostEffectState.h"
#include "Loder.h"
#include<stdio.h>
#include<time.h>
#include "Object3dManager.h"
#include "GlobalVariables/GlobalVariables.h"
#include <Audio.h>
void DemoScene::Init()
{
	Object3dManager::GetInstance()->Init();
	ModelManager::GetInstance()->LoadModel("Resources/worldDesign", "worldDesign.obj");
	ModelManager::GetInstance()->LoadModel("Resources/map", "IROHAmap2.obj");
	ModelManager::GetInstance()->LoadModel("Resources/ball", "ball.obj");
	ModelManager::GetInstance()->LoadModel("Resources/floor", "floor.obj");
	ModelManager::GetInstance()->LoadModel("Resources/TenQ", "TenQ.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Fence", "Fence.obj");
	ModelManager::GetInstance()->LoadModel("Resources/box", "box.obj");


	fade_ = std::make_unique<Fade>();
	fade_->Init("Resources/fade.png");

	fade_->SetTexture(TextureManager::GetInstance()->StoreTexture("Resources/fade.png"));
	spTx_ = TextureManager::GetInstance()->StoreTexture("Resources/load3.png");

	sprite_ = std::make_unique<Sprite>();
	sprite_->Init("Resources/load.png");
	sprite_->SetTexture(TextureManager::GetInstance()->StoreTexture("Resources/load.png"));
  
	std::weak_ptr<ObjectPram> objectpram = Object3dManager::GetInstance()->StoreObject("TenQ", TextureManager::GetInstance()->StoreTexture("Resources/TenQ/TenQ.png"), 0);
	if (objectpram.lock()) {
		objectpram.lock()->worldTransform.translation_ = { 0.0f,-1000000.0f,0.0f };
		objectpram.lock()->worldTransform.scale_ = { -100000.0f,100000.0f,100000.0f };
		objectpram.lock()->worldTransform.UpdateMatrix();
	}
	Object3dManager::GetInstance()->StoreObject("floor", TextureManager::GetInstance()->StoreTexture("Resources/kusa2.png"), 0);
	Object3dManager::GetInstance()->StoreObject("map1", TextureManager::GetInstance()->StoreTexture("Resources/load4.png"), 0);
	worldTransform_.Initialize();

	ui_ = std::make_unique<UI>();
	ui_->Initialize();
	sprite_->Init("Resources/load2.png");

	carGear_ = std::make_unique<Gear>();
	carGear_->Initialize();
	
	sprite_->SetTexture(spTx_);
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	levelData_ = Loder::LoadJsonFile("Resources/json","map");
	GlobalVariables::GetInstance()->LoadFiles();
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Init();
	ArrageObj(maps_);

	

	carSmoke_ = std::make_unique<CarSmoke>();
	carSmoke_->SetCamera(followCamera_->GetCamera());
	carSmoke_->Init();

	// 速度メーターにスピードのポインタを渡す
	ui_->SetSpeed(car_->GetSpeed());

	followCamera_->SetSpeed(car_->GetSpeed());
	WorldTransform* wt = car_->GetWorldTransform();
	followCamera_->SetTarget(wt);

	carSmoke_->SetParent(car_->GetWorldTransform());


	depthOutlineInfo_.farClip = 55.0f;
	depthOutlineInfo_.diffSize = { 0.0f,1.0f };
	postProcess_ = std::make_unique<PostProcess>();
	postProcess_->Init();
	postProcess_->SetCamera(followCamera_->GetCamera());
	postProcess_->SetEffectNo(PostEffectMode::kDepthOutline);

	wall_ = std::make_unique<Wall>();
	wall_->Initialize({ -0.523599f ,0,0 }, { 10.0f,0.5f,10.0f }, { 0,0,20.0f }, "box");

	// collisionManager
	collisionManager_ = std::make_unique<CollisionManager>();

}

void DemoScene::Update()
{
	followCamera_->Upadate();

	camera_->Move();
	if (Input::GetInstance()->TriggerKey(DIK_V)) {
		fade_->StartFadeIn();
	}
#ifdef _DEBUG
	PostEffectChange();
	camera_->CameraDebug();
#endif // _DEBUG
	if (Input::GetInstance()->GetJoystickState()) {
		if (Input::GetInstance()->PushJoyButton(XINPUT_GAMEPAD_B)) {
			sceneNo = TITLE;
		}

	}
	// カメラの視点によってアウトラインのパラメータを変更
	DepthOutlinePramChange();
	camera_->Update();
	GlobalVariables::GetInstance()->Update();
	for (std::list<std::unique_ptr<map>>::iterator itr = maps_.begin(); itr != maps_.end(); itr++) {
		(*itr)->Update();
	}
	for (std::list<std::unique_ptr<Fence>>::iterator itr = fences_.begin(); itr != fences_.end(); itr++) {
		(*itr)->Update();
	}
	car_->Update();
	//particle_->CreateParticle();
	Object3dManager::GetInstance()->Update();
	postProcess_->Update();
	

	sprite_->Update();
	carGear_->Update();
	ui_->SetGear(carGear_->GetCurrentGear());
	ui_->Update();
	//ui_->SetSpeed(carGear_->GetCurrentSpeed());
	//particle_->Update();

	fade_->Update();
	fade_->UpdateFade();
	carSmoke_->Update();

	wall_->Update();

	Collision(); 
}
void DemoScene::Draw()
{

	Object3dManager::GetInstance()->Draw(followCamera_->GetCamera());
	/*for (const auto& hitBoxWire : hitBoxWires_) {
		hitBoxWire->Draw();
	}*/
	//carSmoke_->Draw();
}

void DemoScene::PostDraw()
{
	postProcess_->Draw();
	
}

void DemoScene::Draw2d()
{
	//carSmoke_->Draw();
	//sprite_->Draw();
	ui_->Draw();
	//sprite_->Draw();
	fade_->Draw();

}

void DemoScene::Release() {
}

// ゲームを終了
int DemoScene::GameClose()
{
	return false;
}

void DemoScene::PostEffectChange()
{
	ImGui::Begin("PostEffect");
	Vector2 viggnetDarkness = postProcess_->GetDarkness();
	float gauss = postProcess_->GetDeviation();
	float threa = postProcess_->GetThreshold();
	time_t currentTime = time(nullptr);
	BloomInfo bloomInfo = postProcess_->GetBloominfo();
	srand(unsigned int(currentTime));
	int eye = rand() % 70 + 1;
	Vector2 randaa = { float(eye),float(rand() % 90 + 2) };
	hsv = {
		postProcess_->GetHSVInfo().hue,
		postProcess_->GetHSVInfo().saturation,
		postProcess_->GetHSVInfo().value,
	};
	if (ImGui::TreeNode("Base(now hsv)")) {
		if (ImGui::Button("Base On ")) {
			postProcess_->SetEffectNo(PostEffectMode::kFullScreen);
		}


		ImGui::SliderFloat("hue", &hsv.x, -1.0f, 1.0f);
		ImGui::SliderFloat("saturation", &hsv.y, -1.0f, 1.0f);
		ImGui::SliderFloat("value", &hsv.z, -1.0f, 1.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GrayScale")) {
		if (ImGui::Button("GrayScale On")) {
			postProcess_->SetEffectNo(PostEffectMode::kGrayscale);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Viggnet")) {
		if (ImGui::Button("Viggnet On")) {
			postProcess_->SetEffectNo(PostEffectMode::kVignetting);
		}
		ImGui::SliderFloat("darkness 1", &viggnetDarkness.x, 0.0f, 16.0f);
		ImGui::SliderFloat("darkness 2", &viggnetDarkness.y, 0.0f, 1.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GaussianFilter")) {
		if (ImGui::Button("Gaussian On")) {
			postProcess_->SetEffectNo(PostEffectMode::kGaussianFilter);
		}
		ImGui::SliderFloat("Devaition", &gauss, 0.01f, 10.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("DepthOutline")) {
		if (ImGui::Button("DepthOutline On")) {
			postProcess_->SetEffectNo(PostEffectMode::kDepthOutline);
		}
		ImGui::DragFloat("farClip", &depthOutlineInfo_.farClip, 1.0f);
		ImGui::DragFloat2("dissSize", &depthOutlineInfo_.diffSize.x, 0.1f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Radial Blur")) {
		if (ImGui::Button("Radial Blur On")) {
			postProcess_->SetEffectNo(PostEffectMode::kRadialBlur);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Dissolve")) {
		if (ImGui::Button("Dissolve On")) {
			postProcess_->SetEffectNo(PostEffectMode::kDissolve);
		}
		ImGui::SliderFloat("Devaition", &threa, 0.00f, 1.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Random")) {
		if (ImGui::Button("Random On")) {
			postProcess_->SetEffectNo(PostEffectMode::kRandom);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Bloom")) {
		if (ImGui::Button("Bloom On")) {
			postProcess_->SetEffectNo(PostEffectMode::kBloom);
		}

		ImGui::SliderFloat("luminance", &bloomInfo.luminance, 0.2f, 0.7f);
		ImGui::SliderFloat("Devaition", &bloomInfo.deviation, 0.0f, 10.0f);
		ImGui::SliderFloat("brightness", &bloomInfo.brightness, 0.0f, 10.0f);
		ImGui::SliderInt("kaernel", &bloomInfo.kernelSize, 1, 10);
		ImGui::TreePop();
	}
	ImGui::End();
	postProcess_->SetDarkness(viggnetDarkness);
	postProcess_->SetDeviation(gauss);
	postProcess_->SetThreshold(threa);
	postProcess_->Setrandom(randaa);
	postProcess_->SetBloomInfo(bloomInfo);
	postProcess_->SetHSVInfo({ hsv.x,hsv.y,hsv.z });
	postProcess_->SetDepthOutlineInfo(depthOutlineInfo_);
}

void DemoScene::DepthOutlinePramChange()
{
	if(followCamera_->IsFirstPerson()){
		depthOutlineInfo_.farClip = 35.0f;
		depthOutlineInfo_.diffSize = { 0.0f,1.0f };
		postProcess_->SetDepthOutlineInfo(depthOutlineInfo_);
	}
	else {
		depthOutlineInfo_.farClip = 55.0f;
		depthOutlineInfo_.diffSize = { 0.0f,1.0f };
		postProcess_->SetDepthOutlineInfo(depthOutlineInfo_);
	}
	
}

void DemoScene::ParticleEmitter()
{

}

void DemoScene::ArrageObj(std::list<std::unique_ptr<map>>& maps)
{
	maps;
	for (auto& objectData : levelData_.objects) {
		if (objectData.filename.compare("load") == 0) {

			ModelManager::GetInstance()->LoadModel("Resources/" + objectData.filename, objectData.filename + ".obj");
			std::unique_ptr<map> enemy = std::make_unique<map>();
			enemy->Init(objectData.transform.scale, {
				objectData.transform.rotate .x,
				objectData.transform.rotate.y,
				objectData.transform.rotate.z
				}, objectData.transform.translate, objectData.filename);
			maps.push_back(std::move(enemy));
		}
		if (objectData.filename.compare("car") == 0) {

			ModelManager::GetInstance()->LoadModel("Resources/carBody",   "carBody.obj");
			car_ = std::make_unique<Car>();
			car_->Initialize(objectData.transform.scale, {
				objectData.transform.rotate.x,
				objectData.transform.rotate.y,
				objectData.transform.rotate.z
				}, objectData.transform.translate, "carBody");
		}
		if (objectData.filename.compare("Fence") == 0) {

			ModelManager::GetInstance()->LoadModel("Resources/" + objectData.filename, objectData.filename + ".obj");
			std::unique_ptr<Fence> fence = std::make_unique<Fence>();
			fence->Initialize(objectData.transform.rotate / 180, 
				objectData.transform.scale
				, objectData.transform.translate, objectData.filename);
			fence->SetCollisionScale(objectData.collisionSize);
			std::unique_ptr<HitBoxWire> hitBoxWire = std::make_unique<HitBoxWire>();
			hitBoxWire->Init(objectData.collisionSize,objectData.transform.translate);
			hitBoxWire->SetCamera(followCamera_->GetCamera());
			hitBoxWires_.push_back(std::move(hitBoxWire));
			fences_.push_back(std::move(fence));
		}
		if (objectData.filename.compare("guardrail") == 0) {

			ModelManager::GetInstance()->LoadModel("Resources/" + objectData.filename, objectData.filename + ".obj");
			std::unique_ptr<Fence> fence = std::make_unique<Fence>();
			fence->Initialize({ (objectData.transform.rotate.x + (3.1415f /2)),(objectData.transform.rotate.y),(objectData.transform.rotate.z) },
				objectData.transform.scale
				, objectData.transform.translate, objectData.filename);
			fence->SetCollisionScale(objectData.collisionSize);
			std::unique_ptr<HitBoxWire> hitBoxWire = std::make_unique<HitBoxWire>();
			hitBoxWire->Init(objectData.collisionSize,objectData.transform.translate);
			hitBoxWire->SetCamera(followCamera_->GetCamera());
			hitBoxWires_.push_back(std::move(hitBoxWire));
			fences_.push_back(std::move(fence));
		}
	}
}

void DemoScene::Collision()
{
	collisionManager_->ColliderClear();

	collisionManager_->PushCollider(car_->GetBodyCollider());
	for (const auto& fence : fences_) {
		collisionManager_->PushCollider(fence->GetCollider());
	}

	collisionManager_->PushCollider(wall_->GetCollider());
	collisionManager_->CheckAllCollision();

	
}

