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

void FollowCamera::Upadate()
{
    if (!target_) return;

    // 進行方向をrotation_.yから計算
    Vector3 forward = {
        std::sin(target_->rotation_.y),
        0.0f,
        std::cos(target_->rotation_.y)
    };

    // カメラの背後オフセット位置
    float cameraDistance = 10.0f;
    float cameraHeight = 2.0f;
    Vector3 behind = {
        target_->translation_.x - forward.x * cameraDistance,
        target_->translation_.y + cameraHeight,
        target_->translation_.z - forward.z * cameraDistance
    };

    // 滑らかな補間
    camera_->SetTranslate(Lerp(camera_->GetTranslate(), behind, 0.65f));

    // カメラ回転をターゲットの方向に合わせる（簡易LookAtの代替）
    Vector3 cameraRotation = {
        0.0f,
        (target_->rotation_.y),
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