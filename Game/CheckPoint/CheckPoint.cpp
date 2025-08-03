#include "CheckPoint.h"

void CheckPoint::Update()
{
}

void CheckPoint::OnCollision()
{
	// 衝突したら何かする
	if (collider_) {
		collider_->SetWorldPosition(GetWorldPosition());
		collider_->SetScale(collisionScale_);
		collider_->SetMatWorld(objectParam_.lock()->worldTransform.matWorld_);
	}
}