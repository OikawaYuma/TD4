#include "Car.h"
#include <ModelManager.h>

void Car::Initialize(const Vector3& scale, const Vector3& rotate, const Vector3& translate, const std::string filename)
{
	scale;
	rotate;
	// 車の核となる場所を設定　各クラスに送る座標
	worldTransform_.Initialize();
	worldTransform_.translation_ = translate;
	worldTransform_.translation_.y = 0.345f;
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
	
	// 慣性モーメントの初期化（仮定値、調整可能）
	momentOfInertia_ = (1.0f / 12.0f) * mass_ * (carLength_ * carLength_ + carWidth_ * carWidth_);
	// 物理設定
	rigidBody_.worldTransform = &worldTransform_;
	rigidBody_.mass = 1.0f;
	rigidBody_.useGravity = true;
	if(physicsSystem_){
		physicsSystem_->AddObject(&rigidBody_);
	}
	
}

void Car::Update()
{
	

	// アクセル処理
	Accel();
	// ブレーキ処理
	Brake();
	// エンジントルク計算（必要なら）
	CulculateEngineTorque();
	// バイシクルモデルでの車の動き
	BicycleModel();
	// 車体の更新
	body_->Update();
	// 車輪の更新
	for (auto& tire : tires_) {
		tire->Update();
	}
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
	ModelManager::GetInstance()->LoadModel("Resources/box", "box.obj");
	// 車体に座標を送り初期化
	body_ = std::make_unique<CarBody>();
	body_->Initialize({}, {}, {}, "carBody");
	body_->SetParent(&worldTransform_);
	body_->SetCollisionScale({ 0.88f,
		0.69f,2.3f });

}

