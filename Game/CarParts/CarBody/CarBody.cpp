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
		{ 0.0f,0.0f,0.0f },
		{ 1.0f,1.0f,1.0f },
		{ 0.0f,0.21f,0.0f },
		"carBody");

	collider_->SetCollisonAttribute(kCollisionAttributePlayer);
	collider_->SetCollisionMask(kCollisionAttributeEnemy);
}

void CarBody::Update()
{
	// 初期化
	penetration_ = {};
	normal_ = {};

	const auto& collisionInfos = collider_->GetCollisionInfo();
	// 衝突情報が空じゃなかったら
	if (!collisionInfos.empty()) {

		float maxTEnter = 0.0f;
		Vector3 pushBackNormal = {};
		float penetration = 0.0f;

		for (const auto& info : collisionInfos) {
			if (info.time > maxTEnter) {
				maxTEnter = info.time;
				pushBackNormal = info.normal;
				penetration = info.penetration;
			}
		}

		collisionTime_ = maxTEnter;
		normal_ = Normalize(pushBackNormal);
		penetration_ = penetration;
	}

	// colliderに送る
	if (collider_) {
		collider_->SetWorldPosition(GetWorldPosition());
		collider_->SetPrevWorldPosition(prevPosition_);
		collider_->SetScale(collisionScale_);
		collider_->SetMatWorld(objectParam_.lock()->worldTransform.matWorld_);
	}

	// 前フレームの座標記録しておく
	prevPosition_ = GetWorldPosition();

	BaseObject::Update();
	OnCollision();

#ifdef _DEBUG
	ImGui::Begin("hit");
	ImGui::Text("hit : %d", isHit_);
	ImGui::End();
#endif
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
	Vector3 n = Normalize(normal);
	result = velocity - Dot(velocity, n) * n;
	return result;
}
