/**
* @ file GameScene.h
* @ brief 実際のゲーム部分の関するScene
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "IScene.h"
#include "PostProcess.h"
#include "CollisionManager.h"	
#include <vector>
#include <memory>
#include "WorldDesign/WorldDesign.h"
#include "Skydome/Skydome.h"
#include "GameCamera/FollowCamera/FollowCamera.h"
#include "Camera.h"
#include "Score/Score.h"
#include "GameTimer/GameTimer.h"
/*--------------------------------------------
ゲームのスタート演出のための待ちを管理するためのenum
---------------------------------------------*/
enum GameState {
	WAITGAME,
	STARTGAME,
	PLAYGAME,
	CLEARGAME,
	DEADGAME,
	POSEMENUGAME,
	TITLEGAME,
};


class GameScene :public IScene
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

public:

private:
	// ポストエフェクト
	PostProcess* postProcess_ = nullptr;
};

