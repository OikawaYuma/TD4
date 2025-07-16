/**
* @ file Fence.h
* @ brief 背景Objectの設定
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "Object3d.h"
#include <list>
#include "Object3d.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "PlaneProjectionShadow.h"
#include "BaseObject/BaseObject.h"

class Fence : public BaseObject
{
public:
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="scale"></param>
	/// <param name="translate"></param>
	/// <param name="filename"></param>
	void Initialize(const Vector3& rotate, const Vector3& scale, const Vector3& translate, const std::string filename)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

private: // Collision
	/// <summary>
	/// 衝突
	/// </summary>
	void OnCollision();

public: // Getter
	void SetCollisionScale(const Vector3& scale) { collisionScale_ = scale; }// 衝突スケールの設定


private:
	Vector3 collisionScale_ = { 1,1,1 };
	// ヒットフラグ
	bool isHit_ = false;
};


