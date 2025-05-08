#pragma once
#include "BaseObject/BaseObject.h"

struct RangeKgfm {
	float min;
	float max;
};

struct RangeRpm {
	float min;
	float max;
};

class CarEngine : public BaseObject
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="scale"></param>
	/// <param name="translate"></param>
	/// <param name="filename"></param>
	void Initialize(const Vector3& rotate, const Vector3& scale, const Vector3& translate, const std::string filename)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// トルクの計算
	/// </summary>
	void TorqueCalc();

	/// <summary>
	/// 回転速度の計算
	/// </summary>
	void RotateSpeedCalc();
	

#pragma region setter


#pragma endregion

#pragma region getter


#pragma endregion

private:
	// 現在の回転数
	float rpm_ = 0.0f;
	// rpmの範囲
	RangeRpm rangeRpm_{};
	// kgf・m
	RangeKgfm rangeKgfm_{};
	// トルク
	float torque_ = 0.0f;
	// アクセル
	float ac_ = 0.0f;
	// アクセル最大値
	const float kMaxAc_ = 100.0f;
	// エンジンブレーキトルク
	float ebt_ = 0.0f;
	// 回転速度
	float rotateSpeed_ = 0.0f;
	// いまいち分からん
	float erpm_ = 0.0f;
	// 慣性モーメント
	float inertia_ = 0.0f;
	// deltaTime
	const float kDeltaTime_ = 1.0f / 60.0f;
};