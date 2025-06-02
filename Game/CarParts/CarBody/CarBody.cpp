#include "CarBody.h"

void CarBody::Initialize(const Vector3& rotate, const Vector3& scale, const Vector3& translate, const std::string filename)
{
	rotate;
	scale;
	translate;
	filename;
	// texture
	texture_ = TextureManager::GetInstance()->StoreTexture("Resources/car/car.png");
	// 初期化
	BaseObject::Initialize(
		{0.0f,0.0f,0.0f},
		{1.0f,1.0f,1.0f},
		{0.0f,0.21f,0.0f},
		"carBody");
}

void CarBody::Update()
{
	// 更新
	BaseObject::Update();
}

void CarBody::SetParent(WorldTransform* worldTransform)
{
	if (objectParam_.lock()) {
		objectParam_.lock()->worldTransform.parent_ = worldTransform;
	}
}
