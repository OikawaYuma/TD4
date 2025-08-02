#include "SelectScene.h"
#include <filesystem>
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"
#include "Loder.h"
#include "Audio.h"
#include "Object3dManager.h"
#include "GlobalVariables/GlobalVariables.h"
#include "SharedGameData/SharedGameData.h"

void SelectScene::Init()
{
	Object3dManager::GetInstance()->Init();
	camera_ = std::make_unique<ClearCamera>();
	camera_->Init();
	// PostEffect初期化
	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera_->GetCamera());
	postProcess_->Init();
	postProcess_->SetDissolveInfo({ 1.0f, 0.984313f, 0.643f });
	postProcess_->SetEffectNo(PostEffectMode::kDissolveOutline); // OutlineとDissolveの使用

	const std::filesystem::path folderPath = "Resources/json/stage/"; // ← ここを自分のパスに
	for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
		if (entry.is_regular_file() && entry.path().extension() == ".json") {
			maxStageNum_++;
		}
	}
	
	ModelManager::GetInstance()->LoadModel("Resources/TenQ", "TenQ.obj");
	{
		std::weak_ptr<ObjectPram> objectpram = Object3dManager::GetInstance()->StoreObject("TenQ", TextureManager::GetInstance()->StoreTexture("Resources/white.png"), 0);
		if (objectpram.lock()) {
			objectpram.lock()->worldTransform.translation_ = { 0.0f,-1000000.0f,0.0f };
			objectpram.lock()->worldTransform.scale_ = { -100000.0f,100000.0f,100000.0f };
			objectpram.lock()->worldTransform.UpdateMatrix();
			objectpram.lock()->color = { 0.6f,0.6f,0.6f,1.0f };
		}
	}
	carView_ = std::make_unique<CarView>();
	carView_->Init();
	// 選択画面のスプライトの初期化
	selectSprite_ = std::make_unique<Sprite>();
	selectSprite_->Init("Resources/white.png");
	selectSprite_->SetPosition({ 640.0f, 464.0f });
	selectSprite_->SetSize({ 1280.0f, 128.0f });
	selectSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	selectSprite_->SetColor({ 0.0f,0.0f,0.0f,0.7f });

	// 選択ステージのスプライトの初期化
	for (int i = 0; i < maxStageNum_; i++) {
		std::unique_ptr<SelectStage> selectStage_ = std::make_unique<SelectStage>();
		selectStage_->Init(i);
		selectStages_.push_back(std::move(selectStage_));

	}
	
	//fadeの初期化
	fade_ = std::make_unique<Fade>();
	fade_->Init("Resources/Black.png");
	//fade_->SetAlpha(1.0f); // 初期アルファ値を1に設定
	fade_->StartFadeOut();
}
void SelectScene::Update()
{
	carView_->Update();
	// ステージ選択の処理
	StageSelect();
	selectSprite_->Update();
	for(auto& selectStage : selectStages_){
		selectStage->Update(selectedStageNum_);
	}

	// シーン切り替え用のフェード処理
	fade_->UpdateFade();
	fade_->Update();

	if (fade_->GetAlpha() >= 1.0f) {
		sceneNo = STAGE; // ステージシーンに遷移
		SharedGameData::GetInstance()->SetSelectedStageNo(selectedStageNum_); // 選択されたステージ番号を保存
	}
	
}
void SelectScene::Draw()
{
	Object3dManager::GetInstance()->Draw(camera_->GetCamera());
}

void SelectScene::Draw2d()
{
	selectSprite_->Draw();
	for (auto& selectStage : selectStages_) {
		selectStage->Draw();
	}
	fade_->Draw();
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
	if (Input::GetInstance()->GetJoystickState() && fade_->GetAlpha() <= 0.0f) {
		if (Input::GetInstance()->JoyStickParmLX(1.0f) > 0.0f && inputWaitTime_ >= maxInputWaitTime_) {
			inputWaitTime_ = 0.0f; // 入力待機時間をリセット
			selectedStageNum_++; // ステージ番号をインクリメント
			// ステージ番号の上限をチェック
			if (selectedStageNum_ == maxStageNum_) {
				selectedStageNum_--; // ステージ番号が最大値を超えないようにする
			}
		}
		else if (Input::GetInstance()->JoyStickParmLX(1.0f) < 0.0f && inputWaitTime_ >= maxInputWaitTime_) {
			inputWaitTime_ = 0.0f; // 入力待機時間をリセット
			selectedStageNum_--; // ステージ番号をデクリメント
			// ステージ番号の下限をチェック
			if (selectedStageNum_ < 0) {
				selectedStageNum_ = 0; // ステージ番号が負にならないようにする
			}
		}

		
	}
	if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_Y)&& fade_->GetAlpha() <= 0.0f) {
		fade_->StartFadeIn(); // フェードインを開始
	}
}
void SelectScene::DebugDraw()
{
#ifdef _DEBUG
	// デバッグ用の描画処理
	ImGui::Begin("Debug");
	ImGui::Text("Selected Stage: %d", selectedStageNum_);
	ImGui::Text("Input Wait Time: %.2f", inputWaitTime_);
	ImGui::Text("Max Stage Num: %d", maxStageNum_);
	ImGui::End();
#endif // DEBUG_

	
}