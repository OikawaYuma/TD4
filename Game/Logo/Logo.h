#pragma once
#include "Object3d.h"
#include <list>
#include "Object3d.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "PlaneProjectionShadow.h"
#include <Object3dManager.h>

class Logo
{
public:
	~Logo();
	void Init(const Vector3& scale, const Vector3& rotate, const Vector3& translate, const std::string filename);
	void Update();
public: // Getter


	void SetSpotLight(const SpotLight& spotLight) { spotLight_ = spotLight; }

	void SetObjectPram();
private:
	std::weak_ptr<ObjectPram> objectPram_{};
	uint32_t floorTex_ = 0;
	Camera* camera_ = nullptr;

	Vector4 color_;

	Material material_{};
	DirectionalLight direLight_{};
	SpotLight spotLight_{};
	// 平行影
	std::unique_ptr<PlaneProjectionShadow> shadow_;
};
