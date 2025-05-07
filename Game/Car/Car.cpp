#include "Car.h"
#include <ModelManager.h>

void Car::Initialize(const Vector3& scale, const Vector3& rotate, const Vector3& translate, const std::string filename)
{
	ModelManager::GetInstance()->LoadModel("Resources/carBody", "carBody.obj");
	ModelManager::GetInstance()->LoadModel("Resources/carTire", "carTire.obj");
	floorTex_ = TextureManager::GetInstance()->StoreTexture("Resources/car/car.png");
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	// 車の核となる場所を設定　各クラスに送る座標
	worldTransform_.Initialize();
	// 車体に座標を送り初期化
	body_ = std::make_unique<CarBody>();
	body_->Initialize({}, {}, {}, "carBody");
	body_->SetParent(&worldTransform_);
	// 車輪に座標を送り初期化
	// 左前車輪
	std:: unique_ptr<CarTire>frontLeftTire_ = std::make_unique<CarTire>();
	frontLeftTire_->Initialize({ 0.0f,0.0f,0.0f }, {}, { -0.79f,0.34f,1.31f }, "carTire");
	frontLeftTire_->SetParent(&worldTransform_);
	tires_.push_back(std::move(frontLeftTire_));
	// 右前車輪
	std::unique_ptr<CarTire>frontRightTire_ = std::make_unique<CarTire>();
	frontRightTire_->Initialize({ 0.0f,3.1415f,0.0f }, {}, { 0.79f,0.34f,1.31f }, "carTire");
	frontRightTire_->SetParent(&worldTransform_);
	tires_.push_back(std::move(frontRightTire_));
	// 左後車輪
	std::unique_ptr<CarTire>rearLeftTire_ = std::make_unique<CarTire>();
	rearLeftTire_->Initialize({ 0.0f,0.0f,0.0f }, {}, { -0.79f,0.34f,-1.31f }, "carTire");
	rearLeftTire_->SetParent(&worldTransform_);
	tires_.push_back(std::move(rearLeftTire_));
	// 左後車輪
	std::unique_ptr<CarTire>rearRightTire_ = std::make_unique<CarTire>();
	rearRightTire_->Initialize({ 0.0f,3.1415f,0.0f }, {}, { 0.79f,0.34f,-1.31f }, "carTire");
	rearRightTire_->SetParent(&worldTransform_);
	tires_.push_back(std::move(rearRightTire_));

	// マテリアル情報の初期化
	material_.color = { 1.0f,1.0f,1.0f,1.0f };
	material_.enableLighting = true;
	material_.uvTransform = MakeIdentity4x4();
	material_.shininess = 60.0f;
}

void Car::Update(float uiSpeed)
{

	// UIクラスから出たスピードを足す
	worldTransform_.translation_.z += uiSpeed / 100.0f;
	worldTransform_.UpdateMatrix();

	// 車体の更新
	body_->Update();
	// 車輪の更新
	for (std::list<std::unique_ptr<CarTire>>::iterator itr = tires_.begin(); itr != tires_.end(); itr++) {
		(*itr)->Update();
	}
}

WorldTransform* Car::GetWorldTransform() {
	return &worldTransform_;
}