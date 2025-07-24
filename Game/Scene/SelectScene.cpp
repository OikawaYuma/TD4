#include "SelectScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"
#include "Loder.h"
#include "Audio.h"
#include "Object3dManager.h"
#include "GlobalVariables/GlobalVariables.h"
void SelectScene::Init()
{
	camera_ = std::make_unique<ClearCamera>();
	camera_->Init();
	// PostEffect初期化
	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera_->GetCamera());
	postProcess_->Init();
	postProcess_->SetDissolveInfo({ 1.0f, 0.984313f, 0.643f });
	postProcess_->SetEffectNo(PostEffectMode::kDissolveOutline); // OutlineとDissolveの使用
}
void SelectScene::Update()
{
}
void SelectScene::Draw()
{
}

void SelectScene::Draw2d()
{
}

void SelectScene::PostDraw()
{
	postProcess_->Draw();
}

void SelectScene::Release() {

}


// ゲームを終了
int SelectScene::GameClose()
{
	return false;
}

void SelectScene::DeleteObject()
{
}