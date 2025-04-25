#include "BaseObject.h"

BaseObject::~BaseObject()
{
	SetObjectParam();
}

void BaseObject::Initialize(const Vector3& rotate, const Vector3& scale, const Vector3& translate, const std::string filename)
{
	objectParam_ = Object3dManager::GetInstance()->StoreObject(filename, texture_, Transparency::Opaque);
	if (auto obj = objectParam_.lock()) {
		obj->worldTransform.scale_ = scale;
		obj->worldTransform.translation_ = translate;
		obj->worldTransform.rotation_ = rotate;
		obj->color = color_;
	}

	// 衝突判定取るモードなら作る
	if (colliderMode_ != CollisionMode::None) {
		collider_ = std::make_unique<Collider>();
	}
}

void BaseObject::Update()
{
	if (objectParam_.lock()) {
		objectParam_.lock()->worldTransform.UpdateMatrix();
	}

	// colliderに送る
	if (collider_) {
		collider_->SetWorldPosition(GetWorldPosition());
		collider_->SetScale(GetScale());
	}
}

Vector3 BaseObject::GetWorldPosition()const
{
	Vector3 worldPos{};
	if (auto obj = objectParam_.lock()) {
		worldPos.x = obj->worldTransform.matWorld_.m[3][0];
		worldPos.y = obj->worldTransform.matWorld_.m[3][1];
		worldPos.z = obj->worldTransform.matWorld_.m[3][2];
	}
	return worldPos;
}

const Vector3& BaseObject::GetScale() const
{
	return objectParam_.lock()->worldTransform.scale_;
}

void BaseObject::SetObjectParam()
{
	if (auto locked = objectParam_.lock()) {  // weak_ptrからshared_ptrに変換
		locked->isAlive = false;
	}
}
