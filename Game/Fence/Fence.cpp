#include "Fence.h"

#include "TextureManager.h"
#include "ModelManager.h"
#include "Model.h"
#include "Object3dManager.h"
#include "Input.h"

void Fence::Initialize(const Vector3& rotate, const Vector3& scale, const Vector3& translate, const std::string filename)
{
	// texture
	texture_ = TextureManager::GetInstance()->StoreTexture("Resources/white.png");
	// collider
	colliderMode_ = CollisionMode::OBBc;
	// 初期化
	BaseObject::Initialize(rotate, scale, translate, filename);
	collider_->SetCollisonAttribute(kCollisionAttributeEnemy);
	collider_->SetCollisionMask(kCollisionAttributePlayer);
}

void Fence::Update()
{
	

	// colliderに送る
	if (collider_) {
		collider_->SetWorldPosition(GetWorldPosition());
		collider_->SetScale(GetScale());
		collider_->SetMatWorld(objectParam_.lock()->worldTransform.matWorld_);
	}
	BaseObject::Update();
	OnCollision();

	/*if (isHit_) {
		objectParam_.lock()->color.w = 0.0f;
	}*/
}

void Fence::OnCollision()
{
	isHit_ = collider_->OnCollision();
}