#include "map.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Model.h"
#include "Object3dManager.h"
#include "Input.h"
map::~map()
{
	SetObjectPram();

}
void map::Init(const Vector3& scale, const Vector3& rotate, const Vector3& translate, const std::string filename)
{
	floorTex_ = TextureManager::GetInstance()->StoreTexture("Resources/load4.png");
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	objectPram_ = Object3dManager::GetInstance()->StoreObject(filename, floorTex_, Transparency::Opaque);
	objectPram_.lock()->worldTransform.scale_ = scale;
	objectPram_.lock()->worldTransform.rotation_ = rotate;
	objectPram_.lock()->worldTransform.translation_ = translate;
	objectPram_.lock()->color = color_;


	material_.color = { 1.0f,1.0f,1.0f,1.0f };
	material_.enableLighting = false;
	material_.uvTransform = MakeIdentity4x4();
	material_.shininess = 60.0f;

}

void map::Update()
{
	if (objectPram_.lock()) {
		objectPram_.lock()->worldTransform.UpdateMatrix();
	}
}



void map::SetObjectPram()
{
	if (auto locked = objectPram_.lock()) {  // weak_ptrからshared_ptrに変換
		locked->isAlive = false;
	}
}
