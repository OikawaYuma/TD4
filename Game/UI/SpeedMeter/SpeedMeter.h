#pragma once
#include <memory>
#include <Sprite.h>
#include <imgui/imgui.h>
#include <Input.h>
#include <numbers>

class SpeedMeterUI {
public:
	~SpeedMeterUI();

	// 初期化
	void Initialize();
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

	// Setter
	void SetSpeed(float* speed_) { speed = speed_; }

private:
	// Sprite
	std::unique_ptr<Sprite> SpeedSprite_;
	std::unique_ptr<Sprite> SpeedMeterSprite_;

	// 最高速
	const float maxSpeed = 245.0f;
	// 最低速
	const float minSpeed = 0.0f;
	// 速さ
	float* speed;
	// 加速度
	float acceleration = 1.5f;
	// 減速度
	float deceleration = 1.0f;
	// 位置
	Vector2 pos_ = { 970.0f, 410.0f };
	// 大きさ
	Vector2 scale_ = { 300.0f, 300.0f };
	// 位置
	Vector2 hariPos_ = { 1123.0f, 560.0f };
	// 大きさ
	Vector2 hariScale_ = { 85.0f, 150.0f };
	// 回転
	Vector3 hariRotate_ = { 0.0f,0.0f,-132.0f };
	// 調整用変数
	Vector2 hariAnchor_ = { 0.5f, 0.933f };

	// メーターの針の回転計算
	float speedRatio;
	float minAngle = -132.0f;
	float maxAngle = 132.0f;
};