#pragma once
#include "Particle.h"
class CarSmoke
{
public:
	void Init();
	void Update();
	void Draw();

public: // Getter
	void SetCamera(Camera* camera) { camera_ = camera; }
	void SetParent(WorldTransform* worldTransform) { particle_->SetParent(worldTransform); }


private:
	std::unique_ptr<Particle> particle_;
	Emitter emitter_{};
	RandRangePro randRangePro_{};
	Camera* camera_ = nullptr;
	uint32_t tex_ = 0;
};

