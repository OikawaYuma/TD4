/**
* @ file FallowCamera.h
* @ brief GameSceneでプレイヤーに追従するカメラの設定
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "Camera.h"
#include "WorldTransform.h"
#include <memory>
#include <Car/Car.h>

enum CamreraDirection {
	XP,
	XM,
	ZP,
	ZM,
	NONEDIRECTION
};

// 
class LockOn;
class FollowCamera
{
public:
	void Init();
	void StartCameraEffect();
	void Upadate();


public: // Getter
	Camera* GetCamera() { return camera_.get(); }

public:
	void SetTarget(const WorldTransform* target);
	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn;};
private:
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Car> car_;

	// 追従対象
	const WorldTransform* target_ = nullptr;
	// ロックオン
	LockOn* lockOn_ = nullptr;

	float destinationAngleY_ = 0.0f;
	// 追従対象の残像座標
	Vector3 interarget_ = {};
	Vector3 previousLookAt_ = {};

	float offsetZ_ = 0.0f;
	float preOffsetZ_ = 0.0f;

	float cameraTime_ = 1.0f;
	bool isDash_ = false;
	float angletime = 0.0f;

	bool resetAngleFlag = false;
	float resetAngleTime_ = 0.0f;

	uint32_t cameraDirection_ = NONEDIRECTION;

	float cameraChanegeTimer_ = 0.0f;
	Vector3 preCameraTranslate_ {};
};

