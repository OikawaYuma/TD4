#include "Car.h"
#include <ModelManager.h>
void Car::Initialize(const Vector3& scale, const Vector3& rotate, const Vector3& translate, const std::string filename)
{
	// 車の核となる場所を設定　各クラスに送る座標
	worldTransform_.Initialize();
	worldTransform_.translation_ = translate;
	// ステアリング生成（Tireにポインタを渡す処理があるためそれより前に記述）
	steering_ = std::make_unique<CarSteering>();
	steering_->Init();
	// body生成
	CreateCarBody();
	// tire生成
	CreateCarTire();
	//// 影生成
	//shadow_ = std::make_unique<PlaneProjectionShadow>();
	//shadow_->Init(&worldTransform_,filename);
	

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
	//shadow_->Update();
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
	frontLeftTire->Initialize({ 0.0f,0.0f,0.0f }, {}, { -0.71f,0.4f,1.31f }, "carTire");
	frontLeftTire->SetParent(&worldTransform_);
	//　下記一行問題ICar参照
	frontLeftTire->SetSteeringAngle(steering_->GetAngle());
	tires_.push_back(std::move(frontLeftTire));
	// 右前車輪
	std::unique_ptr<ICarTire> frontRightTire = std::make_unique<FrontCarTire>();
	frontRightTire->Initialize({ 0.0f,0.0f,0.0f }, {}, { 0.71f,0.4f,1.31f }, "carTire");
	frontRightTire->SetParent(&worldTransform_);
	//　下記一行問題ICar参照
	frontRightTire->SetSteeringAngle(steering_->GetAngle());
	tires_.push_back(std::move(frontRightTire));
	// 左後車輪
	std::unique_ptr<ICarTire> rearLeftTire = std::make_unique<RearCarTire>();
	rearLeftTire->Initialize({ 0.0f,0.0f,0.0f }, {}, { -0.71f,0.4f,-1.31f }, "carTire");
	rearLeftTire->SetParent(&worldTransform_);
	//　下記一行問題ICar参照
	rearLeftTire->SetSteeringAngle(steering_->GetAngle());
	tires_.push_back(std::move(rearLeftTire));
	// 左後車輪
	std::unique_ptr<ICarTire> rearRightTire = std::make_unique<RearCarTire>();
	rearRightTire->Initialize({ 0.0f,0.0f,0.0f }, {}, { 0.71f,0.4f,-1.31f }, "carTire");
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
			speed_ -= 0.5f;
			if (speed_ <= 0.0f) {
				speed_ = 0.0f;
			}
		}
		if (Input::GetInstance()->GetJoystickState()) {
			if (Input::GetInstance()->PushLTrigger(0.5f)) {
				speed_ -= Input::GetInstance()->GetLTValue();
				if (speed_ <= 0.0f) {
					speed_ = 0.0f;
				}
			}
		}
		if (Input::GetInstance()->GetJoystickState()) {
			if (Input::GetInstance()->PushJoyButton(XINPUT_GAMEPAD_X)) {
				speed_ -= 0.9f;
				if (speed_ <= 0.0f) {
					speed_ = 0.0f;
				}
			}
		}
	}

	

	const float frameTime = 60.0f;
	float adustSpeed = speed_ / 3.6f / frameTime;
	float wheelBase = frontLength + rearLength;
	float steerAngle = *steering_->GetAngle();
	float steerEffect = 1.2f;  // 曲がりやすさを強調する係数
	float theta = (adustSpeed / wheelBase) * std::tan(steerAngle) * steerEffect;

	float turningRadius = wheelBase / std::tan(steerAngle);
	float velocity = adustSpeed * frameTime;

	float requiredLatForce = std::abs((mass_ * velocity * velocity) / turningRadius);
	float gripRatio = 1.0f;

	if (requiredLatForce > weight_) {
		gripRatio = weight_ / requiredLatForce;

		const float gripInfluence = 0.6f;  // 滑りによる影響度（小さいほど曲がりやすくなる）
		theta *= (1.0f - gripInfluence) + gripInfluence * gripRatio;
	}

	worldTransform_.rotation_.y += theta;

	float yaw = worldTransform_.rotation_.y;
	Vector3 forward = {
		std::sin(yaw),
		0.0f,
		std::cos(yaw)
	};

	//gripRatioを移動全体に影響させる（アンダーステア時は曲がりが浅く＝方向も前寄りに）
	worldTransform_.translation_.x += forward.x * adustSpeed;
	worldTransform_.translation_.z += forward.z * adustSpeed;

#ifdef _DEBUG
	ImGui::Begin("CarGrip");
	ImGui::Text("saideForce : %f", requiredLatForce);
	ImGui::Text("weight : %f", weight_);
	ImGui::Text("grip : %f", gripRatio);
	ImGui::Text("theta : %f", theta);
	//ImGui::Text("theta : %f", theta);
	ImGui::End();
#endif // _DEBUG


}
