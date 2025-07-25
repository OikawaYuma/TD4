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

	// ステージ選択の処理
	StageSelect();
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

void SelectScene::StageSelect()
{
	DebugDraw(); // デバッグ用の描画を呼び出し
	inputWaitTime_ += 0.016f; // フレームごとに入力待機時間を増加
	if (inputWaitTime_ >= maxInputWaitTime_) {
		inputWaitTime_ = maxInputWaitTime_; // 入力待機時間の上限を設定
	}
	if (Input::GetInstance()->GetJoystickState()) {
		if (Input::GetInstance()->JoyStickParmLX(1.0f) > 0.0f && inputWaitTime_ >= maxInputWaitTime_) {
			inputWaitTime_ = 0.0f; // 入力待機時間をリセット
			selectedStageNum_++; // ステージ番号をインクリメント
		}
		else if (Input::GetInstance()->JoyStickParmLX(1.0f) < 0.0f && inputWaitTime_ >= maxInputWaitTime_) {
			inputWaitTime_ = 0.0f; // 入力待機時間をリセット
			selectedStageNum_--; // ステージ番号をデクリメント
		}

		if (Input::GetInstance()->PushJoyButton(XINPUT_GAMEPAD_Y)) {
			sceneNo = STAGE; // ステージシーンに遷移
		}
	}
}
void SelectScene::DebugDraw()
{
#ifdef _DEBUG
	// デバッグ用の描画処理
	ImGui::Begin("Debug");
	ImGui::Text("Selected Stage: %d", selectedStageNum_);
	ImGui::Text("Input Wait Time: %.2f", inputWaitTime_);
	ImGui::End();
#endif // DEBUG_

	
}