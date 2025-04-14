#include "GameScene.h"
#include "Loder.h"
#include "ImGuiCommon.h"
#include "Object3dManager.h"
#include "Input.h"
#include "Audio.h"
#include "GlobalVariables/GlobalVariables.h"

void GameScene::Init()
{
	
	// PostEffect初期化
	postProcess_ = new PostProcess();
	postProcess_->Init();
	postProcess_->SetDissolveInfo({1.0f,1.0f,1.0f});
	postProcess_->SetEffectNo(PostEffectMode::kDissolveOutline);
	postProcess_->SerDissolveOutline({ .weightSize = 100.0f });

}

void GameScene::Update()
{

	
}
void GameScene::Draw()
{
	
}

void GameScene::Draw2d()
{
	
}

void GameScene::PostDraw()
{
	
	postProcess_->Draw();
	
}

void GameScene::Release() {
	postProcess_->Release();
	delete postProcess_;
}
// ゲームを終了
int GameScene::GameClose()
{
	return false;
}

void GameScene::DeleteObject()
{
}

