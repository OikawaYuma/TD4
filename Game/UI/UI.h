#pragma once
#include "SpeedMeter/SpeedMeter.h"

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

private:
	// スピードメーター
	std::unique_ptr<SpeedMeterUI> speedMeterUI_;
	
};