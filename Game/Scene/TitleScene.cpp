#include "TitleScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"
#include "Loder.h"
#include "Audio.h"
#include "Object3dManager.h"
#include <ModelManager.h>
#include <GlobalVariables/GlobalVariables.h>

void TitleScene::Init()
{
	Object3dManager::GetInstance()->Init();
	ModelManager::GetInstance()->LoadModel("Resources/carBody", "carBody.obj");
	car_ = std::make_unique<Car>();
	car_->Initialize({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 8.0f }, "carBody");

	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetPos({ -12.0f, 3.0f, 22.0f });
	camera_->SetRotate({ 0.0f, 2.3f, 0.0f });

	ModelManager::GetInstance()->LoadModel("Resources/ball", "ball.obj");
	GlobalVariables::GetInstance()->LoadFiles();
	carSmoke_ = std::make_unique<CarSmoke>();
	carSmoke_->SetCamera(camera_.get());
	carSmoke_->Init();
	carSmoke_->SetParent(car_->GetWorldTransform());

	ModelManager::GetInstance()->LoadModel("Resources", "logo.obj");
	logo_ = std::make_unique<Logo>();
	logo_->Init({ 5.0f, 5.0f, 1.0f }, { 0.0f, -1.5f, 0.0f }, { 12.0f, 1.0f, 8.0f }, "logo");

	ModelManager::GetInstance()->LoadModel("Resources/worldDesign", "worldDesign.obj");
	ModelManager::GetInstance()->LoadModel("Resources/map", "IROHAmap2.obj");
	ModelManager::GetInstance()->LoadModel("Resources/floor", "floor.obj");

	levelData_ = Loder::LoadJsonFile("Resources/json", "stage4");
	ArrageObj(maps_);

	Object3dManager::GetInstance()->StoreObject("floor", TextureManager::GetInstance()->StoreTexture("Resources/kusa.png"), 0);

	postProcess_ = std::make_unique<PostProcess>();
	postProcess_->Init();
	postProcess_->SetCamera(camera_.get());
	postProcess_->SetEffectNo(PostEffectMode::kDepthOutline);
}

void TitleScene::Update()
{
	Object3dManager::GetInstance()->Update();
	postProcess_->Update();
	if (Input::GetInstance()->GetJoystickState()) {
		if (Input::GetInstance()->PushJoyButton(XINPUT_GAMEPAD_Y)) {
			sceneNo = DEMO;
		}

	}
	for (std::list<std::unique_ptr<map>>::iterator itr = maps_.begin(); itr != maps_.end(); itr++) {
		(*itr)->Update();
	}
	camera_->Update();
	car_->Update();
	logo_->Update();
	carSmoke_->Update();

#ifdef _DEBUG
	camera_->CameraDebug();
#endif // _DEBUG

	camera_->Update();

	
}
void TitleScene::Draw()
{
	Object3dManager::GetInstance()->Draw(camera_.get());
}

void TitleScene::PostDraw()
{
	postProcess_->Draw();
	
}

void TitleScene::Draw2d()
{
	carSmoke_->Draw();
}

void TitleScene::Release() {

}

// ゲームを終了
int TitleScene::GameClose()
{
	return false;
}

void TitleScene::DeleteObject()
{
}

void TitleScene::ArrageObj(std::list<std::unique_ptr<map>>& maps)
{
	for (auto& objectData : levelData_.objects) {
		if (objectData.filename.compare("load") == 0) {
			ModelManager::GetInstance()->LoadModel("Resources/" + objectData.filename, objectData.filename + ".obj");
			std::unique_ptr<map> enemy = std::make_unique<map>();
			enemy->Init(objectData.transform.scale, {
				objectData.transform.rotate.x * 3.1415f / 180.0f,
				objectData.transform.rotate.y * 3.1415f / 180.0f ,
				objectData.transform.rotate.z * 3.1415f / 180.0f
				}, objectData.transform.translate, objectData.filename);
			maps.push_back(std::move(enemy));
		}
	}
}
