#include "CarTire.h"

void CarTire::Initialize(const Vector3& rotate, const Vector3& scale, const Vector3& translate, const std::string filename)
{
	// texture
	texture_ = TextureManager::GetInstance()->StoreTexture("Resources/car/CAR.png");
	// 初期化
	BaseObject::Initialize(rotate, scale, translate, filename);
}

void CarTire::Update()
{
	BaseObject::Update();
}
