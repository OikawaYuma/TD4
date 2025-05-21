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

void Car::Update()
{
	


	// 車体の更新
	body_->Update();

	// 車輪の更新
	for (auto& tire : tires_) {
		tire->Update();
	}
	BicycleModel();
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

void Car::BicycleModel()
{
	
	if (Input::GetInstance()->GetJoystickState()) {
		if (Input::GetInstance()->PushRTrigger(0.15f)) {
			speed_ += Input::GetInstance()->GetRTValue();
			if (speed_ >= 245.0f) {
				speed_ = 245.0f;
			}
		}
		else {
			speed_-=0.5f;
			if (speed_ <= 0.0f) {
				speed_ = 0.0f;
			}
		}
	}

	

	// 車速を100で割って正規化（おそらく 1.0 以下の小数値に調整）
	float adustSpeed = speed_ / 100.0f;

	// ホイールベース（前輪から後輪までの距離の合計）
	float wheelBase = frontLength + rearLength;

	// βは「滑り角」や「車両の方向と進行方向の差」などの補正角度を意味する
	float beta = std::atan(std::tan(*steering_->GetAngle()) * rearLength / wheelBase);

	// 車両の回転角速度を初期化
	float theta = 0;

	// 回転角速度をステア角と車速に基づいて計算
	theta = (adustSpeed / wheelBase) * std::tan(*steering_->GetAngle());

	// フレーム毎の経過時間（60fpsとして1フレーム分）
	float feltaTime = 1.0f / 60.0f;
	// 車両の向きをy軸回転として更新（thetaはラジアン単位）
	worldTransform_.rotation_.y += theta;

	// 車両の位置を更新（向きに基づいて前進）
	// z軸方向（前後方向）に移動：向いている方向にcosで前進
	worldTransform_.translation_.z += adustSpeed * std::cos(worldTransform_.rotation_.y);

	// x軸方向（左右方向）に移動：向いている方向にsinで横方向補正
	worldTransform_.translation_.x += adustSpeed * std::sin(worldTransform_.rotation_.y);
}
