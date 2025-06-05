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
	ModelManager::GetInstance()->LoadModel("Resources/carBody", "carBody.obj");
	car_ = std::make_unique<Car>();
	car_->Initialize({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, "carBody");

	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetPos({ -12.0f, 3.0f, 17.0f });
	camera_->SetRotate({ 0.0f, 2.3f, 0.0f });

	ModelManager::GetInstance()->LoadModel("Resources/ball", "ball.obj");
	GlobalVariables::GetInstance()->LoadFiles();
	carSmoke_ = std::make_unique<CarSmoke>();
	carSmoke_->SetCamera(camera_.get());
	carSmoke_->Init();
	carSmoke_->SetParent(car_->GetWorldTransform());

	ModelManager::GetInstance()->LoadModel("Resources", "logo.obj");
	logo_ = std::make_unique<Logo>();
	logo_->Init({ 5.0f, 5.0f, 1.0f }, { 0.0f, -1.5f, 0.0f }, { 12.0f, 1.0f, 3.5f }, "logo");

	postProcess_ = std::make_unique<PostProcess>();
	postProcess_->Init();
	postProcess_->SetCamera(camera_.get());
	postProcess_->SetEffectNo(PostEffectMode::kDepthOutline);
}

void TitleScene::Update()
{
	Object3dManager::GetInstance()->Update();
	postProcess_->Update();

#ifdef _DEBUG
	//camera_->CameraDebug();
#endif // _DEBUG

	//camera_->Move();
	camera_->Update();
	car_->Update();
	logo_->Update();
	carSmoke_->Update();
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
