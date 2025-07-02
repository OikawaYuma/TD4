
#include "Wall.h"

void Wall::Initialize(const Vector3& rotate, const Vector3& scale, const Vector3& translate, const std::string filename)
{
	// texture
	texture_ = TextureManager::GetInstance()->StoreTexture("Resources/car/CAR.png");
	// collider
	colliderMode_ = CollisionMode::OBBc;
	// 初期化
	BaseObject::Initialize(rotate, scale, translate, filename);

	collider_->SetCollisonAttribute(kCollisionAttributeEnemy);
	collider_->SetCollisionMask(kCollisionAttributePlayer);
}

void Wall::Update()
{
	BaseObject::Update();

	// colliderに送る
	if (collider_) {
		collider_->SetWorldPosition(GetWorldPosition());
		collider_->SetScale(GetScale());
		collider_->SetMatWorld(objectParam_.lock()->worldTransform.matWorld_);
	}

	OnCollision();

	if (isHit_) {
		objectParam_.lock()->color.w = 0.0f;
	}
}

void Wall::OnCollision()
{
	isHit_ = collider_->OnCollision();
}
