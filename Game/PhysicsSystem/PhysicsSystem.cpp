#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem()
	: gravity_(-9.8f) // 重力
{
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::Apply(float dt)
{
	for (auto& body : bodies_) {
		// かけるなら
		if (body->useGravity) {
			// 重力をかける
			body->velocity.y += gravity_ * dt;
			// 位置に反映させる
			body->worldTransform->translation_.y += body->velocity.y * dt;
		}
	}
}


