#include "CollisionManager.h"
#include "GameScene.h"
#include "GameScene.h"
//#include "AxisIndicator.h"

void CollisionManager::CheckAllCollision() {

	//  リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;
		colliderA;
		// イテレータBはイテレータ―Aの次の要素から回す（重複判定を回避）
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;
			colliderB;
			// ペアの当たり判定
			CheckCollisionPair(*itrA, *itrB);
		}
	}
}


void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {

	if ((colliderA->GetCollisonAttribute() & colliderB->GetCollisionMask()) ==
		(colliderB->GetCollisonAttribute() & colliderA->GetCollisionMask())) {
		return;
	};

	// 球同士の当たり判定
	if (colliderA->GetCollisionMode() == CollisionMode::Ballc && colliderB->GetCollisionMode() == CollisionMode::Ballc) {
		// 判定対象AとBの座標
		Vector3 posA, posB;
		float radiusA, radiusB;

		// colliderAの座標
		posA = colliderA->GetWorldPosition();
		radiusA = colliderA->GetRadius();

		// colliderBの座標
		posB = colliderB->GetWorldPosition();
		radiusB = colliderB->GetRadius();
		// 弾と弾の考交差判定

		if (CheckCollision(posA, radiusA, posB, radiusB)) {
			// コライダーAの衝突時コールバックを呼び出す
			colliderA->SetOnCollision(true);
			// コライダーBの衝突時コールバックを呼び出す
			colliderB->SetOnCollision(true);
		}
		else {
			// コライダーAの衝突時コールバックを呼び出す
			colliderA->SetOnCollision(false);
			// コライダーBの衝突時コールバックを呼び出す
			colliderB->SetOnCollision(false);
		}
	}


	// 以下制作中
	if (
		(colliderA->GetCollisionMode() == CollisionMode::Ballc && colliderB->GetCollisionMode() == CollisionMode::OBBc) ||
		(colliderA->GetCollisionMode() == CollisionMode::OBBc && colliderB->GetCollisionMode() == CollisionMode::Ballc)
		) {
		// 判定対象AとBの座標
		Vector3 posA, posB;
		float radiusA, radiusB;

		// colliderAの座標
		posA = colliderA->GetWorldPosition();
		radiusA = colliderA->GetRadius();

		// colliderBの座標
		posB = colliderB->GetWorldPosition();
		radiusB = colliderB->GetRadius();
		// 弾と弾の考交差判定
		// 衝突フィルタリング
	}
}

bool CollisionManager::CheckCollision(Vector3 v1, float v1Radious, Vector3 v2, float v2Radious)
{
	float x = (v2.x - v1.x);
	float y = (v2.y - v1.y);
	float z = (v2.z - v1.z);

	float position = (x * x) + (y * y) + (z * z);

	float radious = v1Radious + v2Radious;

	if (position <= (radious * radious)) {
		return true;
	}
	else {
		return false;
	}
};
