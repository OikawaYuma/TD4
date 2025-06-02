#include "CarBody.h"

void CarBody::Initialize(const Vector3& rotate, const Vector3& scale, const Vector3& translate, const std::string filename)
{
	rotate;
	scale;
	translate;
	filename;
	// texture
	texture_ = TextureManager::GetInstance()->StoreTexture("Resources/car/car.png");
	// collider
	colliderMode_ = CollisionMode::OBBc;
	// 初期化
	BaseObject::Initialize(
		{0.0f,0.0f,0.0f},
		{1.0f,1.0f,1.0f},
		{0.0f,0.21f,0.0f},
		"carBody");

	collider_->SetCollisonAttribute(0xFFFFFFFF);
	collider_->SetCollisionMask(0xffffffff);
}

void CarBody::Update()
{
	
	// 押し出し
	const auto& collisionInfo = collider_->GetCollisionInfo();
	for (const auto& info : collisionInfo) {
		objectParam_.lock()->worldTransform.translation_ = objectParam_.lock()->worldTransform.translation_ + (info.normal * info.penetration);
	}

	// 更新
	BaseObject::Update();

	OnCollision();

	if (isHit_) {
		objectParam_.lock()->color.w = 0.0f;
	}

}

void CarBody::SetParent(WorldTransform* worldTransform)
{
	if (objectParam_.lock()) {
		objectParam_.lock()->worldTransform.parent_ = worldTransform;
	}
}

void CarBody::OnCollision()
{
	isHit_ = collider_->OnCollision();
}

Vector3 CarBody::Reflect(const Vector3& velocity, const Vector3& normal)
{
	Vector3 result{};
	result = velocity - 2.0f * Dot(velocity, normal) * normal;
	return result;
}

Vector3 CarBody::Slide(const Vector3& velocity, const Vector3& normal)
{
	Vector3 result{};
	result = velocity - Dot(velocity, normal) * normal;
	return result;
}
