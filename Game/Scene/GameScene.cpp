#include "GameScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ParticleManager.h"
#include "Line/LineManager.h"
#include "IPostEffectState.h"
#include "Loder.h"
#include<stdio.h>
#include<time.h>
#include "Object3dManager.h"
#include "GlobalVariables/GlobalVariables.h"
#include <Audio.h>
#include "SharedGameData/SharedGameData.h"
void GameScene::Init()
{
	Object3dManager::GetInstance()->Init();
	LineManager::GetInstance()->Init();
	ModelManager::GetInstance()->LoadModel("Resources/worldDesign", "wood2.obj");
	ModelManager::GetInstance()->LoadModel("Resources/map", "IROHAmap2.obj");
	ModelManager::GetInstance()->LoadModel("Resources/ball", "ball.obj");
	ModelManager::GetInstance()->LoadModel("Resources/floor", "floor.obj");
	ModelManager::GetInstance()->LoadModel("Resources/TenQ", "TenQ.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Fence", "Fence.obj");
	ModelManager::GetInstance()->LoadModel("Resources/box", "box.obj");
	ModelManager::GetInstance()->LoadModel("Resources/road2", "road2.obj");
	ModelManager::GetInstance()->LoadModel("Resources/driftmap", "driftmap6.obj");
	ModelManager::GetInstance()->LoadModel("Resources/map", "map.obj");

	// 物理
	physicsSystem_ = std::make_unique<PhysicsSystem>();


	fade_ = std::make_unique<Fade>();
	fade_->Init("Resources/Black.png");
	fade_->StartFadeOut();

	// miniMap
	ModelManager::GetInstance()->LoadModel("Resources/driftmap", "minimap.obj");
	minimap_ = std::make_unique<MiniMap>();
	minimap_->Initialize({ 0.002f, 0.002f, 0.002f }, { 1.6f, 0.0f, 0.0f }, { -1.9f, 0.45f, 5.0f }, "minimap");
  
	{
		std::weak_ptr<ObjectPram> objectpram = Object3dManager::GetInstance()->StoreObject("TenQ", TextureManager::GetInstance()->StoreTexture("Resources/TenQ/TenQ.png"), 0);
		if (objectpram.lock()) {
			objectpram.lock()->worldTransform.translation_ = { 0.0f,-1000000.0f,0.0f };
			objectpram.lock()->worldTransform.scale_ = { -100000.0f,100000.0f,100000.0f };
			objectpram.lock()->worldTransform.UpdateMatrix();
		}
	}
	
	
	if (SharedGameData::GetInstance()->GetSelectedStageNo() == 0) {
		std::weak_ptr<ObjectPram> objectpram = Object3dManager::GetInstance()->StoreObject("floor", TextureManager::GetInstance()->StoreTexture("Resources/kusa2.png"), 0);
		if (objectpram.lock()) {
			objectpram.lock()->worldTransform.translation_ = { 0.0f,0.0f,128.0f };
			objectpram.lock()->worldTransform.UpdateMatrix();
		}
	}
	else if (SharedGameData::GetInstance()->GetSelectedStageNo() == 1) {
		Object3dManager::GetInstance()->StoreObject("map", TextureManager::GetInstance()->StoreTexture("Resources/load4.png"), 0);
		std::weak_ptr<ObjectPram> objectpram = Object3dManager::GetInstance()->StoreObject("floor", TextureManager::GetInstance()->StoreTexture("Resources/kusa2.png"), 0);
	}
	else if (SharedGameData::GetInstance()->GetSelectedStageNo() == 2) {
		Object3dManager::GetInstance()->StoreObject("driftmap6", TextureManager::GetInstance()->StoreTexture("Resources/driftmap/driftmap.png"), 0);
	}

	worldTransform_.Initialize();

	ui_ = std::make_unique<UI>();
	ui_->Initialize();

	carGear_ = std::make_unique<Gear>();
	carGear_->Initialize();

	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	levelData_ = Loder::LoadJsonFile("Resources/json/stage", "stage" + std::to_string(SharedGameData::GetInstance()->GetSelectedStageNo() + 1));
	GlobalVariables::GetInstance()->LoadFiles();
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Init();
	ArrageObj(maps_);
	car_->SetCamera(followCamera_->GetCamera());

	// 速度メーターにスピードのポインタを渡す
	ui_->SetSpeed(car_->GetSpeed());

	followCamera_->SetSpeed(car_->GetSpeed());
	WorldTransform* wt = car_->GetWorldTransform();
	followCamera_->SetTarget(wt);

	depthOutlineInfo_.farClip = 55.0f;
	depthOutlineInfo_.diffSize = { 0.0f,1.0f };
	postProcess_ = std::make_unique<PostProcess>();
	postProcess_->Init();
	postProcess_->SetCamera(followCamera_->GetCamera());
	postProcess_->SetEffectNo(PostEffectMode::kDepthOutline);
	// collisionManager
	collisionManager_ = std::make_unique<CollisionManager>();

	// ミニプレイヤー
	miniUI_ = std::make_unique<MiniPlayer>();
	miniUI_->Initialize();
	timer_.start();
	
	checkPoint_ = std::make_unique<CheckPoint>();
	checkPoint_->Initialize({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 40.0f }, "box");
}

