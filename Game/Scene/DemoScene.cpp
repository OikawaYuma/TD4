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
	ModelManager::GetInstance()->LoadModel("Resources/map", "map.obj");
	ModelManager::GetInstance()->LoadModel("Resources/map", "map0.obj");
	ModelManager::GetInstance()->LoadModel("Resources/ball", "ball.obj");
	//ModelManager::GetInstance()->LoadModel("Resources/map", "IROHAmap.obj");
	wood_ = std::make_unique<WorldDesign>();
	wood_->Init({ 1.0f,1.0f,1.0f }, { 0.0f,15.0f,30.0f }, "map0");
	fade_ = std::make_unique<Fade>();
	fade_->Init("Resources/fade.png");
	fade_->SetTexture(TextureManager::GetInstance()->StoreTexture("Resources/fade.png"));
	spTx_ = TextureManager::GetInstance()->StoreTexture("Resources/load2.png");
	sprite_ = std::make_unique<Sprite>();
	sprite_->Init("Resources/load.png");
	sprite_->SetTexture(TextureManager::GetInstance()->StoreTexture("Resources/load.png"));

	ui_ = std::make_unique<UI>();
	ui_->Initialize();
	sprite_->Init("Resources/load2.png");
	
	sprite_->SetTexture(spTx_);
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	levelData_ = Loder::LoadJsonFile("Resources/json","stage1");
	GlobalVariables::GetInstance()->LoadFiles();

	particle_ = std::make_unique<Particle>();
	particle_->SetModel("ball.obj");
	particle_->Init();
	particle_->SetName("bomb");
	particle_->ApplyGlovalVariables();
	particle_->SetJsonPram();
	
	particle_->SetCamera(camera_.get());
	emitter_.count = 10;
	emitter_.frequency = 0.5f;
	emitter_.frequencyTime = 0.0f;
	emitter_.transform.rotate = { 0.0f,0.0f,0.0f };
	emitter_.transform.scale = { 0.25f,0.25f,0.25f };
	emitter_.transform.translate = { 0.0f,1.0f,25.0f };
	emitter_.randRangeXYZ =	//発生範囲を設定
	{
		{0.3f,0.7f},
		{0.2f,0.5f},
		{-0.5f,0.3f}
	};
	emitter_.size = 0.5f;
	particle_->SetEmitter(emitter_);
	particle_->SetTexture(spTx_);
	particle_->SetScleChangeFlag(false);
	ArrageObj(maps_);
	postProcess_ = std::make_unique<PostProcess>();
	postProcess_->Init();
	postProcess_->SetEffectNo(PostEffectMode::kFullScreen);
	
}

void DemoScene::Update()
{
	camera_->Move();
	if (Input::GetInstance()->TriggerKey(DIK_V)) {
		fade_->StartFadeIn();
	}
	camera_->Update();
	GlobalVariables::GetInstance()->Update();
	for (std::list<std::unique_ptr<map>>::iterator itr = maps_.begin(); itr != maps_.end(); itr++) {
		(*itr)->Update();
	}
	car_->Update();
	//particle_->CreateParticle();
	Object3dManager::GetInstance()->Update();
	postProcess_->Update();
	camera_->CameraDebug();
	sprite_->Update();
	ui_->Update();
	particle_->Update();
	fade_->Update();
	fade_->UpdateFade();
	PostEffectChange();
}
void DemoScene::Draw()
{
	Object3dManager::GetInstance()->Draw(camera_.get());
	particle_->Draw();
}

void DemoScene::PostDraw()
{
	postProcess_->Draw();
}

void DemoScene::Draw2d()
{
	sprite_->Draw();
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
}

void DemoScene::ParticleEmitter()
{

}

void DemoScene::ArrageObj(std::list<std::unique_ptr<map>>& maps)
{

	for (auto& objectData : levelData_.objects) {
		if (objectData.filename.compare("load") == 0) {

			ModelManager::GetInstance()->LoadModel("Resources/" + objectData.filename, objectData.filename + ".obj");
			std::unique_ptr<map> enemy = std::make_unique<map>();
			enemy->Init(objectData.transform.scale, {
				objectData.transform.rotate .x * 3.1415f /180.0f,
				objectData.transform.rotate.y * 3.1415f / 180.0f ,
				objectData.transform.rotate.z * 3.1415f / 180.0f
				}, objectData.transform.translate, objectData.filename);
			maps.push_back(std::move(enemy));
		}
		if (objectData.filename.compare("car") == 0) {

			ModelManager::GetInstance()->LoadModel("Resources/" + objectData.filename, objectData.filename + ".obj");
			car_ = std::make_unique<Car>();
			car_->Initialize(objectData.transform.scale, {
				objectData.transform.rotate.x,
				objectData.transform.rotate.y,
				objectData.transform.rotate.z
				}, objectData.transform.translate, objectData.filename);
		}
	}
}
