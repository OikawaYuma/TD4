#pragma once
#include "Particle.h"
class CarSmoke
{
public:
	void Init();
	void Update();

public: // Getter
	void SetCamera(Camera* camera) { camera_ = camera;}
	void SetParent(WorldTransform* worldTransform) { particle_->SetParent(worldTransform); }

	void DebugEmitter();

private:
	std::shared_ptr<Particle> particle_;
	Emitter emitter_{};
	RandRangePro randRangePro_{};
	Camera* camera_ = nullptr;
	uint32_t tex_ = 0;
};

