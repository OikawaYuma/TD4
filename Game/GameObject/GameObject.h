#pragma once
#include "Car/Car.h"

class GameObject {
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

	std::vector<std::unique_ptr<BaseObject>> objects_;
};
