#include "CarEngine.h"

void CarEngine::Initialize(const Vector3& rotate, const Vector3& scale, const Vector3& translate, const std::string filename)
{
	// texture
	texture_ = TextureManager::GetInstance()->StoreTexture("Resources/car/CAR.png");
	// 初期化
	BaseObject::Initialize(rotate, scale, translate, filename);
}

void CarEngine::Update()
{
	BaseObject::Update();

	// アクセル確認用
	if (Input::GetInstance()->PushKey(DIK_G)) {
		ac_ += 1.0f;
	}
	// アクセルの範囲制限
	ac_ = std::clamp(ac_, 1.0f, 100.0f);

	// トルクの計算
	TorqueCalc();
	// 回転速度の計算
	RotateSpeedCalc();
}

void CarEngine::TorqueCalc()
{
	// 回転数が0の時はトルクも0にしておく
	if (rpm_ <= 0.0f) {
		torque_ = 0.0f;
		rpm_ = 0.0f;
	}
	// 0じゃないならトルクの計算をする
	else {
		// トルクの計算
		torque_ = rangeKgfm_.min + (rangeKgfm_.max - rangeKgfm_.min) * (rpm_ - rangeRpm_.min) / (rangeRpm_.max - rangeRpm_.min);
		// アクセルがオフの時の計算
		ebt_ = 0.007f * erpm_;
		// アクセルの踏み具合でトルクを変える
		torque_ = torque_ * (ac_ / 100.0f) - ebt_;
	}

}

void CarEngine::RotateSpeedCalc()
{
	// 回転速度の計算
	rotateSpeed_ += torque_ / inertia_ * kDeltaTime_;
	// 分からん
	erpm_ = rotateSpeed_ * 60.0f / 2.0f / std::numbers::pi_v<float>;
}

