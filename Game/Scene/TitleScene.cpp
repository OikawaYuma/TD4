#include "TitleScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"
#include "Loder.h"
#include "Audio.h"
#include "Object3dManager.h"
void TitleScene::Init()
{
	postProcess_ = new PostProcess();
	postProcess_->SetDissolveInfo({ 1.0f,1.0f,1.0f });
	postProcess_->Init();
}

void TitleScene::Update()
{
	Object3dManager::GetInstance()->Update();
	postProcess_->Update();
	
}
void TitleScene::Draw()
{

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
