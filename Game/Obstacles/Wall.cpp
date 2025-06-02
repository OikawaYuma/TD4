#include "Wall.h"

void Wall::Initialize(const Vector3& rotate, const Vector3& scale, const Vector3& translate, const std::string filename)
{
	// texture
	texture_ = TextureManager::GetInstance()->StoreTexture("Resources/car/CAR.png");
	// collider
	colliderMode_ = CollisionMode::OBBc;
	// 初期化
	BaseObject::Initialize(rotate, scale, translate, filename);

	collider_->SetCollisonAttribute(0xffffffff);
	collider_->SetCollisionMask(0xFFFFFFFF);
}

void Wall::Update()
{
	BaseObject::Update();
}
