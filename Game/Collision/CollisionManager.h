
/**
* @ file CollisionManager.h
* @ brief Collisionを管理するクラス
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "Collider.h"
#include<list>
#include <memory>
#include <iostream>
#include <algorithm>
#include <limits>
#include"math/mathFunction.h"

class CollisionManager {
public:
	/// <summary>
	/// 衝突判定チェック
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// コライダーのリストに追加
	/// </summary>
	/// <param name="collider"></param>
	void PushCollider(Collider* collider) { colliders_.push_back(collider); };

	/// <summary>
	/// コライダーのリストをクリア
	/// </summary>
	void ColliderClear() { colliders_.clear(); }

private:

	/// <summary>
	/// 衝突フィルタリング
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// 球同士の当たり判定
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v1Radious"></param>
	/// <param name="v2"></param>
	/// <param name="v2Radious"></param>
	/// <returns></returns>
	bool CheckCollision(Vector3 v1, float v1Radious, Vector3 v2, float v2Radious);

	/// <summary>
	/// OBB同士
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="outNormal"></param>
	/// <param name="outPenetration"></param>
	/// <returns></returns>
	bool CheckCollision(OBB a, OBB b, Vector3* outNormal, float* outPenetration);

	/// <summary>
	/// 球とOBBの当たり判定
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="radius"></param>
	/// <param name="obb"></param>
	/// <returns></returns>
	bool CheckCollision(Vector3 v1, float radius, OBB obb);

	/// <summary>
	/// obb生成
	/// </summary>
	/// <param name="collider"></param>
	/// <returns></returns>
	OBB CreateObb(Collider* collider);

private:
	std::list<Collider*> colliders_;
};
