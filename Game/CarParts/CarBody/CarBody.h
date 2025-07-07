#pragma once
#include "BaseObject/BaseObject.h"

class CarBody : public BaseObject
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

	/// <summary>
	/// スライド
	/// </summary>
	/// <param name="velocity"></param>
	/// <param name="normal"></param>
	/// <returns></returns>
	Vector3 Slide(const Vector3& velocity, const Vector3& normal);

	/// <summary>
	/// 反射
	/// </summary>
	/// <param name="velocity"></param>
	/// <param name="normal"></param>
	Vector3 Reflect(const Vector3& velocity, const Vector3& normal);


#pragma region setter

	void SetParent(WorldTransform* worldTransform);
#pragma endregion

#pragma region getter

	const Vector3& GetPenetration()const { return penetration_; }

	bool GetIsHit()const { return isHit_; }

	const Vector3& GetNormal()const { return normal_; }

#pragma endregion

private:

	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision();

private:

	bool isHit_ = false;
	Vector3 penetration_{}; // めり込み量
	Vector3 normal_{};
};