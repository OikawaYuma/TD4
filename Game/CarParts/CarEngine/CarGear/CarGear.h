#pragma once
#include <Input.h>
#include <imgui/imgui.h>
#include <array>
#include <memory>
#include "CarParts/CarEngine/CarGear/IGearState.h"

class IGearState;

class Gear {
public:
	// 初期化
	void Initialize();
	// 更新
	void Update();
	void Draw();

	// ギア状態を切り替える
	void ChangeState(int gearNum);
	// 現在のスピード取得
	float GetCurrentSpeed() const;
	float GetCurrentGear() const;
private:
	// 現在のギア
	int currentGear = 0;
	// 0～5ギア
	std::array<std::unique_ptr<IGearState>, 6> gearStates;
	IGearState* currentState = nullptr;
};
