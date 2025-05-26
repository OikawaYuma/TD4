#include "CarSmoke.h"
#include <ModelManager.h>

void CarSmoke::Init()
{
	particle_ = std::make_unique<Particle>();
	particle_->SetModel("ball.obj");
	particle_->Init();
	particle_->SetName("bomb");
	particle_->ApplyGlovalVariables();
	particle_->SetJsonPram();

	emitter_.count = 10;
	emitter_.frequency = 0.015f;
	emitter_.frequencyTime = 0.0f;
	emitter_.transform.rotate = { 0.0f,0.0f,0.0f };
	emitter_.transform.scale = { 0.25f,0.25f,0.25f };
	emitter_.transform.translate = { 0.0f,1.0f,25.0f };
	emitter_.randRangeXYZ =	//発生範囲を設定
	{
		{0.3f,0.7f},
		{0.2f,0.5f},
		{-0.5f,0.3f}
	};
	tex_ = TextureManager::GetInstance()->StoreTexture("Resources/white.png");
	emitter_.size = 0.5f;
	particle_->SetCamera(camera_);
	particle_->SetEmitter(emitter_);
	particle_->SetTexture(tex_);
	particle_->SetScleChangeFlag(false);
}

void CarSmoke::Update()
{
	particle_->Update();
}

void CarSmoke::Draw()
{
	particle_->Draw();
}
