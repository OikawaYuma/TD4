#include "CarView.h"
#include <ModelManager.h>

void CarView::Init()
{
	// モデルの読み込み
	ModelManager::GetInstance()->LoadModel("Resources/car", "car.obj");
	// texture
	texture_ = TextureManager::GetInstance()->StoreTexture("Resources/car/car.png");
	// collider
	colliderMode_ = CollisionMode::OBBc;
	// 初期化
	BaseObject::Initialize(
		{ 0.0f,0.0f,0.0f },
		{ 1.0f,1.0f,1.0f },
		{ 0.0f,0.21f,0.0f },
		"car");
	if (objectParam_.lock()) {
		objectParam_.lock()->worldTransform.translation_ = { 0.0f,-0.2f,-65.0f };
	}
}

void CarView::Update()
{
	DebugCar();
	if (objectParam_.lock()) {
		objectParam_.lock()->worldTransform.rotation_.y += 0.01f;
		objectParam_.lock()->worldTransform.UpdateMatrix();
	}
	BaseObject::Update();
}

void CarView::DebugCar()
{
	if (objectParam_.lock()) {
#ifdef _DEBUG
		ImGui::Text("CarView");
		ImGui::DragFloat3("Position", &objectParam_.lock()->worldTransform.translation_.x, 0.01f);
		ImGui::DragFloat3("Rotation", &objectParam_.lock()->worldTransform.rotation_.x, 0.01f);
		ImGui::DragFloat3("Scale", &objectParam_.lock()->worldTransform.scale_.x, 0.01f);
#endif // _DEBUG

		
	}
}
