#pragma once
#include "BaseObject/BaseObject.h"

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

#pragma endregion

protected:
	// ステアリングクラスからポインタを受け取る
	// インターフェースなのに作っちゃったすまん
	float* steeringAngle_ = nullptr;
};