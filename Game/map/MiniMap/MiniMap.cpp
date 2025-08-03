#include "MiniMap.h"

MiniMap::~MiniMap()
{
}

void MiniMap::Initialize(const Vector3& scale, const Vector3& rotate, const Vector3& translate, const std::string filename)
{
	// texture
	texture_ = TextureManager::GetInstance()->StoreTexture("Resources/driftmap/driftmap.png");
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	// 初期化
	objectPram_ = Object3dManager::GetInstance()->StoreObject(filename, texture_, Transparency::Opaque);
	objectPram_.lock()->worldTransform.scale_ = scale;
	objectPram_.lock()->worldTransform.rotation_ = rotate;
	objectPram_.lock()->worldTransform.translation_ = translate;
	offset = translate;
	objectPram_.lock()->color = color_;

	if (objectPram_.lock()) {
		objectPram_.lock()->worldTransform.UpdateMatrix();
	}
}

void MiniMap::Update(Camera* camera)
{
	if (auto locked = objectPram_.lock()) {
		// カメラの情報取得
		Vector3 camPos = camera->GetTranslate();
		Vector3 camRot = camera->GetRotate();

		// カメラの回転行列を作成
		Matrix4x4 camRotMat = MakeRotationMatrix(camRot);

		// カメラの回転に合わせてオフセットをワールド変換
		Vector3 worldOffset = TransformNormal(offset, camRotMat);

		// オブジェクトのワールド位置を計算
		Vector3 finalPos = camPos + worldOffset;
		locked->worldTransform.translation_ = finalPos;

		// X軸に90度回転を加える
		constexpr float pi = -3.14159265f;
		Vector3 additionalRotation = { 90.0f * (pi / 180.0f), 0.0f, 0.0f };

		// カメラ回転 + X軸回転
		locked->worldTransform.rotation_ = camRot + additionalRotation;

		// 行列更新
		locked->worldTransform.UpdateMatrix();


	}

	/*if (objectPram_.lock()) {
		objectPram_.lock()->worldTransform.UpdateMatrix();
	}*/

#ifdef _DEBUG
	ImGui::Begin("minimap");
	ImGui::DragFloat3("Scale", &objectPram_.lock()->worldTransform.scale_.x, 0.1f);
	ImGui::DragFloat3("Rotate", &objectPram_.lock()->worldTransform.rotation_.x, 0.1f);
	ImGui::DragFloat3("Transform", &objectPram_.lock()->worldTransform.translation_.x, 0.1f);
	ImGui::End();
#endif // _DEBUG
}

void MiniMap::SetObjectPram()
{
	if (auto locked = objectPram_.lock()) {  // weak_ptrからshared_ptrに変換
		locked->isAlive = false;
	}
}
