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
	//// 影生成
	shadow_ = std::make_unique<PlaneProjectionShadow>();
	shadow_->Init(&worldTransform_,filename);
	

}

void Car::Update()
{
	// 車体の更新
	body_->Update();
	// 車輪の更新
	for (auto& tire : tires_) {
		tire->Update();
	}
	// バイシクルモデルでの車の動き
	BicycleModel();
	// 平行影の更新
	shadow_->Update();
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
	// 1フレームあたりの時間（60FPS）
	const float deltaTime = 1.0f / 60.0f;
	/*-----------------------本来CarEngineから持ってくる値--------------------------------*/
	if (Input::GetInstance()->GetJoystickState()) {
		if (Input::GetInstance()->PushRTrigger(0.05f)) {
			
		}
		throttle_ = Input::GetInstance()->GetRTValue();
		if (Input::GetInstance()->PushLTrigger(0.05f)) {
			
		}
		brake_ = Input::GetInstance()->GetLTValue();
	}
	// エンジントルク算出
	float engineTorque = maxEngineTorque_ * throttle_;
	// 駆動輪トルク（ギア比・ファイナル含む）
	float wheelTorque = engineTorque * 1.0f * 1.0f;
	// 駆動力[N] = トルク / 半径
	float driveForce = wheelTorque / wheelRadius_;
	const float maxBrakeForce = weight_ * 0.8f; // [N]
	// ブレーキ力 [N]（最大制動力を係数として）
	float brakeForce = brake_ * maxBrakeForce;

	// 駆動と逆向きに作用するので、減速として加速度に使う
	float netForce = driveForce - brakeForce;


	float brakeLimitForce = mu_ * weight_; // 最大摩擦力による制動限界
	brakeForce = std::min(brakeForce, brakeLimitForce);
	// 加速度[m/s2]
	float acceleration = (driveForce - brakeForce) / mass_;


	if (throttle_ < 0.01f && brake_ == 0.0f) {
		float engineBrakeForce = 0.2f * weight_; // 仮定値（調整可）
		acceleration -= engineBrakeForce / mass_;
	}
	/*----------------------------------------------------------*/


	// 入力：時速 [km/h] → 毎秒に変換
	float velocity_mps = speed_ / 3.6f;
	velocity_mps += acceleration * deltaTime;
	// 速度下限は0（バックは考慮せず）
	if (velocity_mps < 0.0f) {
		velocity_mps = 0.0f;
	}
	// km/hに戻す
	speed_ = velocity_mps * 3.6f;
	
	float frameSpeed = velocity_mps * deltaTime;

	// ホイールベース
	float wheelBase = frontLength + rearLength;

	// ステア角（既にラジアンと仮定）
	float steerAngle = *steering_->GetAngle();

	// 回転半径 R（ゼロ割防止）
	float turningRadius = (std::abs(std::tan(steerAngle)) > 0.0001f) ? (wheelBase / std::abs(std::tan(steerAngle))) : FLT_MAX;

	// 遠心力（必要な横力）
	float requiredLatForce = (mass_ * velocity_mps * velocity_mps) / turningRadius;

	// 摩擦による最大横グリップ力（全車体で一括で考える）
	float maxGripForce = mu_ * weight_; // = mu * m * g

	// グリップ率
	float gripRatio = 1.0f;
	if (requiredLatForce > maxGripForce) {
		gripRatio = maxGripForce / requiredLatForce;
	}

	// 回転角速度（ステアと速度により）
	float theta = (velocity_mps / wheelBase) * std::tan(steerAngle);

	// グリップ率が低いなら回転も減衰させる
	theta *= gripRatio;

	// 向き更新
	worldTransform_.rotation_.y += theta * deltaTime;

	// 移動更新
	worldTransform_.translation_.z += frameSpeed * std::cos(worldTransform_.rotation_.y);
	worldTransform_.translation_.x += frameSpeed * std::sin(worldTransform_.rotation_.y);

#ifdef _DEBUG
	ImGui::Begin("CarGripDebug");
	ImGui::Text("Required Lat Force: %.2f N", requiredLatForce);
	ImGui::Text("Max Grip Force: %.2f N", maxGripForce);
	ImGui::Text("Grip Ratio: %.2f", gripRatio);
	ImGui::Text("Theta: %.4f rad/frame", theta);
	ImGui::Text("Thorottle: %.4f ", throttle_);
	ImGui::Text("Brake: %.4f ", brake_);
	ImGui::Text("EngineTorque: %.4f ", engineTorque);
	ImGui::Text("wheelTorque: %.4f ", wheelTorque);
	ImGui::Text("driveForce: %.4f ", driveForce);
	ImGui::Text("acceleration: %.4f ", acceleration);
	ImGui::Text("velocity_mps: %.4f ", velocity_mps);
	ImGui::Text("speed_: %.4f ", speed_);
	ImGui::End();
#endif


}

void Car::Brake()
{
}
