 #include "Camera.h"


void Camera::Initialize() {
	transform_={ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-80.0f} };
	//WinAPI* sWinAPI = WinAPI::GetInstance();

	cameraMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, asepectRatio_, nearClip_, farClip_);
	//worldCameraMatrix = Multiply(worldmatrix, Multiply(viewMatrix, projectionMatrix));
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}

void Camera::Update() {
	cameraMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, asepectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);


}
void Camera::Move()
{
    // ゲームパッドによる移動処理
    HandleGamepadMovement();
}
void Camera::HandleGamepadMovement()
{
    XINPUT_STATE joyState;
    if (Input::GetInstance()->GetJoystickState2(joyState))
    {
        // 左スティックによる移動
        Vector3 moveLeftStick = { 0, 0, 0 };
        const float leftStickDeadZone = 0.2f;
        if (std::abs(joyState.Gamepad.sThumbLX) > leftStickDeadZone * SHRT_MAX ||
            std::abs(joyState.Gamepad.sThumbLY) > leftStickDeadZone * SHRT_MAX)
        {
            moveLeftStick = {
                (float)joyState.Gamepad.sThumbLX / SHRT_MAX,
                0.0f,
                (float)joyState.Gamepad.sThumbLY / SHRT_MAX
            };

            float inputMagnitude = Length(moveLeftStick);
            if (inputMagnitude > leftStickDeadZone)
            {
                moveLeftStick.x *= baseSpeed;
                moveLeftStick.z *= baseSpeed;
            }
        }

        // カメラの向きに合わせて移動方向を調整
        if (moveLeftStick.x != 0.0f || moveLeftStick.z != 0.0f)
        {
            float cosY = cosf(transform_.rotate.y);
            float sinY = sinf(transform_.rotate.y);
            Vector3 move = {
                moveLeftStick.x * cosY + moveLeftStick.z * sinY,
                0.0f,
                moveLeftStick.z * cosY - moveLeftStick.x * sinY
            };
            transform_.translate.x += move.x;
            transform_.translate.z += move.z;
        }
        // 右スティックによる視点移動
        HandleRightStick(joyState);
        if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
        {
            // デバッグ用：地面を上昇
            transform_.translate.y += Yspeed;
        }
        if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
        {
            // デバッグ用：地面を下降
            transform_.translate.y -= Yspeed;
        }
    }
}


void Camera::HandleRightStick(const XINPUT_STATE& joyState)
{
    const float rightStickDeadZone = 0.1f;
    if (std::abs(joyState.Gamepad.sThumbRX) > rightStickDeadZone * SHRT_MAX ||
        std::abs(joyState.Gamepad.sThumbRY) > rightStickDeadZone * SHRT_MAX)
    {
        float rightStickX = (float)joyState.Gamepad.sThumbRX / SHRT_MAX * lookSensitivity;
        float rightStickY = (float)joyState.Gamepad.sThumbRY / SHRT_MAX * lookSensitivity;

        transform_.rotate.y += rightStickX;  // Yaw（左右の回転）
        transform_.rotate.x -= rightStickY;  // Pitch（上下の回転）

        // ピッチ角度の制限
        const float maxPitch = 45.0f * (float)std::numbers::pi / 180.0f;
        const float minPitch = -45.0f * (float)std::numbers::pi / 180.0f;
        transform_.rotate.x = std::clamp(transform_.rotate.x, minPitch, maxPitch);
    }
}




/// <summary>
/// カメラのデバッグ情報を表示する関数
/// </summary>
void Camera::CameraDebug()
{
#ifdef _DEBUG

    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.0f, 0.7f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.3f, 0.5f));
    ImGui::Begin("camera");
    ImGui::Text("Move : Left stick");
    ImGui::Text("View : Right stick");
    ImGui::Text("Y Move : LB RB button");

    // FOVの調整
    ImGui::DragFloat("FOV", &fovY_, 0.01f);

    // 位置・回転・スケールの調整
    float translate[3] = { transform_.translate.x,transform_.translate.y,transform_.translate.z };
    ImGui::DragFloat3("transform", translate, 0.01f);
    float rotate[3] = { transform_.rotate.x,transform_.rotate.y,transform_.rotate.z };
    ImGui::DragFloat3("rotate", rotate, 0.01f);
    float scale[3] = { transform_.scale.x, transform_.scale.y, transform_.scale.z };
    ImGui::DragFloat3("scale", scale, 0.01f);

    transform_.translate = { translate[0],translate[1],translate[2] };
    transform_.rotate = { rotate[0],rotate[1],rotate[2] };
    transform_.scale = { scale[0],scale[1],scale[2] };
    ImGui::DragFloat("XZspeed", &baseSpeed, 0.01f);
    ImGui::DragFloat("Yspeed", &Yspeed, 0.01f);
    ImGui::DragFloat("SEN", &lookSensitivity, 0.001f);

    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
#endif // _DEBUG

}

