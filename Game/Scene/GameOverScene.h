/**
* @ file GameOverScene.h
* @ brief ゲームオーバー部分の関するScene
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "PostProcess.h"
#include "Camera.h"
#include "GameCamera/GameOverCamera/GameOverCamera.h"
#include "Skydome/Skydome.h"
#include "WorldDesign/WorldDesign.h"

enum class BehaviorGameOver {
	kStart,
	kMain,
	kEnd
};

class GameOverScene : public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void PostDraw()override;
	void Draw2d()override;
	void Release()override;
	int GameClose()override;

	void DeleteObject();

private: // Object
private:
	// ポストエフェクト
	PostProcess* postProcess_ = nullptr;
};

