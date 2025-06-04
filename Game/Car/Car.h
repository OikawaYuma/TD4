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

private: // 外部情報　本来はここに書くべきではない
	// 摩擦係数
	float mu_ = 1.5f; // とりあえず乾いたコンクリで考える（大体0.9~1.2くらいらしい、スポーツのタイヤだと1.1~1.2でもいいとか）
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


private: // エンジンの出力が完成まで以下を使う　にへー頼んだ
	const float maxEngineTorque_ = 400.0f; // 最大トルク[Nm] (適宜調整)
	const float wheelRadius_ = 0.3f; // ホイール半径[m]
	
	// アクセル
	float throttle_ = 0.0f;
	// ブレーキ
	float brake_ = 0.0f;
	


};

