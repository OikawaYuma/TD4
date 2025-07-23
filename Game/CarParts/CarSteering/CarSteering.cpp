#include "CarSteering.h"
#include "Input.h"
#include "mathFunction.h"
#include <cmath>
#include <numbers>
#include "ImGuiCommon.h"
void CarSteering::Init()
{
	angle = 0.0f;
}

void CarSteering::Update()
{
    // 入力取得
    float lxValue = Input::GetInstance()->JoyStickParmLX(1.0f);
    const float kYawMaxAngle = 35.0f; // 最大ステア角 [度]
    const float kLerpSpeed = 2.0f;     // 補間速度（大きいほど追従が早い）

    // 入力から目標角度（ラジアン）を計算
    float targetAngle = lxValue * kYawMaxAngle * (std::numbers::pi_v<float> / 180.0f);

    // 前フレームとの補間（線形補間）
    angle = Lerp(angle, targetAngle, kLerpSpeed / (60.0f));

#ifdef _DEBUG
    ImGui::Begin("Steering");
    ImGui::Text("lx: %.3f", lxValue);
    ImGui::Text("angle (rad): %.3f", angle);
    ImGui::Text("targetAngle (deg): %.3f", lxValue * kYawMaxAngle);
    ImGui::End();
#endif // _DEBUG

}
