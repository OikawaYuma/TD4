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

	// OBB同士の当たり判定
	if (colliderA->GetCollisionMode() == CollisionMode::OBBc && colliderB->GetCollisionMode() == CollisionMode::OBBc) {
		OBB obbA{};
		obbA.center = colliderA->GetWorldPosition();
		obbA.halfSize.x = colliderA->GetScale().x / 2.0f;
		obbA.halfSize.y = colliderA->GetScale().y / 2.0f;
		obbA.halfSize.z = colliderA->GetScale().z / 2.0f;
		Matrix4x4 matWorldA{};
		matWorldA = colliderA->GetMatWorld();

		// 各ローカル軸を正規化して格納（行ベース）
		obbA.axis[0] = Normalize(Vector3(matWorldA.m[0][0], matWorldA.m[0][1], matWorldA.m[0][2])); // X軸
		obbA.axis[1] = Normalize(Vector3(matWorldA.m[1][0], matWorldA.m[1][1], matWorldA.m[1][2])); // Y軸
		obbA.axis[2] = Normalize(Vector3(matWorldA.m[2][0], matWorldA.m[2][1], matWorldA.m[2][2])); // Z軸

		OBB obbB{};
		obbB.center = colliderB->GetWorldPosition();
		obbB.halfSize.x = colliderB->GetScale().x / 2.0f;
		obbB.halfSize.y = colliderB->GetScale().y / 2.0f;
		obbB.halfSize.z = colliderB->GetScale().z / 2.0f;
		Matrix4x4 matWorldB{};
		matWorldB = colliderB->GetMatWorld();

		// 各ローカル軸を正規化して格納（行ベース）
		obbB.axis[0] = Normalize(Vector3(matWorldB.m[0][0], matWorldB.m[0][1], matWorldB.m[0][2])); // X軸
		obbB.axis[1] = Normalize(Vector3(matWorldB.m[1][0], matWorldB.m[1][1], matWorldB.m[1][2])); // Y軸
		obbB.axis[2] = Normalize(Vector3(matWorldB.m[2][0], matWorldB.m[2][1], matWorldB.m[2][2])); // Z軸

		if (CheckCollision(obbA, obbB)) {
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

	// 球とOBBの当たり判定
	if (colliderA->GetCollisionMode() == CollisionMode::Ballc && colliderB->GetCollisionMode() == CollisionMode::OBBc) {
		OBB obb{};
		obb.center = colliderB->GetWorldPosition();
		obb.halfSize.x = colliderB->GetScale().x / 2.0f;
		obb.halfSize.y = colliderB->GetScale().y / 2.0f;
		obb.halfSize.z = colliderB->GetScale().z / 2.0f;
		Matrix4x4 matWorld{};
		matWorld = colliderB->GetMatWorld();

		// 各ローカル軸を正規化して格納（行ベース）
		obb.axis[0] = Normalize(Vector3(matWorld.m[0][0], matWorld.m[0][1], matWorld.m[0][2])); // X軸
		obb.axis[1] = Normalize(Vector3(matWorld.m[1][0], matWorld.m[1][1], matWorld.m[1][2])); // Y軸
		obb.axis[2] = Normalize(Vector3(matWorld.m[2][0], matWorld.m[2][1], matWorld.m[2][2])); // Z軸


		// colliderBの座標
		Vector3 center{};
		float radius{};
		center = colliderA->GetWorldPosition();
		radius = colliderA->GetRadius();

		// 弾と弾の考交差判定
		if (CheckCollision(center, radius,obb)) {
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

	// 球とOBBの当たり判定
	if (colliderA->GetCollisionMode() == CollisionMode::OBBc && colliderB->GetCollisionMode() == CollisionMode::Ballc) {
		OBB obb{};
		obb.center = colliderA->GetWorldPosition();
		obb.halfSize.x = colliderA->GetScale().x / 2.0f;
		obb.halfSize.y = colliderA->GetScale().y / 2.0f;
		obb.halfSize.z = colliderA->GetScale().z / 2.0f;
		Matrix4x4 matWorld{};
		matWorld = colliderA->GetMatWorld();

		// 各ローカル軸を正規化して格納（行ベース）
		obb.axis[0] = Normalize(Vector3(matWorld.m[0][0], matWorld.m[0][1], matWorld.m[0][2])); // X軸
		obb.axis[1] = Normalize(Vector3(matWorld.m[1][0], matWorld.m[1][1], matWorld.m[1][2])); // Y軸
		obb.axis[2] = Normalize(Vector3(matWorld.m[2][0], matWorld.m[2][1], matWorld.m[2][2])); // Z軸


		// colliderBの座標
		Vector3 center{};
		float radius{};
		center = colliderB->GetWorldPosition();
		radius = colliderB->GetRadius();

		// 弾と弾の考交差判定
		if (CheckCollision(center, radius, obb)) {
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
}

bool CollisionManager::CheckCollision(const OBB& a, const OBB& b)
{
	const float ep = std::numeric_limits<float>::epsilon();
	// 各軸：aの3軸 + bの3軸 + a×bの交差軸 = 15本
	Vector3 axis[15];

	// aのローカル軸
	for (int i = 0; i < 3; i++) {
		axis[i] = a.axis[i];
	}

	// bのローカル軸
	for (int i = 0; i < 3; i++) {
		axis[i + 3] = b.axis[i];
	}

	// a×b の交差軸
	int index = 6;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			axis[index++] = Normalize(Cross(a.axis[i], b.axis[j]));
		}
	}

	// 中心間ベクトル
	Vector3 t;
	t.x = b.center.x - a.center.x;
	t.y = b.center.y - a.center.y;
	t.z = b.center.z - a.center.z;

	// 全軸で投影をチェック
	for (int i = 0; i < 15; i++) {
		const Vector3& L = axis[i];
		if (Length(L) < ep) continue; // 無効軸スキップ

		// aの投影幅
		float ra =
			std::abs(Dot(a.axis[0], L)) * a.halfSize.x +
			std::abs(Dot(a.axis[1], L)) * a.halfSize.y +
			std::abs(Dot(a.axis[2], L)) * a.halfSize.z;

		// bの投影幅
		float rb =
			std::abs(Dot(b.axis[0], L)) * b.halfSize.x +
			std::abs(Dot(b.axis[1], L)) * b.halfSize.y +
			std::abs(Dot(b.axis[2], L)) * b.halfSize.z;

		// 中心差の投影
		float dist = std::abs(Dot(t, L));

		if (dist > ra + rb) {
			// この軸で分離がある → 衝突してない
			return false;
		}
	}

	// 全軸で分離がなかった → 衝突している
	return true;
}

bool CollisionManager::CheckCollision(const Vector3& v1, float radius, const OBB& obb)
{
	// 最近接点を計算
	Vector3 d = v1 - obb.center;
	Vector3 closest = obb.center;

	// X軸
	{
		float dist = Dot(d, obb.axis[0]);
		float clamped = std::clamp(dist, -obb.halfSize.x, obb.halfSize.x);
		closest = closest + obb.axis[0] * clamped;
	}
	// Y軸
	{
		float dist = Dot(d, obb.axis[1]);
		float clamped = std::clamp(dist, -obb.halfSize.y, obb.halfSize.y);
		closest = closest + obb.axis[1] * clamped;
	}
	// Z軸
	{
		float dist = Dot(d, obb.axis[2]);
		float clamped = std::clamp(dist, -obb.halfSize.z, obb.halfSize.z);
		closest = closest + obb.axis[2] * clamped;
	}

	// 球の中心と最近接点との距離の2乗を計算
	Vector3 delta = v1 - closest;
	float distSq = Length(delta);

	return distSq <= radius * radius;
}

