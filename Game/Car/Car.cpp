#include "Car.h"
#include <ModelManager.h>

void Car::Initialize(const Vector3& scale, const Vector3& rotate, const Vector3& translate, const std::string filename)
{
	// 車の核となる場所を設定　各クラスに送る座標
	worldTransform_.Initialize();
	// ステアリング生成（Tireにポインタを渡す処理があるためそれより前に記述）
	steering_ = std::make_unique<CarSteering>();
	steering_->Init();
	// body生成
	CreateCarBody();
	// tire生成
	CreateCarTire();

	

}

void Car::Update(float uiSpeed)
{
	


	// 車体の更新
	body_->Update();

	// 車輪の更新
	for (auto& tire : tires_) {
		tire->Update();
	}
	BicycleModel(uiSpeed);
	// UIクラスから出たスピードを足す
	worldTransform_.UpdateMatrix();
	// ステアリング更新
	steering_->Update();
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
	//　下記一行問題ICar参照
	frontLeftTire->SetSteeringAngle(steering_->GetAngle());
	tires_.push_back(std::move(frontLeftTire));
	// 右前車輪
	std::unique_ptr<ICarTire> frontRightTire = std::make_unique<FrontCarTire>();
	frontRightTire->Initialize({ 0.0f,0.0f,0.0f }, {}, { 0.79f,0.34f,1.31f }, "carTire");
	frontRightTire->SetParent(&worldTransform_);
	//　下記一行問題ICar参照
	frontRightTire->SetSteeringAngle(steering_->GetAngle());
	tires_.push_back(std::move(frontRightTire));
	// 左後車輪
	std::unique_ptr<ICarTire> rearLeftTire = std::make_unique<RearCarTire>();
	rearLeftTire->Initialize({ 0.0f,0.0f,0.0f }, {}, { -0.79f,0.34f,-1.31f }, "carTire");
	rearLeftTire->SetParent(&worldTransform_);
	//　下記一行問題ICar参照
	rearLeftTire->SetSteeringAngle(steering_->GetAngle());
	tires_.push_back(std::move(rearLeftTire));
	// 左後車輪
	std::unique_ptr<ICarTire> rearRightTire = std::make_unique<RearCarTire>();
	rearRightTire->Initialize({ 0.0f,0.0f,0.0f }, {}, { 0.79f,0.34f,-1.31f }, "carTire");
	rearRightTire->SetParent(&worldTransform_);
	//　下記一行問題ICar参照
	rearRightTire->SetSteeringAngle(steering_->GetAngle());
	tires_.push_back(std::move(rearRightTire));
}

void Car::Yawing()
{

}

void Car::BicycleModel(float speed)
{

	//float wheelBase = frontLength + rearLength;
	//float steerAngle = *steering_->GetAngle(); // ラジアン
	//float beta = atan(tan(steerAngle) * rearLength / wheelBase);

	//// ヨー角速度（rad/s）
	//float yawRate = (speed / wheelBase) * tan(steerAngle);

	//// フレーム時間（例えば 1/60 秒）
	//float deltaTime = 1.0f / 60.0f;

	//// ヨー角を更新（＝車体の向きを回す）
	//worldTransform_.rotation_.y += yawRate * deltaTime;

	//// 実際の移動方向を計算
	//float heading = worldTransform_.rotation_.y + beta;
	//worldTransform_.translation_.z += speed * cos(heading) * deltaTime;
	//worldTransform_.translation_.x += speed * sin(heading) * deltaTime;
	float adustSpeed = speed / 100.0f;
	float wheelBase = frontLength + rearLength;
	float beta = std:: atan(std::tan(*steering_->GetAngle()) * rearLength / wheelBase);
	float theta = 0;
	theta = (adustSpeed / wheelBase) * std::tan(*steering_->GetAngle());
	float feltaTime = 1.0f / 60.0f;
	worldTransform_.rotation_.y += (theta);
	worldTransform_.translation_.z += adustSpeed * std::cos(worldTransform_.rotation_.y);
	worldTransform_.translation_.x += adustSpeed * std::sin(worldTransform_.rotation_.y);
}
