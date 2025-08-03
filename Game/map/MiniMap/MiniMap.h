#pragma once
#include "Object3d.h"
#include <Object3dManager.h>
#include "WorldTransform.h"
#include "Camera.h"

class MiniMap{
public:
	~MiniMap();

	void Initialize(const Vector3& scale, const Vector3& rotate, const Vector3& translate, const std::string filename);
	void Update(Camera* camera);

	void SetObjectPram();
private:
	std::weak_ptr<ObjectPram> objectPram_{};
	uint32_t texture_ = 0;
	Vector4 color_;

	Camera* camera_ = nullptr;

	Vector3 offset = {};
};