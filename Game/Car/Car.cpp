#include "Car.h"
#include <ModelManager.h>

void Car::Initialize(const Vector3& scale, const Vector3& rotate, const Vector3& translate, const std::string filename)
{
	// 車の核となる場所を設定　各クラスに送る座標
	worldTransform_.Initialize();
	// body生成
	CreateCarBody();
	// tire生成
	CreateCarTire();

}

void Car::Update(float uiSpeed)
{

	// UIクラスから出たスピードを足す
	worldTransform_.translation_.z += uiSpeed / 100.0f;
	worldTransform_.UpdateMatrix();

	// 車体の更新
	body_->Update();

	// 車輪の更新
	for (auto& tire : tires_) {
		tire->Update();
	}
}

void Car::CreateCarBody()
{
	ModelManager::GetInstance()->LoadModel("Resources/carBody", "carBody.obj");
	// 車体に座標を送り初期化
	body_ = std::make_unique<CarBody>();
	body_->Initialize({}, {}, {}, "carBody");
	body_->SetParent(&worldTransform_);
}

void Car::CreateCarTire()
{
	ModelManager::GetInstance()->LoadModel("Resources/carTire", "carTire.obj");
	// 車輪に座標を送り初期化
	// 左前車輪
	std::unique_ptr<ICarTire> frontLeftTire = std::make_unique<FrontCarTire>();
	frontLeftTire->Initialize({ 0.0f,0.0f,0.0f }, {}, { -0.79f,0.34f,1.31f }, "carTire");
	frontLeftTire->SetParent(&worldTransform_);
	tires_.push_back(std::move(frontLeftTire));
	// 右前車輪
	std::unique_ptr<ICarTire> frontRightTire = std::make_unique<FrontCarTire>();
	frontRightTire->Initialize({ 0.0f,3.1415f,0.0f }, {}, { 0.79f,0.34f,1.31f }, "carTire");
	frontRightTire->SetParent(&worldTransform_);
	tires_.push_back(std::move(frontRightTire));
	// 左後車輪
	std::unique_ptr<ICarTire> rearLeftTire = std::make_unique<RearCarTire>();
	rearLeftTire->Initialize({ 0.0f,0.0f,0.0f }, {}, { -0.79f,0.34f,-1.31f }, "carTire");
	rearLeftTire->SetParent(&worldTransform_);
	tires_.push_back(std::move(rearLeftTire));
	// 左後車輪
	std::unique_ptr<ICarTire> rearRightTire = std::make_unique<RearCarTire>();
	rearRightTire->Initialize({ 0.0f,3.1415f,0.0f }, {}, { 0.79f,0.34f,-1.31f }, "carTire");
	rearRightTire->SetParent(&worldTransform_);
	tires_.push_back(std::move(rearRightTire));
}
