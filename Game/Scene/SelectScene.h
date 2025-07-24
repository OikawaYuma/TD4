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

private: // Object
	// Clearシーン用Camera
	std::unique_ptr<ClearCamera> camera_ = nullptr;
	// 木
	std::list<std::unique_ptr<WorldDesign>> worldDesigns_;
	// 天球
	std::unique_ptr<Skydome> skydome_;

private:
	// ポストエフェクト
	PostProcess* postProcess_ = nullptr;
};
