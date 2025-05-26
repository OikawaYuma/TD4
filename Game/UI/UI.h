#pragma once
#include "SpeedMeter/SpeedMeter.h"
#include "Gear/GearUI.h"

class UI {
public:
	~UI();

	// 初期化
	void Initialize();
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

	// Setter
	void SetSpeed(float speed_);
	void SetGear(int gear);

private:
	// スピードメーター
	std::unique_ptr<SpeedMeterUI> speedMeterUI_;

	// ギア
	std::unique_ptr<GearUI> gearUI_;
	
};