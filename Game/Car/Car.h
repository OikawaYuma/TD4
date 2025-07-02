#pragma once
#include "CarParts/CarBody/CarBody.h"
#include "CarParts/CarEngine/CarEngine.h"
#include "CarParts/CarTire/ICarTire.h"
#include "CarParts/CarSteering/CarSteering.h"
#include "CarParts/CarTire/FrontCarTire/FrontCarTire.h"
#include "CarParts/CarTire/RearCarTire/RearCarTire.h"
#include <vector>
#include "PlaneProjectionShadow.h"
class Car
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& scale, const Vector3& rotate, const Vector3& translate, const std::string filename);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public: //Getter
	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	float* GetSpeed() { return &speed_; }
	Collider* GetBodyCollider() {return  body_->GetCollider(); }

private: // 生成処理

	/// <summary>
	/// 車体生成
	/// </summary>
	void CreateCarBody();

	/// <summary>
	/// タイヤ生成
	/// </summary>
	void CreateCarTire();

private: // 移動処理

	/// <summary>
	/// ステアリング操作による車体の傾き処理（Y軸回転）
	/// </summary>
	void Yawing();

	/// <summary>
	/// バイシクルモデルでの車の移動および回転処理
	/// </summary>
	void BicycleModel();

	/// <summary>
	/// アクセル処理
	/// </summary>
	void Accel();

	/// <summary>
	/// ブレーキ処理
	/// </summary>
	void Brake();

	void Drift();

	/// <summary>
	/// エンジントルクの計算
	/// </summary>
	void CulculateEngineTorque();

private: // 外部情報　本来はここに書くべきではない
	// 摩擦係数
	float mu_ = 2.0f; // とりあえず乾いたコンクリで考える（大体0.9~1.2くらいらしい、スポーツのタイヤだと1.1~1.2でもいいとか）
	// 重力
	float g_ = 9.8f;
private:
	WorldTransform worldTransform_{};
	std::vector<std::unique_ptr<BaseObject>> parts_;

	// 車体
	std::unique_ptr<CarBody> body_;
	// 車輪
	std::vector<std::unique_ptr<ICarTire>> tires_;
	// ステアリング
	std::unique_ptr<CarSteering> steering_;
	// 仮
	std::weak_ptr<ObjectPram> objectPram_{};
	// 平行影
	std::unique_ptr<PlaneProjectionShadow> shadow_;
	// 中心からの距離
	float frontLength = 1.31f;  // 重心から前輪まで
	float rearLength = 1.31f;   // 重心から後輪まで
	// 車の長さ
	float carLength_ = 3.0f; // 車の長さ（インプレッサは4.4mくらい）
	// 車の幅
	float carWidth_ = 1.75f; // 車の幅（インプレッサは1.75mくらい）
	// Object情報
	uint32_t floorTex_ = 0;
	Vector4 color_;
	Material material_{};
	// 車の荷重 文太のインプレッサはバージョンⅤで1250kgくらいらしい。　ここもjsonとかに飛ばしたい
	float mass_ = 1250.0f;
	// 実際にかかる重量
	float weight_ = mass_ * g_;
	// グリップ
	float grip_ = 0.0f;
	// 静止時の最大横グリップ
	float maxGrip_ = 0.0f;
	// 仮のスピード（後々エンジンから出力を受け取りタイヤの回転力とか合わせて考えたい）
	// 下記をエンジンの仮出力として用いてスピードを決める
	float speed_ = 0.0f;
	// 車体の速度ベクトル
	Vector2 velocityVec_; // x, z成分（横・縦）
private: // エンジンの出力が完成まで以下を使う　にへー頼んだ
	const float maxEngineTorque_ = 4000.0f; // 最大トルク[Nm] (適宜調整)

	float engineTorque_ = 0.0f; // エンジンのトルク[Nm]

	float wheelTorque_ = 0.0f; // 車輪のトルク[Nm] (エンジンのトルクをタイヤに伝える)

	float driveForce_ = 0.0f; // 車輪の駆動力[N] (タイヤのトルクから計算される)

private: // 車輪の情報
	const float wheelRadius_ = 0.3f; // ホイール半径[m]
	
	
	float throttle_ = 0.0f; // アクセルの強さ（0.0~1.0）

private: // 車のブレーキ処理に関する変数
	
	float brake_ = 0.0f; // ブレーキの強さ（0.0~1.0）

	float brakeForce_ = 0.0f; // ブレーキ力[N] (ブレーキの強さから計算される)
	
	// ブレーキの最大力
	const float maxBrakeForce = weight_ * mu_; // [N]

	bool isDrift_ = false;

private: // 完成モーメント用変数
	float yawAngularVelocity_ = 0.0f; // 車両の角速度（ヨー）
	float momentOfInertia_ = 0.0f;    // 慣性モーメント

private: // frame情報
	// 1フレームあたりの時間（60FPS）
	const float deltaTime_ = 1.0f / 60.0f;
	
};