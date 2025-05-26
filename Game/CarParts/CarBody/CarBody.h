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

#pragma region setter


#pragma endregion

#pragma region getter


#pragma endregion

private:

};

