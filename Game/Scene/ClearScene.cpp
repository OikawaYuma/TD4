#include "ClearScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"
#include "Loder.h"
#include "Audio.h"
#include "Object3dManager.h"
#include "GlobalVariables/GlobalVariables.h"
void ClearScene::Init()
{
	
	// PostEffect初期化
	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera_->GetCamera());
	postProcess_->Init();
	postProcess_->SetDissolveInfo({ 1.0f, 0.984313f, 0.643f });
	postProcess_->SetEffectNo(PostEffectMode::kDissolveOutline); // OutlineとDissolveの使用
}
void ClearScene::Update()
{	
}
void ClearScene::Draw()
{
}

void ClearScene::Draw2d()
{
}

void ClearScene::PostDraw()
{
	postProcess_->Draw();
}

void ClearScene::Release() {

}


// ゲームを終了
int ClearScene::GameClose()
{
	return false;
}

void ClearScene::DeleteObject()
{
}
