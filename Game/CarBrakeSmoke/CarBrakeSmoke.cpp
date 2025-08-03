#include "CarBrakeSmoke.h"
#include <ModelManager.h>
#include "ParticleManager.h"
#include "ImGuiCommon.h"
#include "GlobalVariables/GlobalVariables.h"
void CarBrakeSmoke::Init()
{
	material_.color = { 0.7f, 0.7f, 0.7f, 1.0f };
	emitter_.count = 15;
	emitter_.frequency = 0.015f;
	emitter_.frequencyTime = 0.0f;
	// textureの読み込み
	tex_ = TextureManager::GetInstance()->StoreTexture("Resources/white.png");
	// leftSmokeの初期化
	leftSmoke_ = std::make_shared<Particle>();
	leftSmoke_->SetModel("ball.obj");
	leftSmoke_->Init();
	leftSmoke_->SetName("carBrakeSmokeLeft");

	leftSmoke_->SetTexture(tex_);
	leftSmoke_->SetScleChangeFlag(true);
	leftSmoke_->SetEmitter(emitter_);
	leftSmoke_->SetIsEmission(true);
	leftSmoke_->AddJsonPram();
	leftSmoke_->ApplyGlovalVariables();

	leftSmoke_->SetMaterial(material_);
	ParticleManager::GetInstance()->AddParticle(leftSmoke_);
	// rightSmokeの初期化
	rightSmoke_ = std::make_shared<Particle>();
	rightSmoke_->SetModel("ball.obj");
	rightSmoke_->Init();
	rightSmoke_->SetName("carBrakeSmokeRight");
	//
	rightSmoke_->SetTexture(tex_);
	rightSmoke_->SetScleChangeFlag(true);
	rightSmoke_->SetEmitter(emitter_);
	leftSmoke_->SetIsEmission(true);
	rightSmoke_->AddJsonPram();
	rightSmoke_->ApplyGlovalVariables();

	rightSmoke_->SetMaterial(material_);
	ParticleManager::GetInstance()->AddParticle(rightSmoke_);

}

void CarBrakeSmoke::Update()
{

}

void CarBrakeSmoke::SetParent(WorldTransform* worldTransform)
{
	leftSmoke_->SetParent(worldTransform);
	rightSmoke_->SetParent(worldTransform);
}
