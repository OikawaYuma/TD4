#include "UI.h"
#include <imgui/imgui.h>
#include <Input.h>
#include <numbers>

UI::~UI()
{
}

void UI::Initialize()
{
	// メーター本体
	SpeedSprite_ = std::make_unique<Sprite>();
	SpeedSprite_->SetTexture(TextureManager::GetInstance()->StoreTexture("Resources/meter.png"));
	SpeedSprite_->Init("Resources/meter.png");

	// 赤い針
	SpeedMeterSprite_ = std::make_unique<Sprite>();
	SpeedMeterSprite_->SetTexture(TextureManager::GetInstance()->StoreTexture("Resources/hari.png"));
	SpeedMeterSprite_->Init("Resources/hari.png");
	SpeedMeterSprite_->SetRot(hariRotate_);
	// アンカーポイントの設定
	SpeedMeterSprite_->SetAnchorPoint({ 0.5f, 0.9333f });
	// 愚か者
	orokamono_ = std::make_unique<Sprite>();
	orokamono_->SetTexture(TextureManager::GetInstance()->StoreTexture("Resources/orokamono.jpg"));
	orokamono_->Init("Resources/orokamono.jpg");
	orokamono_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	orokamono_->SetPosition({ 640.0f,360.0f });
	orokamono_->SetColor({ 1.0f,1.0f,1.0f,0.0f });
	
}

void UI::Update()
{
	// アンカーの調整
	float imageHeight = 150.0f;
	float offsetY = 10.0f;
	float anchorY = 1.0f - (offsetY / imageHeight);

	SpeedMeterSprite_->SetAnchorPoint({ 0.5f, anchorY });

	// メーターの針の回転計算
	speedRatio = *speed / maxSpeed;
	float angle = minAngle + (maxAngle - minAngle) * speedRatio;

	// ラジアンに変換して z 回転に適用
	hariRotate_.z = angle * (std::numbers::pi_v<float> / 180.0f);


	SpeedSprite_->Update();
	SpeedSprite_->SetPosition(pos_);
	SpeedSprite_->SetSize(scale_);

	SpeedMeterSprite_->Update();
	SpeedMeterSprite_->SetPosition(hariPos_);
	SpeedMeterSprite_->SetSize(hariScale_);
	SpeedMeterSprite_->SetRot(hariRotate_);
  
  
	orokamono_->Update();
	orokamono_->SetSize({ 1000.0f,1000.0f });

	float alphaRatio = std::clamp(*speed / maxSpeed, 0.0f, 0.8f);

	alpha_ = 0.0f + (0.8f - 0.0f) * alphaRatio;

	orokamono_->SetColor({ 1.0f, 1.0f, 1.0f, alpha_ });

#ifdef _DEBUG
	ImGui::Begin("UI");
	//ImGui::DragFloat("speed", &speed, 1.0f);
	if (ImGui::TreeNode("Meter")) {
		ImGui::DragFloat2("pos", &pos_.x, 1.0f);
		ImGui::DragFloat2("scale", &scale_.x, 1.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Hari")) {
		ImGui::DragFloat2("pos", &hariPos_.x, 1.0f);
		ImGui::DragFloat2("scale", &hariScale_.x, 1.0f);
		ImGui::DragFloat("rotate", &hariRotate_.z, 0.1f);
		ImGui::DragFloat("MaxRotate", &maxAngle, 0.1f);
		ImGui::DragFloat("MinRotate", &minAngle, 0.1f);
		ImGui::DragFloat2("anchor", &hariAnchor_.x, 0.01f, 0.0f, 1.0f);
		ImGui::TreePop();
	}
	ImGui::End();
#endif // _DEBUG

	
}

void UI::Draw()
{
	SpeedSprite_->Draw();
	SpeedMeterSprite_->Draw();
	//orokamono_->Draw();
}
