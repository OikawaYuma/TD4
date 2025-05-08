#include "FrontCarTire.h"


void FrontCarTire::Initialize(const Vector3& rotate, const Vector3& scale, const Vector3& translate, const std::string filename)
{
	// texture
	texture_ = TextureManager::GetInstance()->StoreTexture("Resources/car/CAR.png");
	BaseObject::Initialize(rotate, { 1.0f,1.0f,1.0f }, translate, filename); // 共通の初期化
}

void FrontCarTire::Update()
{
	BaseObject::Update(); // 共通の更新
	Rotate(); // 回転
}

void FrontCarTire::Rotate()
{
	// 計算が完成したらエンジンから回転もってこようかな
	// とりあえず仮
	float rtValue = Input::GetInstance()->GetRTValue();
	const float kRotateSpeed = 5.0f; // 適当に調整して
	if (auto param = objectParam_.lock()) {
		param->worldTransform.rotation_.x -= rtValue * kRotateSpeed;
	}

	// 曲がるときの左右回転
	float lxValue = Input::GetInstance()->JoyStickParmLX(1.0f);
	const float kYawMaxAngle = 30.0f; // 適当に調整してタイヤの最大角度
	if (auto param = objectParam_.lock()) {
		param->worldTransform.rotation_.y = lxValue * kYawMaxAngle * (std::numbers::pi_v<float> / 180.0f);
		param->worldTransform.UpdateMatrix();
	}
}

void FrontCarTire::SetParent(const WorldTransform* worldTransform)
{
	if (objectParam_.lock()) {
		objectParam_.lock()->worldTransform.parent_ = worldTransform;
	}
}