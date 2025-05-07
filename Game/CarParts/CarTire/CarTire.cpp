#include "CarTire.h"

void CarTire::Initialize(const Vector3& rotate, const Vector3& scale, const Vector3& translate, const std::string filename)
{
	rotate;
	scale;
	filename;
	// texture
	texture_ = TextureManager::GetInstance()->StoreTexture("Resources/car/CAR.png");
	// 初期化
	BaseObject::Initialize(
		rotate,
		{ 1.0f,1.0f,1.0f },
		translate,
		"carTire");
}

void CarTire::Update()
{
	BaseObject::Update();
}

void CarTire::SetParent(WorldTransform* worldTransform)
{
	if (objectParam_.lock()) {
		objectParam_.lock()->worldTransform.parent_ = worldTransform;
	}
}