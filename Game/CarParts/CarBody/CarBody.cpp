
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
	penetration_ = {};
	normal_ = {};

	const auto& collisionInfos = collider_->GetCollisionInfo();
	// 衝突情報が空じゃなかったら
	if (!collisionInfos.empty()) {

		Vector3 sumNormal{}; // 法線の合計
		float maxPenetration = 0.0f;  // 押し出し量

		// 当たってる情報の数だけ回す（例えば二面当たってたら二つ）
		for (const auto& info : collisionInfos) {
			// 実際に足す処理
			sumNormal = sumNormal + info.normal;
			if (info.penetration > maxPenetration) {
				maxPenetration = info.penetration;
			}
		}

		// 法線の平均を正規化
		Vector3 avgNormal = Normalize(sumNormal);
		// 押し出したい方向と押し出し量をかける
		penetration_ = avgNormal * maxPenetration;
		// 法線の向きを入れる
		normal_ = avgNormal;
	}

	// colliderに送る
	if (collider_) {
		collider_->SetWorldPosition(GetWorldPosition());
		collider_->SetScale(collisionScale_);
		collider_->SetMatWorld(objectParam_.lock()->worldTransform.matWorld_);
	}

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
