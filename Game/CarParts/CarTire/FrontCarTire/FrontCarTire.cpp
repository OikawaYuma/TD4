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
	
}

void FrontCarTire::SetParent(const WorldTransform* worldTransform)
{
	if (objectParam_.lock()) {
		objectParam_.lock()->worldTransform.parent_ = worldTransform;
	}
}