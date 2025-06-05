/**
* @ file Camera.h
* @ brief 3D描画におけるCameraに関しての設定
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/
#pragma once
#include"WinAPI.h"
#include "imgui/imgui.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Transform.h"
#include "mathFunction.h"
#include <numbers>
#include "Input.h"
#include "algorithm"

class Camera
{
public:
	void Initialize();
	void Update();
	void CameraDebug();

	//debug
	void Move();
	void HandleGamepadMovement();
	void HandleRightStick(const XINPUT_STATE& joyState);

public: //Getter
	const Transform& GetTransform() const { return transform_; }
	const Vector3& GetRotate() const { return transform_.rotate; }
	const Vector3& GetTranslate() const { return transform_.translate; }
	const Matrix4x4& GetCameraMatrix() const { return cameraMatrix_; }
	const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }
	const Matrix4x4& GetViewprojectionMatrix() const { return viewProjectionMatrix_; }

	const float& GetFarClip() const { return farClip_; }

public: //Setter
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
	void SetPos(const Vector3& pos_) { transform_.translate = pos_; }
	void SetTranslate(const Vector3& translate) { transform_.translate = translate; }
	void SetFovY() {}
	void SetAspectRario() {}
	void SetNearClip() {}
	void SetFarClip(const float& clip) { farClip_ = clip; }

private:
	Transform transform_{};
	Matrix4x4 worldmatrix_{};
	Matrix4x4 cameraMatrix_{};
	Matrix4x4 viewMatrix_{};
	Matrix4x4 projectionMatrix_{};
	Matrix4x4 viewProjectionMatrix_{};
	float fovY_ = 0.45f;
	float asepectRatio_ = float(WinAPI::kClientWidth_) / float(WinAPI::kClientHeight_);
	float nearClip_ = 0.1f;
	float farClip_ = 1000.0f;

	//debug
	   /// <summary>
	/// 視点移動の感度
	/// </summary>
	float lookSensitivity = 0.03f;
	/// <summary>
	/// プレイヤーの移動速度
	/// </summary>
	float baseSpeed = 0.3f;
	float Yspeed = 1.0f;
	float boostedSpeed = 0.45f;
};