void GameScene::Update()
{
	GlobalVariables::GetInstance()->Update();
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
		if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_Y) && fade_->GetAlpha() <= 0.0f) {
			fade_->StartFadeIn(); // フェードインを開始
		}

	}
	
	// カメラの視点によってアウトラインのパラメータを変更
	DepthOutlinePramChange();
	camera_->Update();
	for (std::list<std::unique_ptr<map>>::iterator itr = maps_.begin(); itr != maps_.end(); itr++) {
		(*itr)->Update();
	}
	for (std::list<std::unique_ptr<Fence>>::iterator itr = fences_.begin(); itr != fences_.end(); itr++) {
		(*itr)->Update();
	}
	car_->Update();

	Object3dManager::GetInstance()->Update();
	for (std::list<std::unique_ptr<HitBoxWire>>::iterator itr = hitBoxWires_.begin(); itr != hitBoxWires_.end(); itr++) {
		(*itr)->Update();
	}
	LineManager::GetInstance()->Update();
	postProcess_->Update();

	carGear_->Update();
	ui_->SetGear(carGear_->GetCurrentGear());
	//ui_->SetRot(car_->GetWorldTransform()->rotation_.y);
	ui_->Update();
	
	// fadeの更新
	fade_->UpdateFade();
	minimap_->Update(followCamera_->GetCamera());
	fade_->Update();

	if (fade_->GetAlpha() >= 1.0f) {
		sceneNo = TITLE; // ステージシーンに遷移
	}
	checkPoint_->Update();
	// ミニプレイヤー
	Vector2 playerPos = { car_->GetWorldTransform()->translation_.x, car_->GetWorldTransform()->translation_ .z};     // プレイヤーのワールド座標
	// プレイヤーの回転角（0〜360°）
	float playerRot = car_->GetWorldTransform()->rotation_.y; 

	// ワールド→ミニマップの縮小倍率
	float mapScale = 0.6f;

	miniUI_->SetSpeed(car_->GetSpeed());

	miniUI_->SetFromPlayer(playerPos, playerRot, miniMapOrigin, mapScale);
	miniUI_->Update();


	//physicsSystem_->Apply(1.0f / 60.0f);
	ParticleManager::GetInstance()->Update(followCamera_->GetCamera());
	Collision();

#ifdef _DEBUG
	ImGui::Begin("a");
	ImGui::DragFloat2("Transform", &miniMapOrigin.x, 0.1f);
	ImGui::DragFloat("scale", &mapScale, 0.1f);
	// タイマー
	if (ImGui::Button("Lap Record"))
	{

		timer_.recordLap();
	}
	// ラップ履歴を表示
	const auto& laps = timer_.getLaps();
	for (size_t i = 0; i < laps.size(); i++)
	{
		ImGui::Text("Lap %zu: %.2f sec", i + 1, laps[i]);
	}
	// 差分も確認
	if (laps.size() >= 2)
	{
		double diff = timer_.getLastLapDiff();
		if (diff < 0)
			ImGui::Text("- %.2f", -diff);
		else
			ImGui::Text("+ %.2f", diff);
	}
	ImGui::Text("TIME: %d%d:%d%d", timer_.elapsedTensOfMinutes(), timer_.elapsedMinutesOnly(), timer_.elapsedTensOfSeconds(), timer_.elapsedSecondsOnly());

	ImGui::End();
#endif // _DEBUG
}

void GameScene::Draw()
{

	Object3dManager::GetInstance()->Draw(followCamera_->GetCamera());
	//LineManager::GetInstance()->Draw(followCamera_->GetCamera());
	// ミニプレイヤー
	miniUI_->Draw();

}

void GameScene::PostDraw()
{
	postProcess_->Draw();

}

void GameScene::Draw2d()
{
	ParticleManager::GetInstance()->Draw();
	ui_->Draw();
	fade_->Draw();


}

void GameScene::Release() {
}

// ゲームを終了
int GameScene::GameClose()
{
	return false;
}

