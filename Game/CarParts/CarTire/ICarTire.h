#pragma once
#include "BaseObject/BaseObject.h"

class Car;

class ICarTire : public BaseObject{
public:
	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~ICarTire() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(const Vector3& rotate, const Vector3& scale, const Vector3& translate, const std::string filename) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 回転
	/// </summary>
	virtual void Rotate() = 0;

#pragma region setter

	virtual void SetParent(const WorldTransform* worldTransform) = 0;

	void SetSteeringAngle(float* angle) { steeringAngle_ = angle; }

	void SetCar(Car* car) { car_ = car; }
#pragma endregion

protected:
	// ステアリングクラスからポインタを受け取る
	// インターフェースなのに作っちゃったすまん
	float* steeringAngle_ = nullptr;

	// タイヤの摩耗度　0.0が新品、1.0で完全消耗
	float wearRate_ = 0.0f;

	Car* car_ = nullptr;
};