void Car::CreateCarTire()
{
	ModelManager::GetInstance()->LoadModel("Resources/carTire", "carTire.obj");
	// 車輪に座標を送り初期化
	// 左前車輪
	std::unique_ptr<ICarTire> frontLeftTire = std::make_unique<FrontCarTire>();
	frontLeftTire->SetCar(this);
	frontLeftTire->Initialize({ 0.0f,0.0f,0.0f }, {}, { -0.71f,-0.0f,1.31f }, "carTire");
	frontLeftTire->SetParent(&worldTransform_);
	//　下記一行問題ICar参照
	frontLeftTire->SetSteeringAngle(steering_->GetAngle());
	tires_.push_back(std::move(frontLeftTire));
	// 右前車輪
	std::unique_ptr<ICarTire> frontRightTire = std::make_unique<FrontCarTire>();
	frontRightTire->SetCar(this);
	frontRightTire->Initialize({ 0.0f,0.0f,0.0f }, {}, { 0.71f,-0.00f,1.31f }, "carTire");
	frontRightTire->SetParent(&worldTransform_);
	//　下記一行問題ICar参照
	frontRightTire->SetSteeringAngle(steering_->GetAngle());
	tires_.push_back(std::move(frontRightTire));
	// 左後車輪
	std::unique_ptr<ICarTire> rearLeftTire = std::make_unique<RearCarTire>();
	rearLeftTire->SetCar(this);
	rearLeftTire->Initialize({ 0.0f,0.0f,0.0f }, {}, { -0.71f,-0.00f,-1.31f }, "carTire");
	rearLeftTire->SetParent(&worldTransform_);
	//　下記一行問題ICar参照
	rearLeftTire->SetSteeringAngle(steering_->GetAngle());
	tires_.push_back(std::move(rearLeftTire));
	// 左後車輪
	std::unique_ptr<ICarTire> rearRightTire = std::make_unique<RearCarTire>();
	rearRightTire->SetCar(this);
	rearRightTire->Initialize({ 0.0f,0.0f,0.0f }, {}, { 0.71f,-0.00f,-1.31f }, "carTire");
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
	
	// 入力：時速 [km/h] → 毎秒に変換
	float velocity_mps = speed_ / 3.6f;
	// 制動距離 = v^2 / (2 * a)
	float brakingDeceleration = brakeForce_ / mass_; // 加速度（正値でOK）
	float brakingDistance = (velocity_mps * velocity_mps) / (2.0f * brakingDeceleration);
	brakingDistance;
	// 加速度[m/s2]
	float acceleration = (driveForce_ - brakeForce_) / mass_;
	if (throttle_ < 0.01f && brake_ == 0.0f) {
		float engineBrakeForce = 0.2f * weight_; // 仮定値（調整可）
		acceleration -= engineBrakeForce / mass_;
	}
	/*----------------------------------------------------------*/

	velocity_mps += acceleration * deltaTime_;



	if (240 /3.6 <= velocity_mps) {
		velocity_mps = 240 / 3.6f; // 最大速度制限（240km/hをm/sに変換）
	}
	// 速度下限は0（バックは考慮せず）
	if (velocity_mps < 0.0f) {
		velocity_mps = 0.0f;
	}
	// km/hに戻す
	speed_ = velocity_mps * 3.6f;
	
	float frameSpeed = velocity_mps * deltaTime_;
	frameSpeed;
	// 車体方向ベクトル（rotation_.yで向いてる方向）
	Vector2 forwardVec = { std::sin(worldTransform_.rotation_.y), std::cos(worldTransform_.rotation_.y) };

	// 駆動・制動による加速
	Vector2 accelVec = forwardVec * velocity_mps;
	velocityVec_.x += accelVec.x / 3.6f;
	velocityVec_.y += accelVec.y / 3.6f;

	// 摩擦による速度の減衰（スライド抑制）
	float dragCoef = 0.1f; // 調整可
	velocityVec_.x *= (1.0f - dragCoef);
	velocityVec_.y *= (1.0f - dragCoef);

	// ホイールベース
	float wheelBase = frontLength + rearLength;
	float steerAngle = *steering_->GetAngle();
	//
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
	//// 回転角速度（ステアと速度により）
	float theta = (velocity_mps / wheelBase) * std::tan(steerAngle);

	// 実際の速度の方向
	float velocityAngle = std::atan2(velocityVec_.x, velocityVec_.y);

	// 車体の向き
	float headingAngle = worldTransform_.rotation_.y;

	// スリップ角（ズレ角）
	float slipAngle = velocityAngle - headingAngle;
	slipAngle;
	float speed_mps = std::sqrt(velocityVec_.x * velocityVec_.x + velocityVec_.y * velocityVec_.y);

	// 車の重心位置（前後方向）
	float cgHeight = 0.5f; // 車の重心高（m）←チューニング推奨
	// ▼ 荷重移動（前後方向）
	float weightTransfer = (acceleration * cgHeight * mass_) / wheelBase;
	// 前後の荷重移動（前：後 = rearLength：frontLength）
	float frontWeight = (weight_ * (rearLength / wheelBase)) - weightTransfer;
	float rearWeight = (weight_ * (frontLength / wheelBase)) + weightTransfer;

	float frontGripMax = mu_ * frontWeight;
	// 重み付けで前後の必要横力を近似（前：後 = rearLength：frontLength）
	float frontLatReq = requiredLatForce * (rearLength / wheelBase);
	float rearLatReq = requiredLatForce * (frontLength / wheelBase);
	rearLatReq;
		frontLatReq;
	// グリップ率（0〜1）で表現
	float frontGripRatio = std::min(1.0f, frontGripMax / frontLatReq);
	// --- 後輪グリップをブレーキで減らす ---
	float rearGripMax = mu_ * rearWeight;
	// ブレーキがかかっている場合、後輪のグリップを減少させる
	float rearGripMultiplier = 1.0f;
	if (brake_ > 0.1f && velocity_mps > 10.0f) {
		rearGripMultiplier = std::clamp(1.0f - brake_ * 0.8f, 0.4f, 1.0f);
	}
	rearGripMax *= rearGripMultiplier;

	// 前輪の旋回影響が gripRatio により弱まる（アンダーステア）
	float targetAngularVel = (speed_mps / wheelBase) * std::tan(steerAngle);
	targetAngularVel *= frontGripRatio;
	float rearGripRatio = std::min(1.0f, rearGripMax / ((mass_ * velocity_mps * velocity_mps) / (wheelBase / std::abs(std::tan(steerAngle)) + 0.0001f)));
	targetAngularVel *= rearGripRatio;

	// 徐々に現在のヨー角速度に反映
	float yawDamping = 4.0f; // 調整可
	float yawDelta = (targetAngularVel - theta) * yawDamping * deltaTime_;
	theta += yawDelta;


	// 車体の回転更新
	worldTransform_.rotation_.y += theta * deltaTime_;
	
	worldTransform_.translation_.x += velocityVec_.x * deltaTime_;
	worldTransform_.translation_.z += velocityVec_.y * deltaTime_;

	// 押し出しの処理（めんどいから後で関数分ける）
	int maxCorrection = 0; // 何回計算するか（調整して）正直あんま変わんない気がする精度に関わる
	for (int i = 0; i < maxCorrection; ++i) {
		if (!body_->GetIsHit()) break;
		Vector3 penetration = body_->GetPenetration();
		Vector3 normal = body_->GetNormal();
		// 法線の正規化
		if (Length(normal) > 0.0001f) {
			normal = Normalize(normal);
		}
		else {
			normal = { 0, 1, 0 };
		}
		float penetrationLength = std::abs(Dot(penetration, normal)); // 取得した法線と押し出し量から押し出す長さ決める
		Vector3 pushOut = normal * penetrationLength; // 押し出したい方向決める
		worldTransform_.translation_ = worldTransform_.translation_ + pushOut; // 押し出し
		body_->Update(); // 補正後に衝突判定を更新する
	}

	// スライドさせる処理
	if (body_->GetIsHit()) {
		Vector3 penetration = body_->GetPenetration();
		Vector3 normal = body_->GetNormal();
		// normalの正規化
		if (Length(normal) > 0.0001f) {
			normal = Normalize(normal);
		}
		else {
			normal = { 0, 1, 0 }; // 万一のためのデフォルト
		}
		// penetrationLengthの絶対値を利用
		float penetrationLength = std::abs(Dot(penetration, normal));
		float penetrationAttenuation = 0.4f; // 100%補正
		Vector3 pushOut = normal * penetrationLength * penetrationAttenuation;
		worldTransform_.translation_ = worldTransform_.translation_ + pushOut;

		// Slideで移動方向を補正
		// なんか知らんけどベクトルが二次元だから無理やり入れます
		Vector3 move = { velocityVec_.x,0.0f,velocityVec_.y };
		Vector3 slidVelocity = body_->Slide(deltaTime_ * move, normal);
		// スライドベクトルの長さを制限
		float slidLen = Length(slidVelocity);
		float veloLen = Length(deltaTime_ * move);
		if (slidLen > veloLen) {
			slidVelocity = Normalize(slidVelocity) + deltaTime_ * move;
		}
		worldTransform_.translation_ = worldTransform_.translation_ + slidVelocity;

		// 速度減衰を強める
		float dot = Dot(deltaTime_ * move, normal);
		if (dot > 0.0f) {
			speed_ *= (1.0f - dot * 0.8f); // 減衰を強める
			//if (speed_ < 0.5f) speed_ = 0.0f;
		}
	}


#ifdef _DEBUG
	ImGui::Begin("CarGripDebug");
	/*ImGui::Text("Required Lat Force: %.2f N", requiredLatForce);
	ImGui::Text("Max Grip Force: %.2f N", maxGripForce);*/
	ImGui::Text("Grip Ratio: %.2f", gripRatio);
	ImGui::Text("Theta: %.4f rad/frame", theta);
	ImGui::Text("Thorottle: %.4f ", throttle_);
	ImGui::Text("Brake: %.4f ", brake_);
	ImGui::Text("EngineTorque: %.4f ", engineTorque_);
	ImGui::Text("wheelTorque: %.4f ", wheelTorque_);
	ImGui::Text("driveForce: %.4f ", driveForce_);
	ImGui::Text("acceleration: %.4f ", acceleration);
	ImGui::Text("velocity_mps: %.4f ", velocity_mps);
	ImGui::Text("speed_: %.4f ", speed_);
	ImGui::Text("Braking Distance: %.2f m", brakingDistance);
	ImGui::Text("BrakeForce: %.2f N", brakeForce_);
	ImGui::Text("Acceleration: %.2f m/s^2", acceleration);
	ImGui::DragFloat("Mu:", &mu_,0.01f);
	// --- ヨーモーメント関連のデバッグ表示 ---
	ImGui::Separator();
	/*ImGui::Text("Yaw Moment: %.2f Nm", yawMoment);
	ImGui::Text("Moment of Inertia: %.2f kg*m^2", momentOfInertia_);
	ImGui::Text("Yaw Angular Accel: %.4f rad/s^2", yawAngularAccel);*/
	ImGui::Text("Yaw Angular Velocity: %.4f rad/s", yawAngularVelocity_);
	ImGui::End();
#endif


}

void Car::Accel()
{
	if (Input::GetInstance()->GetJoystickState()) {
		Input::GetInstance()->PushRTrigger(0.05f);
		throttle_ = Input::GetInstance()->GetRTValue();
	}
}

void Car::Brake()
{
	if (Input::GetInstance()->GetJoystickState()) {
		Input::GetInstance()->PushLTrigger(0.05f);
		brake_ = Input::GetInstance()->GetLTValue();
	}

	// ブレーキ力 [N]（最大制動力を係数として）
	brakeForce_ = (brake_ * maxBrakeForce);

	float brakeLimitForce = mu_ * weight_; // 最大摩擦力による制動限界
	// ブレーキ力が最大摩擦力を超えないように制限
	brakeForce_ = std::min(brakeForce_, brakeLimitForce);
}

void Car::Drift()
{
}

void Car::CulculateEngineTorque()
{
	// エンジントルク算出
	engineTorque_ = maxEngineTorque_ * throttle_;
	// 駆動輪トルク（ギア比・ファイナル含む）
	wheelTorque_ = engineTorque_ * 1.0f * 1.0f;
	// 駆動力[N] = トルク / 半径
	driveForce_ = wheelTorque_ / wheelRadius_;
}