void GameScene::PostEffectChange()
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

void GameScene::DepthOutlinePramChange()
{
	if (followCamera_->IsFirstPerson()) {
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

void GameScene::ParticleEmitter()
{

}

void GameScene::ArrageObj(std::list<std::unique_ptr<map>>& maps)
{
	maps;
	for (auto& objectData : levelData_.objects) {
		if (objectData.filename.compare("load") == 0) {

			ModelManager::GetInstance()->LoadModel("Resources/" + objectData.filename, objectData.filename + ".obj");
			std::unique_ptr<map> enemy = std::make_unique<map>();
			enemy->Init(objectData.transform.scale, {
				objectData.transform.rotate.x,
				objectData.transform.rotate.y,
				objectData.transform.rotate.z
				}, objectData.transform.translate, objectData.filename);
			maps.push_back(std::move(enemy));
		}
		if (objectData.filename.compare("car") == 0) {

			ModelManager::GetInstance()->LoadModel("Resources/carBody", "carBody.obj");
			car_ = std::make_unique<Car>();
			car_->SetPhysicsSystem(physicsSystem_.get());
			car_->Initialize(objectData.transform.scale, {
				objectData.transform.rotate.x,
				objectData.transform.rotate.y,
				objectData.transform.rotate.z
				}, objectData.transform.translate, "carBody");

			//std::unique_ptr<HitBoxWire> hitBoxWire = std::make_unique<HitBoxWire>();
			Vector3 rotateVec = { objectData.transform.rotate.x, objectData.transform.rotate.y, objectData.transform.rotate.z };
			Vector3 collisionSize = { 1.7599999904632568f,  1.3000000715255737f,4.5f };
			//hitBoxWire->Init(collisionSize, rotateVec, { 0.0f,0.335f,0.0f });
			//hitBoxWire->SetWorldTransformParent(car_->GetWorldTransform());
			//hitBoxWires_.push_back(std::move(hitBoxWire));

		}
		if (objectData.filename.compare("Fence") == 0) {

			ModelManager::GetInstance()->LoadModel("Resources/" + objectData.filename, objectData.filename + ".obj");
			std::unique_ptr<Fence> fence = std::make_unique<Fence>();
			fence->Initialize(objectData.transform.rotate,
				objectData.transform.scale
				, objectData.transform.translate, objectData.filename);
			fence->SetCollisionScale({ 0.4f,2.0f,5.0f });
			fences_.push_back(std::move(fence));
			//std::unique_ptr<HitBoxWire> hitBoxWire = std::make_unique<HitBoxWire>();
			Vector3 rotateVec = { objectData.transform.rotate.x , objectData.transform.rotate.y, objectData.transform.rotate.z };
			Vector3 collisionSize = { 0.4f,2.0f,5.0f };
			//hitBoxWire->Init(collisionSize, rotateVec, objectData.transform.translate);
			//hitBoxWires_.push_back(std::move(hitBoxWire));
		}
		if (objectData.filename.compare("guardrail") == 0) {

			ModelManager::GetInstance()->LoadModel("Resources/" + objectData.filename, objectData.filename + ".obj");
			std::unique_ptr<Fence> fence = std::make_unique<Fence>();

			fence->Initialize({ (objectData.transform.rotate.x + 3.1415f / 2),(objectData.transform.rotate.y),(objectData.transform.rotate.z) },
				objectData.transform.scale
				, objectData.transform.translate, objectData.filename);
			fence->SetCollisionScale({ objectData.collisionSize.x, objectData.collisionSize.y,objectData.collisionSize.z+2 });
			//std::unique_ptr<HitBoxWire> hitBoxWire = std::make_unique<HitBoxWire>();
			Vector3 rotateVec = { objectData.transform.rotate.x + 3.1415f / 2 , objectData.transform.rotate.y, objectData.transform.rotate.z };
			Vector3 collisionSize = { objectData.collisionSize.x, objectData.collisionSize.y,objectData.collisionSize.z + 2 };
		//	hitBoxWire->Init(collisionSize, rotateVec, objectData.transform.translate);
			fences_.push_back(std::move(fence));
			//hitBoxWires_.push_back(std::move(hitBoxWire));
		}
	}
}

void GameScene::Collision()
{
	collisionManager_->ColliderClear();

	collisionManager_->PushCollider(car_->GetBodyCollider());
	for (const auto& fence : fences_) {
		collisionManager_->PushCollider(fence->GetCollider());
	}
	collisionManager_->PushCollider(checkPoint_->GetCollider());
	collisionManager_->CheckAllCollision();


}
