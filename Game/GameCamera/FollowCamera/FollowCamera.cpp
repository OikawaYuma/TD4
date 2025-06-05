#include "FollowCamera.h"
#include "Input.h"
#include <iostream>
#include <cmath>
#include <numbers>
// 角度を -π～π に正規化
float NormalizeAngle(float angle) {
    while (angle > std::numbers::pi_v<float>) angle -= std::numbers::pi_v<float> *2.0f;
    while (angle < -std::numbers::pi_v<float>) angle += std::numbers::pi_v<float> *2.0f;
    return angle;
}

// 角度用Lerp（最短経路で補間）
float LerpAngle(float from, float to, float t) {
    float diff = NormalizeAngle(to - from);
    return from + diff * t;
}

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

void FollowCamera::Upadate()
{
    if (!target_) return;

    float rightX = Input::GetInstance()->JoyStickParmRX(1.0f);
    float rightY = Input::GetInstance()->JoyStickParmRY(1.0f);

    float sensitivityYaw = 0.03f;
    float sensitivityPitch = 0.02f;

    const float deadzone = 0.15f;
    bool isRightStickActive = (std::abs(rightX) > deadzone) || (std::abs(rightY) > deadzone);

    // 現在時刻を取得
    auto now = std::chrono::steady_clock::now();

    if (isRightStickActive) {
        manualYaw_ += rightX * sensitivityYaw;
        manualPitch_ += rightY * sensitivityPitch;
        manualYaw_ = NormalizeAngle(manualYaw_);
        // 最後の入力時刻を更新
        lastInputTime_ = now;
    }
    else {
        // 入力がなくなってからの経過時間を計算
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastInputTime_).count();
        if (elapsed >= 3) {
            // 3秒経過後にLerpで0に戻す
            manualYaw_ = LerpAngle(manualYaw_, 0.0f, 0.05f);
            manualPitch_ = Lerp(manualPitch_, 0.0f, 0.05f);
        }
        // 3秒未満なら何もしない（角度を維持）
    }

    manualPitch_ = std::clamp(manualPitch_, -std::numbers::pi_v<float> / 4.0f, std::numbers::pi_v<float> / 4.0f);

    float targetAngle = target_->rotation_.y;
    float angleDiff = targetAngle - prevTargetAngle_;
    prevTargetAngle_ = targetAngle;

    float maxSideAngle = 0.7f;
    float targetSideAngle = std::clamp(std::abs(angleDiff) * 700.0f, 0.0f, maxSideAngle);

    if (angleDiff > 0) targetSideAngle = -targetSideAngle;

    currentSideAngle_ = Lerp(currentSideAngle_, targetSideAngle, 0.01f);

    float angle = targetAngle + currentSideAngle_ + manualYaw_;
    Vector3 forward = {
        std::sin(angle),
        0.0f,
        std::cos(angle)
    };

    float cameraDistance = 10.0f;
    float cameraHeight = 2.0f;
    float maxCameraHeight = 5.0f;
    float behindY = std::min(target_->translation_.y + cameraHeight, maxCameraHeight);

    float pitchOffset = std::sin(manualPitch_) * cameraDistance;

    Vector3 behind = {
        target_->translation_.x - forward.x * cameraDistance,
        behindY + pitchOffset,
        target_->translation_.z - forward.z * cameraDistance
    };

    camera_->SetTranslate(Lerp(camera_->GetTranslate(), behind, 0.8f));

    Vector3 cameraRotation = {
        manualPitch_,
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
