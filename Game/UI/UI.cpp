#include "UI.h"
UI::~UI()
{
}

void UI::Initialize()
{
	// スピードメーター
	speedMeterUI_ = std::make_unique<SpeedMeterUI>();
	speedMeterUI_->Initialize();
}

void UI::Update()
{
	// スピードメーター
	speedMeterUI_->Update();
}

void UI::Draw()
{
	// スピードメーター
	speedMeterUI_->Draw();
}

void UI::SetSpeed(float speed_)
{
	speedMeterUI_->SetSpeed(speed_);
}
