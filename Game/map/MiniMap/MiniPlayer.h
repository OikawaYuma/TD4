#pragma once
#include <memory>
#include <Sprite.h>
#include <imgui/imgui.h>
#include <Input.h>
#include <numbers>

class MiniPlayer {
public:
	~MiniPlayer();

	void Initialize();
	void Update();
	// 描画処理
	void Draw();

	void SetFromPlayer(const Vector2& playerPos, float playerRot, const Vector2& mapOrigin, float scaleRate);

	// Setter
	void SetSpeed(float* speed_) { speed = speed_; }
	void SetRot(float rot_) { rot = rot_; }

private:
	// Sprite
	std::unique_ptr<Sprite> miniPSprite_;
	// 位置
	Vector2 pos = { 640.0f, 360.0f };
	// 大きさ
	Vector2 scale = { 30.0f, 30.0f };
	// 回転
	float rot;
	// 速さ
	float* speed;
};