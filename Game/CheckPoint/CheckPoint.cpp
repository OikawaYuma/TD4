#include "CheckPoint.h"

void CheckPoint::Initialize(const Vector3& scale, const Vector3& rotate, const Vector3& translate, const std::string filename)
{
	// collider
	colliderMode_ = CollisionMode::OBBc;
	// モデルの初期化
	BaseObject::Initialize(scale, rotate, translate, filename);
	if (objectParam_.lock()) {
		objectParam_.lock()->worldTransform.UpdateMatrix();
		objectParam_.lock()->color = { 1.0f, 1.0f, 1.0f, 0.4f }; // 色を白に設定
	}
	// コライダーの初期化
	collider_->SetWorldPosition(GetWorldPosition());
	collider_->SetScale(collisionScale_);
	collider_->SetMatWorld(objectParam_.lock()->worldTransform.matWorld_);
	collider_->SetCollisonAttribute(kCollisionAttributeEnemy);
	collider_->SetCollisionMask(kCollisionAttributePlayer);
}

void CheckPoint::Update()
{
	BaseObject::Update();
}

void CheckPoint::OnCollision()
{
	// 衝突したら何かする
	if (collider_) {
		isGameClear_ = true;
	}
}