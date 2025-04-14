#include "GameOverScene.h"

#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"
#include <Loder.h>
#include <Audio.h>
#include "Object3dManager.h"
void GameOverScene::Init()
{
	// PostEffect初期化
	postProcess_ = new PostProcess();
	postProcess_->Init();
	postProcess_->SetDissolveInfo({ 0.35f, 0.025f, 0.025f });
	postProcess_->SetEffectNo(PostEffectMode::kDissolveOutline); // OutlineとDissolveの使用
	
	// 透明、不透明の条件でソートを掛ける
	Object3dManager::ObjectSort();
}
void GameOverScene::Update()
{
	postProcess_->Update();

}
void GameOverScene::Draw()
{
}

void GameOverScene::Draw2d()
{

}

void GameOverScene::PostDraw()
{
	postProcess_->Draw();
}

void GameOverScene::Release() {

}


// ゲームを終了
int GameOverScene::GameClose()
{
	return false;
}

void GameOverScene::DeleteObject()
{
}
