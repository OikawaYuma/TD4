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

	collider_->SetCollisonAttribute(kCollisionAttributePlayer);
	collider_->SetCollisionMask(kCollisionAttributeEnemy);
}

void CarBody::Update()
{
	
	// 押し出し
	const auto& collisionInfo = collider_->GetCollisionInfo();
	for (const auto& info : collisionInfo) {
		 penetration_ = info.normal * info.penetration;
	}

	// 更新
	BaseObject::Update();

	// colliderに送る
	if (collider_) {
		collider_->SetWorldPosition(GetWorldPosition());
		collider_->SetScale(GetScale());
		collider_->SetMatWorld(objectParam_.lock()->worldTransform.matWorld_);
	}

	OnCollision();

#ifdef _DEBUG
	ImGui::Begin("hit");
	ImGui::Text("hit : %d", isHit_);
	ImGui::End();
#endif // _DEBUG

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
