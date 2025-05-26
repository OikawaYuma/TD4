#include "SpeedMeter.h"

SpeedMeterUI::~SpeedMeterUI()
{
}

void SpeedMeterUI::Initialize()
{
	// メーター本体
	SpeedSprite_ = std::make_unique<Sprite>();
	SpeedSprite_->SetTexture(TextureManager::GetInstance()->StoreTexture("Resources/UI/meter.png"));
	SpeedSprite_->Init("Resources/UI/meter.png");

	// 赤い針
	SpeedMeterSprite_ = std::make_unique<Sprite>();
	SpeedMeterSprite_->SetTexture(TextureManager::GetInstance()->StoreTexture("Resources/UI/hari.png"));
	SpeedMeterSprite_->Init("Resources/UI/hari.png");
	SpeedMeterSprite_->SetRot(hariRotate_);
	// アンカーポイントの設定
	SpeedMeterSprite_->SetAnchorPoint({ 0.5f, 0.9333f });
}

void SpeedMeterUI::Update()
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

	ImGui::Begin("SpeedMetar");
	ImGui::DragFloat("speed", speed, 1.0f);
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
}

void SpeedMeterUI::Draw()
{
	SpeedSprite_->Draw();
	SpeedMeterSprite_->Draw();
}
