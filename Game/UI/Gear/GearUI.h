#pragma once
#include <memory>
#include <Sprite.h>

class GearUI {
public:
	~GearUI();

	void Initialize();
	void Update();
	void Draw();

	// Setter
	void SteGearNum(int gear) { gear_ = gear; }

private:
	// Sprite
	std::unique_ptr<Sprite> gearSprite_;
	std::unique_ptr<Sprite> gearStateSprite_;
	// Gear
	int gear_;
	// 位置
	Vector2 pos_ = { 1050.0f, 170.0f };
	// 大きさ
	Vector2 scale_ = { 250.0f, 250.0f };



	// ギア0位置・大きさ
	Vector2 State0Pos_ = { 1122.0f, 250.0f };
	Vector2 StateScale_ = { 20.0f, 20.0f };

	// ギア1位置
	Vector2 State1Pos_ = { 1122.0f, 305.0f };

	// ギア2位置
	Vector2 State2Pos_ = { 1158.0f, 250.0f };

	// ギア3位置
	Vector2 State3Pos_ = { 1158.0f, 305.0f };

	// ギア4位置
	Vector2 State4Pos_ = { 1192.0f, 249.0f };

	// ギア5位置
	Vector2 State5Pos_ = { 1192.0f, 305.0f };
};