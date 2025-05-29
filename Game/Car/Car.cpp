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

	// 時速から秒速にし、1frame当たりの速度に変換
	float adustSpeed = speed_ /3.6f / frameTime;

	// ホイールベース（前輪から後輪までの距離の合計）
	float wheelBase = frontLength + rearLength;

	//// 車両の回転角速度を初期化
	float theta = 0;

	//// 回転角速度をステア角と車速に基づいて計算
	theta = (adustSpeed / wheelBase) * std::tan(*steering_->GetAngle());

	// turning radius を計算
	float turningRadius = wheelBase / std::tan(*steering_->GetAngle());
	float velocity = adustSpeed * frameTime; // adustSpeedは speed / 100 なので元に戻す

	float requiredLatForce = std::abs((mass_ * velocity * velocity) / turningRadius);
	float gripRatio = 0;
	if (requiredLatForce <= weight_) {
		// 曲がれる場合：今の挙動そのまま適用
		worldTransform_.rotation_.y += theta;

		worldTransform_.translation_.z += adustSpeed * std::cos(worldTransform_.rotation_.y);
		worldTransform_.translation_.x += adustSpeed * std::sin(worldTransform_.rotation_.y);
	}
	else {
		// 曲がれない場合：滑る（アンダーステア）方向へ補正
		gripRatio = weight_ / requiredLatForce;

		// ステアが効きにくくなる → θを減衰させる（滑る）
		theta *= gripRatio;

		worldTransform_.rotation_.y += theta;

		// 滑っている分、前に行きすぎる
		worldTransform_.translation_.z += adustSpeed * std::cos(worldTransform_.rotation_.y);
		worldTransform_.translation_.x += adustSpeed * std::sin(worldTransform_.rotation_.y) * gripRatio; // 横の食いつきは低下
	}
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
