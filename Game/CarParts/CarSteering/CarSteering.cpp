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
	// 曲がるときの左右回転
	float lxValue = Input::GetInstance()->JoyStickParmLX(1.0f);
	const float kYawMaxAngle = 35.0f; // 適当に調整してタイヤの最大角度
	angle = lxValue * kYawMaxAngle * (std::numbers::pi_v<float> / 180.0f);

#ifdef _DEBUG
	ImGui::Begin("Steering");
	ImGui::Text("%f padlx", lxValue);
	ImGui::Text("%f angleO", angle);
	ImGui::Text("%f angleR", lxValue * kYawMaxAngle);
	ImGui::End();
#endif // _DEBUG

}
