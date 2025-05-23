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
	orokamono_->SetPosition({ 50.0f,50.0f });
	orokamono_->SetColor({ 1.0f,1.0f,1.0f,0.6f });

}

void UI::Update()
{
	// アンカーの調整
	float imageHeight = 150.0f;
	float offsetY = 10.0f;
	float anchorY = 1.0f - (offsetY / imageHeight);

	SpeedMeterSprite_->SetAnchorPoint({ 0.5f, anchorY });

	// メーターの針の回転計算
	speedRatio = speed / maxSpeed;
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

	Vector2 pos = orokamono_->GetPosition();
	pos.x += orokamonoVelocity_.x;
	pos.y += orokamonoVelocity_.y;
	if (pos.x < 50 || pos.x > 1230) orokamonoVelocity_.x *= -1;
	if (pos.y < 50 || pos.y > 670) orokamonoVelocity_.y *= -1;
	orokamono_->SetPosition(pos);

	orokamono_->Update();
	orokamono_->SetSize({ 100.0f,100.0f });


	if (Input::GetInstance()->PushKey(DIK_W)) {
		// 加速処理
		speed += acceleration;
		if (speed > maxSpeed) {
			speed = maxSpeed;
		}
	}
	else {
		// 減速処理（自然に減る）
		speed -= deceleration;
		if (speed < minSpeed) {
			speed = minSpeed;
		}
	}

#ifdef _DEBUG
	ImGui::Begin("UI");
	ImGui::DragFloat("speed", &speed, 1.0f);
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
	orokamono_->Draw();
}
