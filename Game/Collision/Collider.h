/**
* @ file Collider.h
* @ brief Collisionに関する情報をまとめたクラス
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include"Vector3.h"
#include<cstdint>
#include"math/Matrix4x4.h"

enum class CollisionMode {
	AABBc,
	OBBc,
	Ballc,
	None
};

class Collider {
public:

	bool OnCollision() { return onCollision_; }
	void SetOnCollision(bool onCollision) { onCollision_ = onCollision; }

#pragma region getter

	float GetRadius() { return radius_; }
	const Vector3& GetWorldPosition()const { return worldPosition_; }
	// 衝突属性（自分）を取得
	uint32_t GetCollisonAttribute() { return collisionAttribute_; }
	// 衝突マスク（相手）を取得
	uint32_t GetCollisionMask() { return CollisionMask_; }
	// CollisionModeを取得
	CollisionMode GetCollisionMode() { return collisionMode_; }
	// スケール
	const Vector3& GetScale() { return scale_; }

	const Matrix4x4& GetMatWorld() { return matWorld_; }

#pragma endregion

#pragma region setter

	void SetRadius(float radius) { radius_ = radius; }
	void SetScale(const Vector3& scale) { scale_ = scale; }
	void SetWorldPosition(const Vector3& position) { worldPosition_ = position; }
	// 衝突属性（自分）を設定
	void SetCollisonAttribute(uint32_t collisionAttribute);
	// 衝突マスク（相手）を設定
	void SetCollisionMask(uint32_t CollisionMask);
	void SetCollisionMode(CollisionMode collisionMode) { collisionMode_ = collisionMode; }
	void SetMatWorld(const Matrix4x4& mat) { matWorld_ = mat; }

#pragma endregion

private:
	// 衝突半径
	float radius_ = 2;
	// スケール
	Vector3 scale_{};
	// collisionMode
	CollisionMode collisionMode_ = CollisionMode::None;
	// 衝突属性（自分）
	uint32_t collisionAttribute_ = 0xFFFFFFFF;
	// 衝突マスク（相手）
	uint32_t CollisionMask_ = 0xffffffff;
	// ワールド座標
	Vector3 worldPosition_{};
	// collisionのフラグ
	bool onCollision_ = false;
	// matWorld
	Matrix4x4 matWorld_{};
};
