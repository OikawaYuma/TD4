#pragma once
#include "CarParts/CarBody/CarBody.h"
#include "CarParts/CarEngine/CarEngine.h"
#include "CarParts/CarTire/CarTire.h"
#include <vector>

class Car
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	WorldTransform worldTransform_{};
	std::vector<std::unique_ptr<BaseObject>> parts_;
};

