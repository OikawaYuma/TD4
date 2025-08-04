#pragma once
#include "BaseObject/BaseObject.h"
#include "Collider.h"
class CheckPoint :public BaseObject
{
public:
		/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& scale, const Vector3& rotate, const Vector3& translate, const std::string filename);
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision();

	bool IsGameClear() const { return isGameClear_; }
private:
	Vector3 collisionScale_{ 1.0f, 1.0f, 1.0f }; // 衝突判定のスケール

	bool isGameClear_ = false; // ゲームクリアフラグ
};

