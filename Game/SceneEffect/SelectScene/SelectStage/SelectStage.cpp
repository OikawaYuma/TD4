#include "SelectStage.h"
#include "TextureManager.h"
void SelectStage::Init(int num)
{

	myNum_ = num; // 自分の番号
	gameNumber_[myNum_] = TextureManager::GetInstance()->StoreTexture("Resources/number/number" + std::to_string(myNum_) + ".png");
	

	frameSprite_ = std::make_unique<Sprite>();
	frameSprite_->Init("Resources/stageFrame/stageFrame2.png");
	frameSprite_->SetPosition({ 640.0f + myNum_ * 220, 464.0f }); // Set initial position, adjust as needed
	frameSprite_->SetSize({ 200.0f, 100.0f }); // Set initial position, adjust as needed
	frameSprite_->SetAnchorPoint({ 0.5f, 0.5f });

	numSprite_ = std::make_unique<Sprite>();
	numSprite_->Init("Resources/number/number" + std::to_string(myNum_) + ".png");
	
	numSprite_->SetPosition({ 640.0f + myNum_ * 220, 464.0f }); // Set initial position, adjust as needed
	numSprite_->SetAnchorPoint({ 0.5f, 0.5f });
}

void SelectStage::Update(int num)
{
	frameSprite_->Update();

	// 自分の番号と選ばれた番号の差分
	int offsetFromCenter = myNum_ - num;

	// x座標を中央 (640.0f) からオフセット
	float baseX = 640.0f;
	float offsetX = 220.0f * offsetFromCenter;
	float posX = baseX + offsetX;

	frameSprite_->SetPosition({ posX, 464.0f });
	numSprite_->SetPosition({ posX, 464.0f });

	// 拡大縮小の加算変化量（±10.0f程度）
	float addSize = 0.0f;

	if (num == myNum_) {
		addSize = 10.0f * std::sinf(timer_); // -10.0f〜+10.0f を繰り返す
	}

	// サイズは「固定値 + 変化量」として設定
	numSprite_->SetSize({ 100.0f + addSize, 100.0f + addSize });

	numSprite_->Update();
	timer_ += 0.1f;

	if (timer_ > 6.283f) { // 2π ≒ 6.283
		timer_ = 0.0f;
	}
}

void SelectStage::Draw()
{
	frameSprite_->Draw();
	numSprite_->Draw();
}