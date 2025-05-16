#pragma once
class CarSteering
{
public:
	void Init();
	void Update();

public: // Getter
	float* GetAngle() { return &angle; }

private:
	// ステアリング角（プレイヤー入力から）
	float angle = 0.0f;

};

