/**
* @ file SelectScene.h
* @ brief ゲームのクリア部分に関するScene
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/


#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "PostProcess.h"
#include "Camera.h"
#include "GameCamera/GameOverCamera/GameOverCamera.h"
#include "GameCamera/ClearCamera/ClearCamera.h"
#include "Skydome/Skydome.h"
#include "WorldDesign/WorldDesign.h"
#include "Score/Score.h"

class SelectScene : public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void Draw2d()override;
	void PostDraw()override;
	void Release()override;
	int GameClose()override;

	void DeleteObject();

private:
	// ステージ選択の処理
	void StageSelect();

	// Debug用の関数
	void DebugDraw();

private: // Object
	// Clearシーン用Camera
	std::unique_ptr<ClearCamera> camera_ = nullptr;
	// 天球
	std::unique_ptr<Skydome> skydome_;

private: // 選択用の情報
	int selectedStageNum_ = 0; // 選択されたステージ番号
	// ステージの最大数
	uint32_t maxStageNum_ = 0; // 最大ステージ数
	
	// padの入力待機時間
	float inputWaitTime_ = 0.0f; // 入力待機時間
	// padの入力待機時間の最大値
	const float maxInputWaitTime_ = 0.5f; // 入力待機時間の最大値


private:
	// ポストエフェクト
	PostProcess* postProcess_ = nullptr;
};
