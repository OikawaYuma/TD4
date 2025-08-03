#include "CarSmoke.h"
#include <ModelManager.h>
#include "ParticleManager.h"
#include "ImGuiCommon.h"
void CarSmoke::Init()
{
	particle_ = std::make_shared<Particle>();
	particle_->SetModel("ball.obj");
	particle_->Init();
	particle_->SetName("bomb");
	particle_->SetIsEmission(true);


	emitter_.count = 10;
	emitter_.frequency = 0.015f;
	emitter_.frequencyTime = 0.0f;

	tex_ = TextureManager::GetInstance()->StoreTexture("Resources/white.png");
	
	particle_->SetTexture(tex_);
	particle_->SetScleChangeFlag(true);
	particle_->SetEmitter(emitter_);
	particle_->ApplyGlovalVariables();
	
	ParticleManager::GetInstance()->AddParticle(particle_);

}

void CarSmoke::Update()
{

}



