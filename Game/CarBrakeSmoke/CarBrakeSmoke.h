#pragma once
#include "Particle.h"

class CarBrakeSmoke
{
public:
	void Init();
	void Update();
public: // Getter
	void SetCamera(Camera* camera) { camera_ = camera; }
	void SetParent(WorldTransform* worldTransform);

	void SetIsEmission(const bool& isEmission) {
		leftSmoke_->SetIsEmission(isEmission);
		rightSmoke_->SetIsEmission(isEmission);
	}
	void DebugEmitter();

private:
	std::shared_ptr<Particle> leftSmoke_;
	std::shared_ptr<Particle> rightSmoke_;
	Emitter emitter_{};
	RandRangePro randRangePro_{};
	Camera* camera_ = nullptr;
	uint32_t tex_ = 0;
	Material material_;
};

