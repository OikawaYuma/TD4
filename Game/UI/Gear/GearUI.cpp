#include "GearUI.h"
#include <imgui/imgui.h>

GearUI::~GearUI()
{
}

void GearUI::Initialize()
{
	// ギア
	gearSprite_ = std::make_unique<Sprite>();
	gearSprite_->SetTexture(TextureManager::GetInstance()->StoreTexture("Resources/UI/gear.png"));
	gearSprite_->Init("Resources/UI/gear.png");

	gearStateSprite_ = std::make_unique<Sprite>();
	gearStateSprite_->SetTexture(TextureManager::GetInstance()->StoreTexture("Resources/UI/gearState.png"));
	gearStateSprite_->Init("Resources/UI/gearState.png");

	gear_ = 0;
}

void GearUI::Update()
{
	gearSprite_->Update();
	gearSprite_->SetPosition(pos_);
	gearSprite_->SetSize(scale_);

	gearStateSprite_->Update();
	gearStateSprite_->SetPosition(State0Pos_);
	gearStateSprite_->SetSize(StateScale_);

	if (gear_ == 0) {
		gearStateSprite_->SetPosition(State0Pos_);
	}
	else if (gear_ == 1) {
		gearStateSprite_->SetPosition(State1Pos_);
	}
	else if (gear_ == 2) {
		gearStateSprite_->SetPosition(State2Pos_);
	}
	else if (gear_ == 3) {
		gearStateSprite_->SetPosition(State3Pos_);
	}
	else if (gear_ == 4) {
		gearStateSprite_->SetPosition(State4Pos_);
	}
	else if (gear_ == 5) {
		gearStateSprite_->SetPosition(State5Pos_);
	}

	ImGui::Begin("GearUI");
	ImGui::DragInt("State", &gear_, 1.0f);
	if (ImGui::TreeNode("GearUI")) {
		ImGui::DragFloat2("pos", &pos_.x, 1.0f);
		ImGui::DragFloat2("scale", &scale_.x, 1.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("GearHandle")) {
		ImGui::DragFloat2("pos", &State0Pos_.x, 1.0f);
		ImGui::DragFloat2("scale", &StateScale_.x, 1.0f);
		ImGui::TreePop();
	}
	ImGui::End();
}

void GearUI::Draw()
{
	gearSprite_->Draw();
	gearStateSprite_->Draw();
}
