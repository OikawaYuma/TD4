#pragma once
#include "CarParts/CarBody/CarBody.h"
#include "CarParts/CarEngine/CarEngine.h"
#include "CarParts/CarTire/ICarTire.h"
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
	void Update(float uiSpeed);

private:

	/// <summary>
	/// 車体生成
	/// </summary>
	void CreateCarBody();

	/// <summary>
	/// タイヤ生成
	/// </summary>
	void CreateCarTire();

private:
	WorldTransform worldTransform_{};
	std::vector<std::unique_ptr<BaseObject>> parts_;

	// 車体
	std::unique_ptr<CarBody> body_;
	// 車輪
	std::vector<std::unique_ptr<ICarTire>> tires_;

	// 仮
	std::weak_ptr<ObjectPram> objectPram_{};
	uint32_t floorTex_ = 0;
	Vector4 color_;

	Material material_{};
};

