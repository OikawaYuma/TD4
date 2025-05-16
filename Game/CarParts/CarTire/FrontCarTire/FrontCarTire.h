#pragma once
#include "CarParts/CarTire/ICarTire.h"

class FrontCarTire : public ICarTire 
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& rotate, const Vector3& scale, const Vector3& translate, const std::string filename)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate()override;

#pragma region setter

	void SetParent(const WorldTransform* worldTransform)override;



#pragma endregion

#pragma region getter


#pragma endregion

private:
	


};

