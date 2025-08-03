/**
* @ file GameScene.h
* @ brief 実際のゲーム部分の関するScene
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include <memory>
#include "IScene.h"
#include "Triangle.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Particle.h"
#include <list>
#include "Sprite.h"
#include "Object3d.h"
#include "Model.h"
#include "PostProcess.h"
#include "Skybox/Skybox.h"
#include "Skydome/SkyDome.h"
#include "WorldDesign/WorldDesign.h"
#include "GameCamera/FollowCamera/FollowCamera.h"
#include "map/map.h"
#include <UI/UI.h>
#include "Car/Car.h"
#include "Fade/Fade.h"
#include "Fence/Fence.h"
#include "Loder.h"
#include "CarParts/CarEngine/CarGear/CarGear.h"
#include "CollisionType/Box/HitBoxWire.h"	   
#include "Obstacles/Wall.h"
#include "Collision/CollisionManager.h"
#include "PhysicsSystem/PhysicsSystem.h"
#include <map/MiniMap/MiniMap.h>
#include <map/MiniMap/MiniPlayer.h>
#include "Timer/Timer.h"
#include "CheckPoint/CheckPoint.h"

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
	void PostEffectChange();

	void DepthOutlinePramChange();

	void ParticleEmitter();

	void ArrageObj(std::list<std::unique_ptr<map>>& maps);

private:

	void Collision();

private:
	Timer timer_;

	std::unique_ptr<FollowCamera> followCamera_;
	std::unique_ptr<PostProcess> postProcess_;
	// Clearシーン用Camera
	std::unique_ptr<Camera> camera_ = nullptr;
	Vector3 hsv = { 1.0f,1.0f,1.0f };

	// 背景オブジェクト
	std::list<std::unique_ptr<map>> maps_;
	// HitBoxWireのリスト
	std::list<std::unique_ptr<HitBoxWire>> hitBoxWires_;
	// HitBoxWireのリスト
	std::list<std::unique_ptr<Fence>> fences_;
	// Carオブジェクト
	std::unique_ptr<Car> car_;
	// Skyboxオブジェクト
	std::unique_ptr<Skydome> skydome_;
	// 背景オブジェクト
	std::unique_ptr<Particle> particle_;
	// ミニマップ
	std::unique_ptr<MiniMap> minimap_;
	// ミニプレイヤー
	std::unique_ptr<MiniPlayer> miniUI_;
	// ミニプレイヤー
	std::unique_ptr<CheckPoint> checkPoint_;

	Emitter emitter_{};
	RandRangePro randRangePro_{};
	std::unique_ptr<Fade> fade_;
	// Sprite
	std::unique_ptr<Sprite> sprite_;
	std::unique_ptr<UI> ui_;
	uint32_t spTx_ = 0;
	LevelData levelData_{};

	std::vector<std::unique_ptr<HitBoxWire>> boxWires_;

	WorldTransform worldTransform_{};
	DepthOutlineInfo depthOutlineInfo_{};
	// Gear
	std::unique_ptr<Gear> carGear_;

	int selectedIndex[1] = { 0 };

	// collisionManager
	std::unique_ptr<CollisionManager> collisionManager_;
	// 自然法則とかの計算クラス
	std::unique_ptr<PhysicsSystem> physicsSystem_;

	// 画面上のミニマップ左上位置
	Vector2 miniMapOrigin = { 45.0f, 207.0f };
};

