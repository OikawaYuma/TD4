#include "ParticleManager.h"

ParticleManager* ParticleManager::GetInstance() {
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Initialize()
{
}
void ParticleManager::Update(Camera* camera)
{
	for (auto it = particles_.begin(); it != particles_.end(); ) {
		if (it->expired()) {
			it = particles_.erase(it); // erase() は次のイテレータを返す
		}
		else if (auto p = it->lock()) {
			p->Update(camera);
			++it;
		}
		else {
			++it; // lock失敗時も進める（保険）
		}
	}
}

void ParticleManager::Draw()
{
	for (auto& particle : particles_) {
		if (auto p = particle.lock()) {
			p->Draw();
			
		}
	}
}

void ParticleManager::AddParticle(const std::shared_ptr<Particle>& particle)
{
	particles_.push_back(particle); // 共有ポインタをリストに追加
}

