#include "RearCarTire.h"

void RearCarTire::Initialize(const Vector3& rotate, const Vector3& scale, const Vector3& translate, const std::string filename)
{
	scale;
	// texture
	texture_ = TextureManager::GetInstance()->StoreTexture("Resources/car/carTire.png");
	BaseObject::Initialize(rotate, { 1.0f,1.0f,1.0f }, translate, filename); // 共通の初期化
}

void RearCarTire::Update()
{
	BaseObject::Update(); // 共通の更新
	Rotate(); // 回転
}

// RearCarTire.cpp
void RearCarTire::Rotate()
{
	if (!car_) return; // 安全チェック

	float speed = *car_->GetSpeed(); // Carのスピードを取得
	const float kRoolSpeed = 0.01f;  // 係数は調整してください

	if (auto param = objectParam_.lock()) {
		// スピードに応じて回転
		param->worldTransform.rotation_.x += speed * kRoolSpeed;
		param->worldTransform.UpdateMatrix();
	}
}

void RearCarTire::SetParent(const WorldTransform* worldTransform)
{
	if (objectParam_.lock()) {
		objectParam_.lock()->worldTransform.parent_ = worldTransform;
	}
}