#pragma once
#include "WorldTransform.h"
#include <vector>

struct RigidBody {
	WorldTransform* worldTransform = nullptr;
	Vector3 velocity;
	float mass;
	bool useGravity;
};

class PhysicsSystem
{
public:

	// コンストラクタ
	PhysicsSystem();
	// デストラクタ
	~PhysicsSystem();

	// 登録
	void AddObject(RigidBody* body) {
		if (body) {
			bodies_.push_back(body);
		}
	}

	// 適用
	void Apply(float dt);

#pragma region getter

	float GetGravity()const { return gravity_; }

#pragma endregion

private:
	float gravity_; // 重力
	std::vector<RigidBody*>	bodies_;
};

