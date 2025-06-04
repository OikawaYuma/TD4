#include "FollowCamera.h"
#include "Input.h"
#include <iostream>
#include <cmath>
#include <numbers>

void FollowCamera::Init()
{
    camera_ = std::make_unique<Camera>();
    camera_->Initialize();
    camera_->SetTranslate({ 0, 0, 0 });
    camera_->SetFarClip(2000.0f);
    cameraTime_ = 1.0f;
    offsetZ_ = 0.0f;
    cameraDirection_ = NONEDIRECTION;
    previousLookAt_ = { 0, 0, 0 }; // 初期化
}


void FollowCamera::StartCameraEffect()
{
    Vector3 t = camera_->GetTranslate();
    Vector3 r = camera_->GetRotate();
    t.z += 0.25f;
    camera_->SetTranslate(t);
    camera_->SetRotate(r);
    camera_->Update();
}

// FollowCamera.cpp の Upadate() を修正
void FollowCamera::Upadate()
{
    if (!target_) return;

    // ターゲットの進行方向（Y回転角）を取得
    float targetAngle = target_->rotation_.y;

    // 前フレームとの差分（角速度）を計算
    float angleDiff = targetAngle - prevTargetAngle_;
    prevTargetAngle_ = targetAngle;

    // 角速度の絶対値を使い、sideAngleの目標値を決定
    float maxSideAngle = 0.7f; // 最大横オフセット（ラジアン、例: 40度くらい）
    float targetSideAngle = std::clamp(std::abs(angleDiff) * 600.0f, 0.0f, maxSideAngle);

    // 進行方向に応じて左右どちらにオフセットするか決める（例: 右旋回なら右側から見る）
    if (angleDiff > 0) targetSideAngle = -targetSideAngle;

    // 滑らかにLerp
    currentSideAngle_ = Lerp(currentSideAngle_, targetSideAngle, 0.01f);

    // カメラ位置計算
    float angle = targetAngle + currentSideAngle_;
    Vector3 forward = {
        std::sin(angle),
        0.0f,
        std::cos(angle)
    };

    float cameraDistance = 10.0f;
    float cameraHeight = 2.0f;
    float maxCameraHeight = 5.0f;
    float behindY = std::min(target_->translation_.y + cameraHeight, maxCameraHeight);

    Vector3 behind = {
        target_->translation_.x - forward.x * cameraDistance,
        behindY,
        target_->translation_.z - forward.z * cameraDistance
    };

    camera_->SetTranslate(Lerp(camera_->GetTranslate(), behind, 0.8f));

    Vector3 cameraRotation = {
        0.0f,
        angle,
        0.0f
    };
    camera_->SetRotate(cameraRotation);
    camera_->CameraDebug();

    camera_->Update();
}
void FollowCamera::SetTarget(const WorldTransform* target)
{
    target_ = target;
}