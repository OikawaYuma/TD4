#include "MiniPlayer.h"

MiniPlayer::~MiniPlayer()
{
}

void MiniPlayer::Initialize()
{
	miniPSprite_ = std::make_unique<Sprite>();
	miniPSprite_->SetTexture(TextureManager::GetInstance()->StoreTexture("Resources/UI/miniplayer.png"));
	miniPSprite_->Init("Resources/UI/miniplayer.png");
	// アンカーポイントの設定
	miniPSprite_->SetAnchorPoint({ 0.5f, 0.5f });
}

void MiniPlayer::Update()
{
	// スプライトの更新
	miniPSprite_->Update();
	miniPSprite_->SetPosition(pos);
	miniPSprite_->SetSize(scale);
	miniPSprite_->SetRot({ 0.0f, 0.0f, rot });


#ifdef _DEBUG
	ImGui::Begin("minimap");
	ImGui::DragFloat2("Scale", &scale.x, 0.1f);
	ImGui::DragFloat("Rotate", &rot, 0.1f);
	ImGui::DragFloat2("Transform", &pos.x, 0.1f);
	ImGui::DragFloat("Speed", speed, 0.1f);
	ImGui::End();
#endif // _DEBUG
}

void MiniPlayer::Draw()
{
	miniPSprite_->Draw();
}

void MiniPlayer::SetFromPlayer(const Vector2& playerPos, float playerRot, const Vector2& mapOrigin, float scaleRate)
{
	// プレイヤー位置からミニマップ座標へ変換
	pos.x = mapOrigin.x - playerPos.x * scaleRate;
	pos.y = mapOrigin.y + playerPos.y * scaleRate;

	// プレイヤーの回転
	rot = playerRot;
}
