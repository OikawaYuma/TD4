#include "UI.h"

UI::~UI()
{
}

void UI::Initialize()
{
	// スピードメーター
	speedMeterUI_ = std::make_unique<SpeedMeterUI>();
	speedMeterUI_->Initialize();

	// ギア
	gearUI_ = std::make_unique<GearUI>();
	gearUI_->Initialize();
}

void UI::Update()
{
	// スピードメーター
	speedMeterUI_->Update();

	// ギア
	gearUI_->Update();
}

void UI::Draw()
{
	// スピードメーター
	speedMeterUI_->Draw();
	// ギア
	gearUI_->Draw();
}

void UI::SetSpeed(float* speed_)
{
	speedMeterUI_->SetSpeed(speed_);
	//miniUI_->SetSpeed(speed_);
}

void UI::SetGear(int gear)
{

	gearUI_->SteGearNum(gear);
}
