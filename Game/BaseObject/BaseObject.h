
#pragma once
#include "Object3d.h"
#include <list>
#include "Object3dManager.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "PlaneProjectionShadow.h"
#include "Collision/Collider.h"

class BaseObject
{
public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~BaseObject();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="scale"></param>
	/// <param name="translate"></param>
	/// <param name="filename"></param>
	virtual void Initialize(const Vector3& rotate, const Vector3& scale, const Vector3& translate, const std::string filename);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

#pragma region getter

	Collider* GetCollider() { return collider_.get(); }

	Vector3 GetWorldPosition()const;

	const Vector3& GetScale()const;

#pragma endregion

#pragma region setter

	void SetTexture(uint32_t handle) { texture_ = handle; }

	void SetColor(const Vector4& color) { color_ = color; }

#pragma endregion

private:

	/// <summary>
	/// 知らん
	/// </summary>
	void SetObjectParam();

protected:
	std::weak_ptr<ObjectPram> objectParam_{};
	uint32_t texture_ = 0;
	Vector4 color_{ 1.0f,1.0f,1.0f,1.0f };
	std::unique_ptr<Collider> collider_;
	CollisionMode colliderMode_ = CollisionMode::None;
};
