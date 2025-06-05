#include "TitleScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"
#include "Loder.h"
#include "Audio.h"
#include "Object3dManager.h"
#include <ModelManager.h>

void TitleScene::Init()
{
	postProcess_ = new PostProcess();
	postProcess_->SetDissolveInfo({ 1.0f,1.0f,1.0f });
	postProcess_->Init();

	ModelManager::GetInstance()->LoadModel("Resources/car", "car.obj");
	car_ = std::make_unique<Car>();
	car_->Initialize({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, "car");

	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
}

void TitleScene::Update()
{
	Object3dManager::GetInstance()->Update();
	postProcess_->Update();

#ifdef _DEBUG
	camera_->CameraDebug();
#endif // _DEBUG

	camera_->Move();
	camera_->Update();
	
	car_->Update();
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
