#include "Car.h"

void Car::Initialize(const Vector3& scale, const Vector3& rotate, const Vector3& translate, const std::string filename)
{
	floorTex_ = TextureManager::GetInstance()->StoreTexture("Resources/car/car.png");
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	objectPram_ = Object3dManager::GetInstance()->StoreObject(filename, floorTex_, Transparency::Opaque);
	objectPram_.lock()->worldTransform.scale_ = scale;
	objectPram_.lock()->worldTransform.rotation_ = rotate;
	objectPram_.lock()->worldTransform.translation_ = translate;
	objectPram_.lock()->color = color_;


	material_.color = { 1.0f,1.0f,1.0f,1.0f };
	material_.enableLighting = true;
	material_.uvTransform = MakeIdentity4x4();
	material_.shininess = 60.0f;
}

void Car::Update()
{
	if (objectPram_.lock()) {
		if (Input::GetInstance()->PushKey(DIK_I)) {
			objectPram_.lock()->worldTransform.translation_.z++;
		};
		objectPram_.lock()->worldTransform.UpdateMatrix();
	}
}
