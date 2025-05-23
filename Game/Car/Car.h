#pragma once
#include "CarParts/CarBody/CarBody.h"
#include "CarParts/CarEngine/CarEngine.h"
#include "CarParts/CarTire/ICarTire.h"
#include "CarParts/CarSteering/CarSteering.h"
#include "CarParts/CarTire/FrontCarTire/FrontCarTire.h"
#include "CarParts/CarTire/RearCarTire/RearCarTire.h"
#include <vector>

class Car
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& scale, const Vector3& rotate, const Vector3& translate, const std::string filename);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public: //Getter
	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	float GetSpeed() { return speed_; }


private: // 生成処理

	/// <summary>
	/// 車体生成
	/// </summary>
	void CreateCarBody();

	/// <summary>
	/// タイヤ生成
	/// </summary>
	void CreateCarTire();

private: // 移動処理

	/// <summary>
	/// ステアリング操作による車体の傾き処理（Y軸回転）
	/// </summary>
	void Yawing();

	/// <summary>
	/// バイシクルモデルでの車の移動および回転処理
	/// </summary>
	void BicycleModel();

private:
	WorldTransform worldTransform_{};
	std::vector<std::unique_ptr<BaseObject>> parts_;

	// 車体
	std::unique_ptr<CarBody> body_;
	// 車輪
	std::vector<std::unique_ptr<ICarTire>> tires_;
	// ステアリング
	std::unique_ptr<CarSteering> steering_;
	// 仮
	std::weak_ptr<ObjectPram> objectPram_{};
	float frontLength = 1.31f;  // 重心から前輪まで
	float rearLength = 1.31f;   // 重心から後輪まで


	uint32_t floorTex_ = 0;
	Vector4 color_;

	Material material_{};

	// 仮のスピード（後々エンジンから出力を受け取りタイヤの回転力とか合わせて考えたい）
	float speed_ = 0.0f;
};

